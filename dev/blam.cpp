#ifndef VAN_H
#define VAN_H
#include <string>
#include <vector>
using namespace std;

/**
   This class models a shuttle van.
*/
class Van
{
public:
   /**
      Constructs a van with a given capacity.
      @param max_passengers the maximum number of passengers that this
      van can hold
   */
   Van(int max_passengers);

   /**
      Boards a passengers up to the capacity of this van.
      @param name the name of the passenger attempting
      to board
   */
   void board(string name);

   /**
      Drives the van and discharges the passengers.
      @param distance the distance driven
   */
   void drive(double distance);

   /**
      Gets the passengers in this van.
      @return the number of passengers
   */
   vector<string> get_passengers() const;

   /**
      Gets the number of miles that this van has driven.
      @return the number of miles
   */
   double get_miles_driven() const;

private:
   double miles_driven;
   vector<string> passengers;
   int capacity;
};
#endif
