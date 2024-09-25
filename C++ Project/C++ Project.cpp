#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream> 
#include <ctime>
#include "Plane.h"
#include "Airport.h"
#include "System.h"
#pragma warning(disable : 4996) //Disables a warning caused by ctime which prevents the program from running, reference: https://stackoverflow.com/questions/13550864/error-c4996-ctime-this-function-or-variable-may-be-unsafe
using namespace std;

map <string, Plane*> planes; //Creates a map of pointers to plane objects
map <int, string> gatesInUse; //Creates a map where in-use gates are the key and callsigns are the value
map <int, string> runwaysInUse; //Creates a map where in-use runways are the key and callsigns are the value
map <float, string> planeSchedule; //Creates a map where the time attribute of a plane is the key and the callsigns are the value
Airport airport = Airport::loadAirport(); //Creates an object using attributes loaded from a file in the loadAirport method

int main()
{
    int num = 0;
    while (cout << "Enter The Month\n" && !(cin >> num) or (num <= 1) or (num >= 12))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please try again!" << endl;
    }

    for (int i = 1; i <= airport.numGates; ++i) //Initialises the gatesInUse map with empty values up to how many gates the airport has
    {
        gatesInUse[i] = "";
    }

    for (int i = 1; i <= airport.numRunways; ++i) //Initialises the runwaysInUse map with empty values up to how many runways the airport has
    {
        runwaysInUse[i] = "";
    }

    System::checkLoginDetails();
}


Plane* Plane::addPlane()
{
    string callsign;

    cout << "Please input the callsign of a plane to add/update:" << endl;
    cin >> callsign;

    bool updatingPlane = false;

    bool invalidInput = true;

    if (planes[callsign]) //Checks whether this callsign already exists
    {
        while (invalidInput)
        {
            string userChoice = "";
            cout << "This plane already exists" << endl << "[1] - Update plane" << endl << "[2] - Return" << endl;
            cin >> userChoice;

            if (userChoice == "1")
            {
                updatingPlane = true;
                invalidInput = false;
            }

            else if (userChoice == "2")
            {
                invalidInput = false;
                return 0; //If the user does not want to add a plane the method returns 0
            }

            else
            {
                cout << "Invalid input!" << endl;
            }
        }
    }

    string type;

    invalidInput = true;

    while (invalidInput)
    {
        cout << "Please input the type of flight (Arrival/Departure):" << endl;
        cin >> type;

        if (type == "Arrival" or type == "Departure") //Checks whether the user inputted one of the 2 intended inputs
        {
            invalidInput = false; //If so the program continues
        }

        else
        {
            cout << "Invalid input!" << endl; //If not the user must enter the input again
        }
    }

    string origin;
    string destination;

    if (type == "Arrival") //If the plane is an arrival the destination is automatically set to the airport's name
    {
        cout << "Please input a origin:" << endl;
        cin >> origin;
        destination = airport.airportName;
    }

    else //Else the plane is a departure and the origin is automatically set to the airport's name
    {
        cout << "Please input a destination:" << endl;
        cin >> destination;
        origin = airport.airportName;
    }

    int altitude = 0;
    while (cout << "Please input an altitude (m):\n" && !(cin >> altitude)) //Loop which activates if an invalid input is entered, reference: https://stackoverflow.com/questions/10349857/how-to-handle-wrong-data-type-input
    {
        cin.clear(); //Clears wrong input flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Discards old input
        cout << "Invalid input, please try again!" << endl;
    }

    int fuelRemaining = 0;
    while (cout << "Please input fuel remaining (%):\n" && !(cin >> fuelRemaining) or (fuelRemaining < 0) or (fuelRemaining > 100))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please try again!" << endl;
    }

    float speed = 0;
    while (cout << "Please input current speed (m/s):\n" && !(cin >> speed))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please try again!" << endl;
    }

    float distance = 0;
    while (cout << "Please input current distance from destination (km):\n" && !(cin >> distance))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please try again!" << endl;
    }

    float estimatedArrival = 0;
    float distanceInM = distance * 1000; //Multiplies distance by 1000 to convert km to m
    estimatedArrival = distanceInM/speed;
    estimatedArrival = estimatedArrival/60;

    int priority = 0;

    int gate = 0;

    int runway = 0;

    if (updatingPlane == true) //If the plane already exists, it will be deleted and replaced by a new plane with new attributes
    {
        int usedGate = planes[callsign]->gate; //Gets the gate the plane used so that it can be cleared
        if (usedGate != 0)
        {
            gatesInUse[usedGate] = ""; //Clears the value of the key
        }

        int usedRunway = planes[callsign]->runway; //Gets the runway the plane used so that it can be cleared
        if (usedRunway != 0)
        {
            runwaysInUse[usedRunway] = ""; //Clears the value of the key
        }

        float time = planes[callsign]->estimatedArrival; //Gets the plane's key in the schedule map
        planeSchedule.erase(time); //Erases the plane's key and callsign from the schedule map

        delete planes[callsign]; //Deletes the plane from memory
        planes.erase(callsign); //Erases the plane's key and pointer from the planes map
    }

    Plane* newPlane = new Plane(callsign, type, origin, destination, altitude, fuelRemaining, speed, distance, estimatedArrival, priority, gate, runway); //Creates a new object with the inputted attributes

    cout << "Sucessfully added that plane!" << endl;

    return newPlane;
}

void Plane::setPriority(string callsignInput)
{   
    int priorityScore = 0; //Variable used to calculate the priority a plane should have
    
    if (planes[callsignInput]->fuelRemaining >= 5)
    {
        priorityScore += 5;
    }

    else if (planes[callsignInput]->fuelRemaining >= 10)
    {
        priorityScore += 4;
    }

    else if (planes[callsignInput]->fuelRemaining >= 15)
    {
        priorityScore += 3;
    }

    else if (planes[callsignInput]->fuelRemaining >= 20)
    {
        priorityScore += 2;
    }

    else if (planes[callsignInput]->fuelRemaining >= 25)
    {
        priorityScore += 1;
    }

    if (planes[callsignInput]->distance >= 0.5)
    {
        priorityScore += 5;
    }

    else if (planes[callsignInput]->distance >= 1)
    {
        priorityScore += 4;
    }

    else if (planes[callsignInput]->distance >= 1.5)
    {
        priorityScore += 3;
    }

    else if (planes[callsignInput]->distance >= 2)
    {
        priorityScore += 2;
    }

    else if (planes[callsignInput]->distance >= 2.5)
    {
        priorityScore += 1;
    }

    if (planes[callsignInput]->estimatedArrival >= 5)
    {
        priorityScore += 5;
    }

    else if (planes[callsignInput]->estimatedArrival >= 10)
    {
        priorityScore += 4;
    }

    else if (planes[callsignInput]->estimatedArrival >= 15)
    {
        priorityScore += 3;
    }

    else if (planes[callsignInput]->estimatedArrival >= 20)
    {
        priorityScore += 2;
    }

    else if (planes[callsignInput]->estimatedArrival >= 25)
    {
        priorityScore += 1;
    }

    if (priorityScore >= 10) //Calculates the priority from the priority score
    {
        planes[callsignInput]->priority = 3;
    }

    else if (priorityScore >= 5)
    {
        planes[callsignInput]->priority = 2;
    }

    else if (priorityScore < 5)
    {
        planes[callsignInput]->priority = 1;
    }
}

void Plane::searchPlane()
{
    try
    {
        string callsignInput;
        cout << "Input a callsign to search for:" << endl;
        cin >> callsignInput;
        cout << planes[callsignInput]->callsign << ":" << endl; //Outputs  depending on the input given
        cout << "The type of that flight is " << planes[callsignInput]->type << endl;
        cout << "The origin of that plane is " << planes[callsignInput]->origin << endl;
        cout << "The destiantion of that plane is " << planes[callsignInput]->destination << endl;
        cout << "The altitude of that plane is " << planes[callsignInput]->altitude << endl;
        cout << "The fuel remaning of that plane is " << planes[callsignInput]->fuelRemaining << "%" << endl;
        cout << "The last know speed of that plane is " << planes[callsignInput]->speed << "m/s" << endl;
        cout << "The last know distance of that plane is " << planes[callsignInput]->distance << "km" << endl;
        cout << "The plane will arrive in around " << planes[callsignInput]->estimatedArrival << " minutes" << endl;
        cout << "The plane's assigned gate is Gate " << planes[callsignInput]->gate << endl;
        cout << "The plane has a priority of " << planes[callsignInput]->priority << endl;
    }

    catch (...) //Catches all exceptions, such as a read access violation if the user enters an invalid callsign
    {
        cout << "Invalid callsign entered!" << endl;
    }

}

void Plane::commenceDepartureArrival()
{
    string callsignInput;

    try
    {
        cout << "Input a callsign:" << endl;
        cin >> callsignInput;

        bool duplicateFound = false;

        for (auto& element : runwaysInUse) //Loop that searches the map for any duplicate keys (callsigns)
        {
            if (element.second == callsignInput)
            {
                duplicateFound = true;
                break;
            }
        }

        if ((!(planes[callsignInput])) || (duplicateFound == true)) //Checks to see if a duplicate was found or if the callsign does not exist
        {
            throw (callsignInput);
        }

        int allocatedRunway = 0;
        for (auto& element : runwaysInUse) //Loop that searches the map for any keys (runways) without values (plane callsigns) and assigns a plane an empty runway
        {
            if (element.second.empty())
            {
                element.second = callsignInput;
                allocatedRunway = element.first;
                break;
            }
        }

        planes[callsignInput]->runway = allocatedRunway; //Plane's runway attribute is updated

        cout << callsignInput << " should land at runway " << allocatedRunway << endl;
        cout << "Upon Arrival/Departure the plane should be logged" << endl;
    }

    catch (string callsign) //Catches all exceptions, such as a read access violation if the enters an invalid callsign
    {
        cout << "The callsign " << callsign << " has already been added or is not valid!" << endl;
    }
}

void Plane::logDepartureArrival()
{
    try
    {
        string callsignInput;
        cout << "Input a callsign:" << endl;
        cin >> callsignInput;

        Plane::savePlane(callsignInput); //Saves the plane to a CVS file before it is deleted

        int usedGate = planes[callsignInput]->gate; //Gets the gate the plane used so that it can be cleared
        if (usedGate != 0)
        {
            gatesInUse[usedGate] = ""; //Clears the value of the key
        }

        int usedRunway = planes[callsignInput]->runway; //Gets the runway the plane used so that it can be cleared
        if (usedRunway != 0)
        {
            runwaysInUse[usedRunway] = ""; //Clears the value of the key
        }

        float time = planes[callsignInput]->estimatedArrival; //Gets the plane's key in the schedule map
        planeSchedule.erase(time); //Erases the plane's key and callsign from the schedule map

        delete planes[callsignInput]; //Deletes the plane from memory
        planes.erase(callsignInput); //Erases the plane's key and pointer from the planes map
    }

    catch (...) //Catches all exceptions, such as a read access violation if the enters an invalid callsign
    {
        cout << "Invalid callsign entered!" << endl;
    }
}

void Plane::savePlane(string callsignInput)
{
    fstream planeFile("flightHistory.cvs", ios::out | ios::app); //Creates or opens the flightHistory file

    string callsign = planes[callsignInput]->callsign;

    string type = planes[callsignInput]->type;

    string origin = planes[callsignInput]->origin;

    string destination = planes[callsignInput]->destination;

    time_t currentTime = time(0); //Gets the current time based on the system's time, reference: https://www.tutorialspoint.com/cplusplus/cpp_date_time

    char* stringTime = ctime(&currentTime); //Converts the current time to a string form, reference: https://www.tutorialspoint.com/cplusplus/cpp_date_time

    planeFile << callsign << ", " //Adds the variables to the cvs file
        << type << ", "
        << origin << ", "
        << destination << ", "
        << stringTime
        << "\n";

    planeFile.close();
}

void Plane::addSchedule(string callsignInput)
{
    float time = planes[callsignInput]->estimatedArrival; //Gets the time attribute of a plane depending on the inputted callsign
    planeSchedule.insert({time, callsignInput}); //Adds that plane's time and callsign to the map
}

void Airport::setAirport()
{
    ofstream airportFile("airport.txt");

    string airportName;
    cout << "Please enter the name of the Airport:" << endl;
    cin >> airportName;

    int numGates;

    while (cout << "Please enter the number of gates:\n" && !(cin >> numGates))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please try again!" << endl;
    }

    int numRunways;

    while (cout << "Please enter the number of runawys:\n" && !(cin >> numRunways))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please try again!" << endl;
    }

    airportFile << airportName << "\n";
    airportFile << numGates << "\n";
    airportFile << numRunways;

    airportFile.close();

    cout << "Airport saved, please restart the program!" << endl;
    exit(0);
}

Airport Airport::loadAirport()
{
    string airportName;
    int numGates;
    int numRunways;

    ifstream airportFile("airport.txt");

    if (airportFile.good()) //If the file above is found, the program executes as normal
    {
        airportFile >> airportName >> numGates >> numRunways;

        airportFile.close();

        Airport airport = Airport(airportName, numGates, numRunways);
        return airport;
    }

    else //Else the program instead moves execution to the setAirport method to create a loginDetails file
    {
        cout << "Airport not found, creating now..." << endl;
        Airport::setAirport();
    }
}

void Airport::calculateGate(string callsignInput)
{
    int allocatedGate = 0;
    for (auto& element : gatesInUse) //Loops through the gatesInuse map to check if any keys have empty values and assigns a value to that key if so
    {
        if (element.second.empty())
        {
            element.second = callsignInput;
            allocatedGate = element.first;
            planes[callsignInput]->gate = allocatedGate; //Plane's gate attribute is updated
            break;
        }
    }

    planes[callsignInput]->gate = allocatedGate;
}

void System::setLoginDetails()
{

    bool loop;
    loop = true;
    string userInput;
    string username;
    string password;

    while (loop)
    {
        cout << "Please input a username:" << endl;
        cin >> username;
        cout << "Please input a password:" << endl;
        cin >> password;
        cout << "The username you have chosen is: " << username << " and the password you have chosen is: " << password << endl;
        cout << "[1] - Continue" << endl << "[2] - Re-enter Details" << endl;
        cin >> userInput;
        if (userInput == "1")
        {
            loop = false;
            string encryptedPassword = ""; //This part of the method encrypts the password, reference: https://www.geeksforgeeks.org/caesar-cipher-in-cryptography/
            int shiftFactor = encryptionShiftFactor; //Traverse text
            for (int i = 0; i < password.length(); i++)  //Apply transformation to each character
            {
                if (isupper(password[i])) //Encrypt Uppercase letters
                {
                    encryptedPassword += char(int(password[i] + shiftFactor - 65) % 26 + 65);
                }

                else //Encrypt Lowercase letters
                {
                    encryptedPassword += char(int(password[i] + shiftFactor - 97) % 26 + 97);
                }
            }

            ofstream loginFile("loginDetails.txt"); //A new text file is created (or overwritten) and the user's details are saved

            loginFile << username << "\n";
            loginFile << encryptedPassword;

            cout << "Login details successfully set!" << endl;
            loginFile.close();
            System::checkLoginDetails();
        }

        else
        {
            cout << "Resetting..." << endl;
        }
    }
  
}

void System::checkLoginDetails()
{
    ifstream loginFile("loginDetails.txt");
    if (loginFile.good()) //If the file above is found, the program executes as normal
    {
        string username;
        string encryptedPassword;
        string decryptedPassword = "";
        string enteredPassword;
        int shiftFactor = decryptionShiftFactor; //Shift factor, was 4 earlier as is now 22 as 26-4 = 22
        int numAttempts = 3;

        loginFile >> username >> encryptedPassword;

        loginFile.close();

        //This part of the method decrypts the password, reference: https://www.geeksforgeeks.org/caesar-cipher-in-cryptography/
        
        for (int i = 0; i < encryptedPassword.length(); i++) //Traverse text
        {
            //Apply transformation to each character
            if (isupper(encryptedPassword[i])) //Encrypt Uppercase letters
            {
                decryptedPassword += char(int(encryptedPassword[i] + shiftFactor - 65) % 26 + 65);
            }
            
            else //Encrypt Lowercase letters
            {
                decryptedPassword += char(int(encryptedPassword[i] + shiftFactor - 97) % 26 + 97);
            }
        }

        while (numAttempts > 0) //While the user has more than 0 attempts, they can attempt to enter their passowrd
        {
            cout << "Welcome " << username << " please input your password:" << endl;
            cin >> enteredPassword;

            if (enteredPassword == decryptedPassword)
            {
                cout << "Password match, logging in..." << endl;
                numAttempts = -1; //User's attempts are set to -1 to stop the max number of attempts message from appearing
                System::menuSystem();
            }

            else
            {
                numAttempts -= 1; //User gets one less attempt after entering an incorrect password
                cout << "Incorrect password " << numAttempts << " attempts remaining" << endl;
            }
        }

        if (numAttempts == 0)
        {
            cout << "You have reached the maximum number of attempts!" << endl;
            exit(0);
        }
    }

    else //Else the program instead moves execution to the setLoginDetails method to create a loginDetails file
    {
        cout << "Login details not found, entering set-up" << endl;
        System::setLoginDetails();
    }
}

void System::menuSystem()
{
    bool loop;
    loop = true;
    string userInput;

    while (loop)
    {
        cout << "\n******Main Menu*****" << endl << "What would you like to do?" << endl << "[1] - Plane Management" << endl << "[2] - Airport Management" << endl << "[3] - System Settings" << endl << "[4] - Help" << endl << "[5] - Exit" << endl;
        cin >> userInput;

        if (userInput == "1")
        {
            loop = false;
            System::planeManagement();
        }

        else if (userInput == "2")
        {
            loop = false;
            System::airportManagement();
        }

        else if (userInput == "3")
        {
            loop = false;
            System::systemSettings();
        }

        else if (userInput == "4")
        {
            cout << "To get started with the program, first add a plane from the plane management menu. Then, you can search for this plane via its callsign, see this plane in the schedule or view the flight history. When a plane requests to land, select commence arrival/departure and then log arrival/departure once the plane has landed. Airport settings can be editted in manage airport and staff details can be added to the program in system management." << endl;
        }

        else if (userInput == "5")
        {
            loop = false;
            cout << "Quitting..." << endl;
            exit(0);
        }

        else
        {
            cout << "That is not a valid input!" << endl;
        }
    }
}

void System::planeManagement()
{
    bool loop;
    loop = true;
    string userInput;

    while (loop)
    {
        cout << "\n******Plane Management*****" << endl << "What would you like to do?" << endl << "[1] - Add/Update Plane" << endl << "[2] - Search Plane via Callsign" << endl << "[3] - View Schedule" << endl << "[4] - Commence Arrival/Departure" << endl << "[5] - Log Arrival/Departure" << endl << "[6] - Display Flight History" << endl << "[7] - Return" << endl;
        cin >> userInput;

        if (userInput == "1")
        {
            string lastValue = gatesInUse.rbegin()->second; //Gets the value of the last key in the gatesInUseMap, used to check whether it is empty or not, reference: https://stackoverflow.com/questions/289715/last-key-in-a-stdmap
            if (!(lastValue == "")) //If it is not  empty, a new plane cannot be made
            {
                cout << "Unable to add a plane, max gates used!" << endl;
            }

            else
            {
                Plane* newPlane = Plane::addPlane(); //A new plane object is created from the plane add method

                if (newPlane == 0) //This if statement is executed if the user does want to create or update a plane
                {
                    cout << "No plane created or updated!" << endl;
                }

                else
                {
                    planes[newPlane->callsign] = newPlane; //The new plane object's pointer is added to the map with its callsign as its key

                    string callsignInput = newPlane->callsign; //The new plane's callsign is obtained

                    airport.calculateGate(callsignInput); //Calculate gate function is called to give the new plane a gate number

                    Plane::addSchedule(callsignInput); //Adds the plane to the schedule
                    Plane::setPriority(callsignInput); //Calculates the plane's priority

                    cout << "That plane should arrive at Gate " << newPlane->gate << endl;
                }
            }
        }

        else if (userInput == "2")
        {
            Plane::searchPlane();
        }

        else if (userInput == "3")
        {
            if (planeSchedule.empty())
            {
                cout << "There are no planes in the schedule!" << endl;
            }

            else
            {
                for (const auto& element : planeSchedule) //Loops through the planeSchedule map and outputs it, reference: https://stackoverflow.com/questions/14070940/how-can-i-print-out-c-map-values
                {
                    cout << element.first << " minutes" << " | " << element.second << endl;
                }
            }
        }

        else if (userInput == "4")
        {
            bool emptyRunwayFound = false;

            for (auto& element : runwaysInUse) //Loop that checks through the runwaysInUse map and checks if any runways are free
            {
                if (element.second.empty()) 
                {
                    Plane::commenceDepartureArrival(); //If a runway is free the commenceDepartureArrival method is called
                    emptyRunwayFound = true;
                    break;
                }
            }
            if (!emptyRunwayFound) //If not the user cannot input any departures/arrivals
            {
                cout << "Unable to commence departure/arrival, no runways are free!" << endl;
            }
        }

        else if (userInput == "5")
        {
            Plane::logDepartureArrival();
        }

        else if (userInput == "6")
        {
            System::loadFlightHistory();
        }

        else if (userInput == "7")
        {
            loop = false;
            System::menuSystem();
        }

        else
        {
            cout << "That is not a valid input!" << endl;
        }
    }
}

void System::airportManagement()
{
    bool loop;
    loop = true;
    string userInput;

    while (loop)
    {
        cout << "\n******Airport Management*****" << endl << "What would you like to do?" << endl << "[1] - View Airport" << endl << "[2] - Set Airport Settings" << endl << "[3] - Return" << endl;
        cin >> userInput;

        if (userInput == "1")
        {   
            cout << "The current airport is: " << endl << airport.airportName << endl << airport.numGates << " gates" << airport.numRunways << " runways" << endl;
        }

        else if (userInput == "2")
        {
            Airport::setAirport();
        }

        else if (userInput == "3")
        {
            loop = false;
            System::menuSystem();
        }

        else
        {
            cout << "That is not a valid input!" << endl;
        }
    }
}

void System::systemSettings()
{
    bool loop;
    loop = true;
    string userInput;

    while (loop)
    {
        cout << "\n******System Management*****" << endl << "What would you like to do?" << endl << "[1] - Change login details" << endl << "[2] - Staff Management" << endl << "[3] - Return" << endl;
        cin >> userInput;

        if (userInput == "1")
        {
            System::setLoginDetails();
        }

        else if (userInput == "2")
        {
            loop = false;
            System::staffManagement();
        }

        else if (userInput == "3")
        {
            loop = false;
            System::menuSystem();
        }

        else
        {
            cout << "That is not a valid input!" << endl;
        }
    }
}

void System::setStaffDetails()
{
    int numStaff = 0;
    int ID;
    string firstName;
    string lastName;
    string phoneNumber;
    string workHours;

    ofstream staffFile("staffDetails.cvs");  //Creates or overrides a CVS file

    while (cout << "Enter the number of staff you wish to add:\n" && !(cin >> numStaff))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, please try again!" << endl;
    }

    cout << "Enter the details of " << numStaff << " staff" << endl << "ID | First Name | Last Name | Phone Number | Work Hours" << endl;

    for (int i = 0; i < numStaff; i++) //Takes inputs and saves them to a CVS file, reference: https://www.geeksforgeeks.org/csv-file-management-using-c/
    {

        cin >> ID
            >> firstName
            >> lastName
            >> phoneNumber
            >> workHours;

        staffFile << ID << ", "
            << firstName << ", "
            << lastName << ", "
            << phoneNumber << ", "
            << workHours
            << "\n";

        cout << "Staff member added!" << endl;
    }
    staffFile.close();
}

void System::viewStaffDetails()
{
    ifstream staffFile("staffDetails.cvs", ios::in);

    if (staffFile.good()) //If the file above is found, the program executes as normal
    {
        const int maxRows = 25; //Maximum number of rows for the 2D array to print out the CVS file, reference: https://www.geeksforgeeks.org/how-to-read-data-from-csv-file-to-a-2d-array-in-cpp/

        const int maxCols = 5; //Maximum number of columns for the 2D array

        string data[maxRows][maxCols]; //Define a 2D array to store the CSV data
        string line;
        int row = 0;
       
        while (getline(staffFile, line) && row < maxRows) //Store the CSV data from the CSV file to the 2D array
        { 
            stringstream ss(line);
            string cell;
            int col = 0;
            while (getline(ss, cell, ',') && col < maxCols) 
            {
                data[row][col] = cell;
                col++;
            }
            row++;
        }
      
        staffFile.close(); //Close the file after read opeartion is complete

        cout << "ID | First Name | Last Name | Phone Number | Work Hours" << endl;
        for (int i = 0; i < row; i++) //Print the data stored in the 2D array
        {
            for (int j = 0; j < maxCols && !data[i][j].empty();
                j++) 
            {
                cout << data[i][j] << " ";
            }
            cout << endl;
        }
    }

    else
    {
        cout << "File not found, creating now..." << endl;
        System::setStaffDetails();
    }
}

void System::staffManagement()
{
    bool loop;
    loop = true;
    string userInput;

    while (loop)
    {
        cout << "\n******Staff Management*****" << endl << "What would you like to do?" << endl << "[1] - View staff details" << endl << "[2] - Set staff details" << endl << "[3] - Return" << endl;
        cin >> userInput;

        if (userInput == "1")
        {
            System::viewStaffDetails();
        }

        else if (userInput == "2")
        {
            System::setStaffDetails();
        }

        else if (userInput == "3")
        {
            loop = false;
            System::menuSystem();
        }

        else
        {
            cout << "That is not a valid input!" << endl;
        }
    }
}

void System::loadFlightHistory()
{
    ifstream planeFile("flightHistory.cvs", ios::in);

    if (planeFile.good()) //If the file above is found, the program executes as normal
    {
        const int maxRows = 30; //Maximum number of rows for the 2D array to print out the CVS file, reference: https://www.geeksforgeeks.org/how-to-read-data-from-csv-file-to-a-2d-array-in-cpp/

        const int maxCols = 5; //Maximum number of columns for the 2D array

        string data[maxRows][maxCols]; // Define a 2D array to store the CSV data
        string line;
        int row = 0;

        while (getline(planeFile, line) && row < maxRows) //Store the CSV data from the CSV file to the 2D array 
        { 
            stringstream ss(line);
            string cell;
            int col = 0;
            while (getline(ss, cell, ',') && col < maxCols) 
            {
                data[row][col] = cell;
                col++;
            }
            row++;
        }

        planeFile.close(); //Close the file after read opeartion is complete

        cout << "Callsign | Arrival/Departure | Origin | Destination | Time of Arrival/Departure" << endl;
        for (int i = 0; i < row; i++) //Print the data stored in the 2D array
        {
            for (int j = 0; j < maxCols && !data[i][j].empty();
                j++) 
            {
                cout << data[i][j] << " ";
            }
            cout << endl;
        }
    }

    else
    {
        cout << "Flight history is empty, flights will be added when a flight arrives/departs" << endl;
    }
}