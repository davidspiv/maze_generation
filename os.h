#include <limits.h>
#include <unistd.h>

#include <iostream>

std::string getWorkingPath() {
  char temp[PATH_MAX];
  return (getcwd(temp, sizeof(temp)) ? std::string(temp) : std::string(""));
}

// Heliocentric plane-of-orbit. ?xyz
// Heliocentric ecliptic. ?XYZ
// Heliocentric equatorial. ?ξηζ
// Geocentric equatorial. /xyz
