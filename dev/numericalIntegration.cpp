#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "timer.h"

#define M_PER_AU 149597870691
#define KM_PER_AU 1.496e8
#define M_PER_KM 1000
#define SEC_PER_DAY 86400
#define SEC_PER_YR 31556952

#define G 6.67430e-11    // Gravitational constant
#define M_SUN 1.9891e30  // [kg]

struct Date {
  int month;
  int day;
  int year;
  double universalTime = 0.0;

  // returns number of days between date and the J2000 Epoch
  double calcDaysSinceEpoch();
};

struct Coord {
  Coord();
  Coord(double x, double y, double z);

  double magSquared(const Coord &other) const;

  void print() const;

  Coord operator+(const Coord &other) const;
  Coord operator*(const double scalar) const;
  Coord operator*(const Coord &other) const;
  Coord operator/(double scalar) const;
  Coord &operator+=(const Coord &other);
  Coord operator-(const Coord &other) const;
  Coord operator/(const Coord &other) const;

  double x, y, z;
};

struct CelestialBody {
  std::string name;
  Coord pos;
  Coord vel;
  double mass;

  double semiMajorAxis;
  double eccentricity;
  double orbitalInclination;
  double longitudeOfAscendingNode;
  double longitudeOfPerihelion;
  double meanAnomaly;
};

// requests and returns text input from user
std::string getString(const std::string &prompt);

// requests date input from user and returns days since J200 epoch
double getDate();

// displays formatted results
void printResults(std::vector<CelestialBody> planets);

// Returns heliocentric acceleration vector [m/s/s]
Coord calcAcc(const CelestialBody &p1, const CelestialBody &p2);

// Adds together acceleration vectors produced by the gravitational force of the
// sun, and for every planet except itself
Coord sumAcc(const CelestialBody &p, const std::vector<CelestialBody> &planets);

// returns numerical approximation of Eccentric Anomaly (E) using the
// Newton-Raphson method
double calcEccentricAnomaly(double eccentricity, double meanAnomaly);

// calculates heliocentric position and velocity vectors
void populateStateVectors(CelestialBody &planet);

// Approximate new state vectors for a given interval using 4th-Order
// Runge-Kutta. Returns updated Celestial Body
CelestialBody rungeKuttaStep(const CelestialBody &p,
                             const std::vector<CelestialBody> &planets, int dt);

// converts degrees to a value between 0 and 360
double normalizeDegrees(double x);

// converts degrees to radians
double toRadians(double degrees);

// Scales a value from a current maximum range to a new maximum range
size_t scaleValue(double x, size_t currMax, size_t newMax);

int main() {
  const double daysFromEpoch = getDate();
  Timer timer;
  const int dt = 600;  // 10-minute intervals
  const int steps = round(SEC_PER_DAY * daysFromEpoch / double(dt));

  // Orbital elements (and mass) available from NASA. In [AU] and degrees
  CelestialBody mercury = {"mercury",  Coord(), Coord(),  3.3011e23, 0.38709893,
                           0.20563069, 7.00487, 48.33167, 77.45645,  174.796};
  CelestialBody venus = {"venus",    Coord(), Coord(),  4.8675e24, 0.72333199,
                         0.00677323, 3.39471, 76.68069, 131.53298, 50.45};
  CelestialBody earth = {"earth",    Coord(), Coord(),   5.97237e24, 1.00000011,
                         0.01671022, 0.00005, -11.26064, 102.94719,  357.51716};
  CelestialBody mars = {"mars",     Coord(), Coord(),  6.4171e23, 1.52366231,
                        0.09341233, 1.85061, 49.57854, 336.04084, 19.387};
  CelestialBody jupiter = {"jupiter",  Coord(),    Coord(), 1.8982e27,
                           5.20336301, 0.04839266, 1.30530, 100.55615,
                           14.75385,   20.020};
  CelestialBody saturn = {"saturn",   Coord(), Coord(),   5.6834e26, 9.53707032,
                          0.05415060, 2.48446, 113.71504, 92.43194,  317.020};
  CelestialBody uranus = {"uranus",   Coord(), Coord(),  8.6810e25, 19.19126393,
                          0.04716771, 0.76986, 74.22988, 170.96424, 142.238600};
  CelestialBody neptune = {"neptune",   Coord(),    Coord(), 1.02413e26,
                           30.06896348, 0.00858587, 1.76917, 131.72169,
                           44.97135,    259.883};

  std::vector<CelestialBody> planets = {mercury, venus,  earth,  mars,
                                        jupiter, saturn, uranus, neptune};

  for (CelestialBody &p : planets) {
    // Convert AU to meters and degrees to radians
    p.orbitalInclination = toRadians(p.orbitalInclination);
    p.longitudeOfAscendingNode = toRadians(p.longitudeOfAscendingNode);
    p.longitudeOfPerihelion = toRadians(p.longitudeOfPerihelion);
    p.meanAnomaly = toRadians(p.meanAnomaly);
    p.semiMajorAxis *= M_PER_AU;

    // derive initial state vectors
    populateStateVectors(p);
  }

  for (int i = 0; i < steps; i++) {
    std::vector<CelestialBody> updatedBodies;
    std::transform(planets.begin(), planets.end(),
                   std::back_inserter(updatedBodies),
                   [planets, dt](const CelestialBody &p) {
                     return rungeKuttaStep(p, planets, dt);
                   });
    planets = updatedBodies;
  }

  printResults(planets);
}

Coord calcAcc(const CelestialBody &p1, const CelestialBody &p2) {
  double gravitationalFactor = G * (p1.mass + p2.mass);
  const double distSquared = p2.pos.magSquared(p1.pos);

  // Newton's law of universal gravitation
  return p1.pos * -gravitationalFactor / (std::sqrt(distSquared) * distSquared);
}

Coord sumAcc(const CelestialBody &p,
             const std::vector<CelestialBody> &planets) {
  const CelestialBody sun = {"Sun", Coord(), Coord(), M_SUN};
  const Coord acc = calcAcc(p, sun);

  return std::accumulate(
      planets.begin(), planets.end(), acc,
      [&](const Coord &totalAcc, const CelestialBody &other) {
        return p.mass != other.mass ? totalAcc + calcAcc(p, other) : totalAcc;
      });
};

CelestialBody rungeKuttaStep(const CelestialBody &p,
                             const std::vector<CelestialBody> &planets,
                             int dt) {
  const Coord k1v = sumAcc(p, planets) * dt;
  const Coord k1r = p.vel * dt;
  const CelestialBody k1CelestialBody{"", p.pos + k1r * 0.5, p.vel + k1v * 0.5,
                                      p.mass};

  const Coord k2v = sumAcc(k1CelestialBody, planets) * dt;
  const Coord k2r = (p.vel + k1v * 0.5) * dt;
  const CelestialBody k2CelestialBody{"", p.pos + k2r * 0.5, p.vel + k2v * 0.5,
                                      p.mass};

  const Coord k3v = sumAcc(k2CelestialBody, planets) * dt;
  const Coord k3r = (p.vel + k2v * 0.5) * dt;
  const CelestialBody k3CelestialBody{"", p.pos + k3r, p.vel + k3v, p.mass};

  const Coord k4v = sumAcc(k3CelestialBody, planets) * dt;
  const Coord k4r = (p.vel + k3v) * dt;

  CelestialBody updatedBody = p;
  updatedBody.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1 / 6.0);
  updatedBody.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1 / 6.0);

  return updatedBody;
}

double calcEccentricAnomaly(double eccentricity, double meanAnomaly) {
  const double e = eccentricity;
  const double M = meanAnomaly;
  auto isConverging = [](int count) { return count < 19; };

  // Kepler's equation
  double E = M + e * sin(M) * (1 + e * cos(M));

  // Newton's method
  double delta;
  int iterationCount = 0;
  do {
    const double E1 = E - (E - e * sin(E) - M) / (1 - e * cos(E));
    delta = abs(E1 - E);
    E = E1;
    iterationCount++;
  } while (delta >= 0.00001 && isConverging(iterationCount));

  // Newton's method does not work on hyperbolic orbits
  if (!isConverging(iterationCount)) {
    throw std::domain_error(
        "eccentricity too high. unable to converge\ndelta: " +
        std::to_string(delta));
  }

  return E;
}

void populateStateVectors(CelestialBody &planet) {
  // Orbital elements normalized to J2000
  const double a = planet.semiMajorAxis;
  const double e = planet.eccentricity;
  const double o = planet.longitudeOfAscendingNode;
  const double p = planet.longitudeOfPerihelion;
  const double i = planet.orbitalInclination;

  // Mean anomaly
  const double M = planet.meanAnomaly;
  const double E = calcEccentricAnomaly(e, M);

  // Position in 2D orbital plane
  const double xv = a * (cos(E) - e);
  const double yv = a * (sqrt(1.0 - e * e) * sin(E));

  // The True Anomaly (v)
  const double v = atan2(yv, xv);

  // The radius vector (r)
  const double r = sqrt(xv * xv + yv * yv);

  // vector components used for both position and velocity
  const double xh = cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i);
  const double yh = sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i);
  const double zh = sin(v + p - o) * sin(i);

  // Heliocentric position in 3D space
  planet.pos = {r * xh, r * yh, r * zh};

  // Standard gravitational parameter (mu)
  const double mu = G * (M_SUN + planet.mass);

  // Vis-Viva equation
  const double orbitalSpeed = sqrt(mu * (2.0 / r - 1.0 / a));

  // Heliocentric orbital velocity vector in 3D space, assuming the satellite's
  // motion is counterclockwise
  planet.vel = {orbitalSpeed * -yh, orbitalSpeed * xh, orbitalSpeed * zh};
}

std::string getString(const std::string &prompt) {
  std::string input = "";
  std::cout << prompt;
  getline(std::cin, input);

  while (input == "") {
    std::cout << "No characters entered, try again." << std::endl;
    std::cout << prompt;
    getline(std::cin, input);
  }

  return input;
}

double getDate() {
  bool isFormatted;
  Date date;
  double days;

  do {
    isFormatted = true;
    int deliminatorCount = 0;
    std::string numAsString = "0";

    const std::string dateAsString = getString("Enter a date (MM/DD/YYYY): ");

    for (const char character : dateAsString) {
      if (character == '/') {
        deliminatorCount += 1;

        if (deliminatorCount == 1) {
          date.month = stoi(numAsString);
          numAsString = "0";
        } else if (deliminatorCount == 2) {
          date.day = stoi(numAsString);
          numAsString = "0";
        }
      } else {
        numAsString += character;
      }
    }

    date.year = stoi(numAsString);

    if (!(deliminatorCount == 2 && date.month > 0 && date.month <= 12 &&
          date.day > 0 && date.day <= 31)) {
      std::cout << "Date formatted incorrectly, try again" << std::endl;
      isFormatted = false;
    }

    days = date.calcDaysSinceEpoch();

    if (days < 0 || days > 36525) {
      std::cout << "Date must be greater than 1/1/2000" << std::endl;
      isFormatted = false;
    }
  } while (!isFormatted);

  return days;
}

void printResults(std::vector<CelestialBody> planets) {
  for (CelestialBody p : planets) {
    std::cout << "----------------------------------\n";
    std::cout << std::setw(27) << "Name: " << p.name << "\n";
    std::cout << std::setw(27) << "Distance from Sun [AU]: ";
    std::cout << sqrt(p.pos.magSquared(Coord())) / M_PER_AU << "\n";
    std::cout << std::setw(27) << "Distance from Earth [AU]: ";
    std::cout << sqrt(p.pos.magSquared(planets.at(2).pos)) / M_PER_AU << "\n";
    std::cout << std::setw(27) << "Vel [km/sec]: ";
    std::cout << sqrt(p.vel.magSquared(Coord())) / M_PER_KM << std::endl;
  }
}

double normalizeDegrees(double x) { return x - floor(x / 360.0) * 360.0; }

double toRadians(double degrees) { return degrees * (M_PI / 180.0); }

size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

double Date::calcDaysSinceEpoch() {
  // intentional integer division
  double totDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                   3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                   275 * month / 9 + day - 730515;
  totDays = totDays + universalTime / 24.0;

  return totDays - 1;
}

Coord::Coord() : x(0), y(0), z(0) {};

Coord::Coord(double x, double y, double z) : x(x), y(y), z(z) {};

double Coord::magSquared(const Coord &other) const {
  const double xD = other.x - x;
  const double yD = other.y - y;
  const double zD = other.z - z;
  return xD * xD + yD * yD + zD * zD;
}

void Coord::print() const {
  std::cout << std::left << std::fixed << std::setprecision(5)
            << "X: " << std::setw(14) << x << "Y: " << std::setw(14) << y
            << "Z: " << std::setw(14) << z << '\n'
            << std::right;
}

Coord Coord::operator+(const Coord &other) const {
  return {x + other.x, y + other.y, z + other.z};
}

Coord Coord::operator-(const Coord &other) const {
  return {x - other.x, y - other.y, z - other.z};
}

Coord Coord::operator*(const double scalar) const {
  return {x * scalar, y * scalar, z * scalar};
}

Coord Coord::operator*(const Coord &other) const {
  return {x * other.x, y * other.y, z * other.z};
}

Coord Coord::operator/(const Coord &other) const {
  return {x / other.x, y / other.y, z / other.z};
}

Coord Coord::operator/(double scalar) const {
  scalar = 1.0 / scalar;
  return {x * scalar, y * scalar, z * scalar};
}

Coord &Coord::operator+=(const Coord &other) {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}
