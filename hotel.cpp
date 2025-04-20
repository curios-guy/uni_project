#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <sstream>
using namespace std;

int generateUniqueId() {
    ifstream inFile("personIds.txt");
    vector<string> personIDs;
    string line;

    while (getline(inFile, line)) {
        personIDs.push_back(line);
    }
    inFile.close();

    srand(time(NULL));
    int num;
    string numStr;

    while (true) {
        num = (rand() % 999) + 100;
        numStr = to_string((num));

        bool found = false;
        for (string& id : personIDs) {
            if (id == numStr) {
                found = true;
                break;
            }
        }

        if (!found) break;
    }

    ofstream outFile("personIds.txt", ios::app);
    outFile << numStr << "\n";
    outFile.close();

    return num;
}



class Guest{
public:
    int personId;
    string firstName;
    string lastName;
    int stayingRoom = 0;
    Guest(int personId, string fName, string lName) {
        this->personId = personId;
        this->firstName = fName;
        this->lastName = lName;
    }

    void setRoom(int roomNum) {
        this->stayingRoom = roomNum;
    }

};

class Room {
public:
    int roomNumber;
    int roomType = 1;
    double pricePerDay = 130;
    bool isAvailable = true;

    Room(int roomId, int roomType, double pricePerDay, bool isAvailable) {
        this->roomNumber = roomId;
        this->roomType = roomType;
        this->pricePerDay = pricePerDay;
        this->isAvailable = isAvailable;
    }
};
vector <Room> loadRooms() {
    vector <Room> rooms;
    ifstream inFile("rooms.csv");


    string line;
    getline(inFile, line);

    while (getline(inFile, line)) {
        stringstream ss(line);
        string roomNumStr, roomType, pricePerDay, isAvailable;

        getline(ss, roomNumStr, ',');
        getline(ss, roomType, ',');
        getline(ss, pricePerDay, ',');
        getline(ss, isAvailable, ',');

        Room room(stoi(roomNumStr),stoi(roomType), stod(pricePerDay), isAvailable == "true");

        rooms.push_back(room);
    }
    return rooms;
};

vector <Guest> loadGuests() {
    vector <Guest> guests;
    ifstream inFile("guests.csv");

    string line;
    getline(inFile, line);

    while (getline(inFile, line)) {
        stringstream ss(line);
        string personId, firstName, lastName, roomNumber;

        getline(ss, personId, ',');
        getline(ss, firstName, ',');
        getline(ss, lastName, ',');
        getline(ss, roomNumber, ',');

        Guest guest(stoi(personId),firstName, lastName);
        guest.setRoom(stoi(roomNumber));
        guests.push_back(guest);
    }
    return guests;
}

class Manager {
public:
    void registerGuest() {
        string firstName;
        string lastName;
        int roomNum;
        cout << "Firstname: ";
        cin >> firstName;
        cout << "Lastname: ";
        cin >> lastName;
        Guest guest(generateUniqueId(), firstName, lastName);
        ofstream outFile("guests.csv", ios::app);

        if (!(outFile.is_open())) {
            cout << "Error opening the file"<< endl;
            return;
        }
        outFile << guest.personId << ","
        << guest.firstName << ","
        << guest.lastName << ","
        << guest.stayingRoom<< "\n";

        outFile.close();

        cout << endl;
        cout << "***********************" << endl;
        cout << "Guest added successfully" << endl;
        cout << "***********************" << endl;
        cout << endl;
    };

    void bookRoom() {
        cout << "Input guest's personal ID: ";
        int guestId;
        cin >> guestId;
        bool guestFound = false;
        vector <Guest> guests = loadGuests();
        vector<Room> rooms = loadRooms();
        vector <Room> availableRooms;

        cout << endl;
        cout << "Personal ID" << "\t" << "FirstName" << "\t" << "LastName" << "\t" << "Booked Room" << endl;
        cout << "---------------------------------------------------------------"<< endl;
        for (Guest& g : guests) {
            if (guestId == g.personId) {
                guestFound = true;
                cout << g.personId << "\t\t" << g.firstName << "\t\t" << g.lastName << "\t\t";
                if (g.stayingRoom == 0) {
                    cout << "no room booked";
                } else {
                    cout << g.stayingRoom;
                }
                cout << endl;
                cout << endl;
                cout << "Room Number" << "\t" << "Room Type" << "\t" << "Price" << "\t\t" << "Availabilty"<< endl;
                cout << "---------------------------------------------------------------"<< endl;
                for (const Room& r : rooms) {
                    if (r.isAvailable) {
                        cout << r.roomNumber << "\t\t" << (r.roomType == 1 ? "Regular" : "Deluxe") << "\t\t$" << r.pricePerDay << "\t\t"
                         << (r.isAvailable ? "Available" : "Occupied") << endl;
                    }
                }

                cout << endl;
                cout << "Guest's choice (input room number): ";
                int choice;
                cin >> choice;
                bool validChoice = false;
                for (Room& r : rooms) {
                    if (choice == r.roomNumber && r.isAvailable) {
                        r.isAvailable = false;
                        g.stayingRoom = choice;
                        validChoice = true;
                        break;
                    }
                }

                if (!validChoice) {
                    cout << endl;
                    cout << "***********************" << endl;
                    cout << "Invalid or unavailable room selected" << endl;
                    cout << "***********************" << endl;
                    cout << endl;
                    return;
                }
            }
        }

        if (!guestFound) {
            cout << endl;
            cout << "***********************" << endl;
            cout << "Guest not found" << endl;
            cout << "***********************" << endl;
            cout << endl;
            return;
        }

        ofstream outFile("guests.csv");
        outFile << "ID,FirstName,Lastname,RoomNumber" << endl; // CSV header
        for (Guest& g : guests) {
            outFile << g.personId << ","
                 << g.firstName << ","
                 << g.lastName << ","
                 << g.stayingRoom << endl;
        }

        ofstream outFile1("rooms.csv");
        outFile1 << "Room Number,Room Type,Price,Availability" << endl; // CSV header
        for (const Room& r : rooms) {
            outFile1 << r.roomNumber << ","
                 << r.roomType << ","
                 << r.pricePerDay << ","
                 << (r.isAvailable ? "true" : "false") << endl;
        }

        cout << endl;
        cout << "***********************" << endl;
        cout << "Room booked successfully" << endl;
        cout << "***********************" << endl;
        cout << endl;

    }

    void viewGuests() {
        vector<Guest> guests = loadGuests();

        cout << endl;
        cout << "Personal ID" << "\t" << "FirstName" << "\t" << "LastName" << "\t" << "Booked Room" << endl;
        cout << "---------------------------------------------------------------" << endl;
        for (const Guest& g : guests) {
            cout << g.personId << "\t\t" << g.firstName << "\t\t" << g.lastName << "\t\t";
            if (g.stayingRoom == 0) {
                cout << "no room booked";
            } else {
                cout << g.stayingRoom;
            }
            cout << endl;
        }
        cout << endl;
    }

    //Room code
    void addRoom() {
        int roomNum;
        int roomType;
        double pricePerDay;
        bool isAvailable;
        char availability;
        cout << "Room number: ";
        cin >> roomNum;
        cout << "Room type: \n\t1 for Regular \n\t2 for Deluxe \nChoose: ";
        cin >> roomType;
        if (roomType != 1 && roomType != 2) {
            cout << "Invalid room type!"<< endl;
            return;
        }
        cout << "Price per day: ";
        cin >> pricePerDay;
        cout << "Is available? [Y/N]: ";
        cin >> availability;
        if (toupper(availability) == 'N') {
            isAvailable = false;
        }else {
            isAvailable = true;
        };

        Room room(roomNum, roomType, pricePerDay, isAvailable);

        //adding data to file
        ofstream outFile("rooms.csv", ios::app);
        if (!(outFile.is_open())) {
            cout << "Error opening the file"<< endl;
            return;
        }
        outFile << room.roomNumber << ","
        << room.roomType << ","
        << room.pricePerDay << ","
        << (room.isAvailable ? "true" : "false") << "\n";

        outFile.close();

        cout << endl;
        cout << "***********************" << endl;
        cout << "Room added successfully" << endl;
        cout << "***********************" << endl;
        cout << endl;
    }

    void viewRooms() {
        vector<Room> rooms = loadRooms();
        vector<Guest> guests = loadGuests();

        cout << endl;
        cout << "Room Number" << "\t" << "Room Type" << "\t" << "Price" << "\t\t" << "Availabilty"
            << "\t"<< "Guest FullName"<<endl;
        cout << "---------------------------------------------------------------------------------"<< endl;

        for (const Room& r : rooms) {
            bool hasGuest = false;

            for (const Guest& g : guests) {
                if (r.roomNumber == g.stayingRoom) {
                    cout << r.roomNumber << "\t\t"
                         << (r.roomType == 1 ? "Regular" : "Deluxe") << "\t\t$"
                         << r.pricePerDay << "\t\t"
                         << (r.isAvailable ? "Available" : "Occupied") << "\t"
                         << g.firstName + " " + g.lastName << endl;
                    hasGuest = true;
                    break;
                }
            }

            if (!hasGuest) {
                cout << r.roomNumber << "\t\t"
                     << (r.roomType == 1 ? "Regular" : "Deluxe") << "\t\t$"
                     << r.pricePerDay << "\t\t"
                     << (r.isAvailable ? "Available" : "Occupied") << "\t"
                     << "No Guest" << endl;
            }
        }
        cout << endl;

    };

    void modifyRoom() {
        cout << endl;
        cout << "Please input room number: ";
        int roomNum;
        cin >> roomNum;
        cout << endl;
        bool roomFound = false;

        vector<Room> rooms = loadRooms();
        for (Room& r : rooms) {
            if (r.roomNumber == roomNum) {
                roomFound = true;
                cout << "Room Number" << "\t" << "Room Type" << "\t" << "Price" << "\t\t" << "Availabilty"<< endl;
                cout << "---------------------------------------------------------------"<< endl;
                cout << r.roomNumber << "\t\t" << r.roomType << "\t\t$" << r.pricePerDay << "\t\t"
                 << (r.isAvailable ? "Available" : "Occupied") << endl;
                cout << "Enter new Room Type: \n\t1 for Regular \n\t2 for Deluxe";
                cin >> r.roomType;
                cout << "Enter new Price: ";
                cin >> r.pricePerDay;
                cout << "Is Available? [Y/N]: ";
                char availability;
                cin >> availability;
                r.isAvailable = (availability == 'Y' || availability == 'y') ? true : false;
                break;
            }
        }
        cout << endl;
        if (!roomFound) {
            cout << "Room not found"<< endl;
            return;
        }

        // Save modified rooms back to CSV
        ofstream outFile("rooms.csv");
        outFile << "Room Number,Room Type,Price,Availability" << endl; // CSV header
        for (const Room& r : rooms) {
            outFile << r.roomNumber << ","
                 << r.roomType << ","
                 << r.pricePerDay << ","
                 << (r.isAvailable ? "true" : "false") << endl;
        }

        cout << endl;
        cout << "***********************" << endl;
        cout << "Room updated successfully" << endl;
        cout << "***********************" << endl;
        cout << endl;
    }

    void deleteRoom() {
        cout << "Please input room number to delete: ";
        int roomNum;
        cin >> roomNum;

        vector<Room> rooms = loadRooms();
        bool roomFound = false;

        // Loop through the rooms and remove the matching room
        for (int i = 0; i < rooms.size(); i++) {
            if (rooms[i].roomNumber == roomNum) {
                rooms.erase(rooms.begin() + i);
                roomFound = true;
                cout << endl;
                cout << "***********************" << endl;
                cout << "Room " << roomNum << " deleted" << endl;
                cout << "***********************" << endl;
                cout << endl;
                break;
            }
        }

        if (!roomFound) {
            cout << endl;
            cout << endl;
            cout << "***********************" << endl;
            cout << "Room not found" << endl;
            cout << "***********************" << endl;
            cout << endl;
            return;
        }

        // Save the updated list back to the CSV
        ofstream outFile("rooms.csv");
        outFile << "Room Number,Room Type,Price,Availability" << endl;
        for (const Room& r : rooms) {
            outFile << r.roomNumber << ","
                 << r.roomType << ","
                 << r.pricePerDay << ","
                 << (r.isAvailable ? "true" : "false") << endl;
        }

        cout << endl;
        cout << "***********************" << endl;
        cout << "Room list updated successfully" << endl;
        cout << "***********************" << endl;
        cout << endl;
    }

};



int main(){
    cout << R"(

__        __   _
\ \      / /__| | ___ ___  _ __ ___   ___
 \ \ /\ / / _ \ |/ __/ _ \| '_ ` _ \ / _ \
  \ V  V /  __/ | (_| (_) | | | | | |  __/
   \_/\_/ \___|_|\___\___/|_| |_| |_|\___|

)";
    cout << "Welcome Manager!"<< endl;
    int choice;
    Manager manager;
    do {
        cout << "What we are doing? \n" << "*Guests: " << "\n\t" <<"1. Register Guest" << "\n\t" <<"2. Book Room for Guest" <<
            "\n\t" << "3. View Guests" << "\n" << "*Hotel" << "\n\t" << "4. Add New Rooms" << "\n\t" << "5. View Rooms" << "\n\t" <<"6. Modify Room Info" <<
                "\n\t" << "7. Delete Room from Database" << "\n" << "*Quit" << "\n\t" <<"8. Quit" << "\n\n" << "Choice: ";

        cin >> choice;
        switch (choice) {
            case 1:
                manager.registerGuest();
                break;
            case 2:
                manager.bookRoom();
                break;
            case 3:
                manager.viewGuests();
                break;
            case 4:
                manager.addRoom();
                break;
            case 5:
                manager.viewRooms();
                break;
            case 6:
                manager.modifyRoom();
                break;
            case 7:
                manager.deleteRoom();
                break;
            case 8:
                cout << endl;
                cout << "System shutting down... Hope to see you again!";
                cout << endl;
                break;
            default:
                cout << "Please choose wisely" << endl;
                break;
        }
    }while (choice != 8);
}

// room booking when available rooms 404