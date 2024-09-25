class System
{
public:
    static void setLoginDetails(); //Sets a username and password, encrypt it and save it to a text file
    static void checkLoginDetails(); //Checks an inputted password against a saved and encrypted password
    static void menuSystem(); //Displays the main menu
    static void planeManagement(); //Displays the plane management menu
    static void airportManagement(); //Displays the airport management menu
    static void systemSettings(); //Displays the system settings menu
    static void staffManagement(); //Dispalys the staff management menu
    static void setStaffDetails(); //Creates a CVS file of staff information, source: https://www.geeksforgeeks.org/csv-file-management-using-c/
    static void viewStaffDetails(); //Displays the contents of the CVS file in a 2D array, source: https://www.geeksforgeeks.org/how-to-read-data-from-csv-file-to-a-2d-array-in-cpp/
    static void loadFlightHistory(); //Displays the contents of a CVS file in a 2D array, source: https://www.geeksforgeeks.org/how-to-read-data-from-csv-file-to-a-2d-array-in-cpp/

private:
    static const int encryptionShiftFactor = 4; //Shift factor for the encryption process, determines by how much the letters will be shifted
    static const int decryptionShiftFactor = 22; //Shift factor for the decryption process, determines by how much the letters will be shifted back to give the original string
};