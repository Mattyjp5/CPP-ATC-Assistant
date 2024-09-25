#include <string>
using namespace std;

class Plane
{
public:
    string callsign;
    string type;
    string origin;
    string destination;
    int altitude;
    int fuelRemaining;
    float speed;
    float distance;
    float estimatedArrival;
    int priority;
    int gate;
    int runway;
    Plane(string a, string b, string c, string d, int e, int f, float g, float h, float i, int j, int k, int l) //Constructor
    {
        callsign = a;
        type = b;
        origin = c;
        destination = d;
        altitude = e;
        fuelRemaining = f;
        speed = g;
        distance = h;
        estimatedArrival = i;
        priority = j;
        gate = k;
        runway = l;
    }

    ~Plane() { cout << "Plane deleted!" << endl; } //Destructor

    static Plane* addPlane(); //Adds a plane to the map
    static void setPriority(string callsignInput); //Calculates and sets the plane's priority
    static void searchPlane(); //Searches planes a via their callsign
    static void commenceDepartureArrival(); //Starts the plane arrival/departure process and assigns the plane a runway
    static void logDepartureArrival(); //Deletes a plane object and calls the save method
    static void savePlane(string callsignInput); //Saves certain attributes of a plane based on its callsign
    static void addSchedule(string callsignInput); //Adds a callsign and its estimated time to a map
};
