#include <string>
using namespace std;

class Airport
{
public:
    string airportName;
    int numGates;
    int numRunways;

    Airport(string a, int b, int c) //Constructor
    {
        airportName = a;
        numGates = b;
        numRunways = c;
    }

    static void setAirport(); //Saves inputted attributes to a text file
    static Airport loadAirport(); //Loads attributes from a text 
    void calculateGate(string callsignInput); //Works out which gate to assign a plane to via searching through the gatesInUse map until an empty value is found
};