#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <limits>
using namespace std;

int getSafeInt() {
    int x;
    while (true) {
        if (cin >> x) return x;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number: ";
    }
}

string hashPassword(const string &pass) {
    unsigned int hash = 0;
    for (char c : pass) hash = hash * 101 + c;
    return to_string(hash);
}

bool loginSystem() {
    string user, pass, fileUser, fileHash;

    ifstream f("users.txt");
    if (!f) {
        ofstream create("users.txt");
        create << "admin|" << hashPassword("1234") << "\n";
        f.open("users.txt");
        cout << "Default admin created (username: admin, password: 1234)\n";
    }

    cout << "\n---- Login Required ----\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    string line;
    while (getline(f, line)) {
        int pos = line.find("|");
        fileUser = line.substr(0, pos);
        fileHash = line.substr(pos + 1);

        if (fileUser == user && fileHash == hashPassword(pass))
            return true;
    }
    return false;
}

class Person {
protected:
    string name;
    int age;
public:
    Person(string n="", int a=0) : name(n), age(a) {}
    virtual void input() = 0;
    virtual void display() = 0;
};

class Patient : public Person {
    int id;
    string disease;
    string mobile;   // NEW FIELD

public:
    Patient(string n="", int a=0, string d="", string m="", int i=0)
        : Person(n, a), disease(d), mobile(m), id(i) {}

    void input() override {
        cout << "\nEnter Patient ID: ";
        id = getSafeInt();
        cin.ignore();

        cout << "Name: "; 
        getline(cin, name);

        cout << "Age: ";
        age = getSafeInt();
        cin.ignore();

        cout << "Disease: ";
        getline(cin, disease);

        cout << "Mobile Number: ";
        getline(cin, mobile);
    }

    void display() override {
        cout << left << setw(10) << id
             << setw(20) << name
             << setw(10) << age
             << setw(20) << disease
             << setw(15) << mobile << endl;
    }

    int getID() { return id; }

    string serialize() {
        return to_string(id) + "|" + name + "|" + to_string(age) + "|" + disease + "|" + mobile + "\n";
    }
};

class Doctor : public Person {
    int docID;
    string speciality;

public:
    Doctor(int id=0, string n="", int a=0, string s="")
        : Person(n,a), docID(id), speciality(s) {}

    void input() override {
        cout << "\nEnter Doctor ID: ";
        docID = getSafeInt();
        cin.ignore();

        cout << "Name: "; 
        getline(cin, name);

        cout << "Age: ";
        age = getSafeInt();
        cin.ignore();

        cout << "Speciality: ";
        getline(cin, speciality);
    }

    void display() override {
        cout << left << setw(10) << docID
             << setw(20) << name
             << setw(20) << speciality << endl;
    }

    string serialize() {
        return to_string(docID) + "|" + name + "|" + to_string(age) + "|" + speciality + "\n";
    }
};

class Appointment {
    int patientID, doctorID;
    string date;

public:
    void book() {
        cout << "\nEnter Patient ID: ";
        patientID = getSafeInt();

        cout << "Enter Doctor ID: ";
        doctorID = getSafeInt();
        cin.ignore();

        cout << "Date (DD-MM-YYYY): ";
        getline(cin, date);
    }

    string serialize() {
        return to_string(patientID) + "|" + to_string(doctorID) + "|" + date + "\n";
    }
};

class FileManager {
public:

    static void savePatient(Patient p) {
        ofstream f("patients.txt", ios::app);
        f << p.serialize();
    }

    static void saveDoctor(Doctor d) {
        ofstream f("doctors.txt", ios::app);
        f << d.serialize();
    }

    static void saveAppointment(Appointment a) {
        ofstream f("appointments.txt", ios::app);
        f << a.serialize();
    }

    static void viewPatients() {
        ifstream f("patients.txt");
        if (!f) { cout << "No patient records.\n"; return; }

        cout << left << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(10) << "Age"
             << setw(20) << "Disease"
             << setw(15) << "Mobile" << "\n";

        cout << string(75, '-') << endl;

        string line, id, name, age, disease, mobile;

        while (getline(f, line)) {
            int p1 = line.find("|");
            int p2 = line.find("|", p1+1);
            int p3 = line.find("|", p2+1);
            int p4 = line.find("|", p3+1);

            id = line.substr(0, p1);
            name = line.substr(p1+1, p2-p1-1);
            age = line.substr(p2+1, p3-p2-1);
            disease = line.substr(p3+1, p4-p3-1);
            mobile = line.substr(p4+1);

            cout << left << setw(10) << id
                 << setw(20) << name
                 << setw(10) << age
                 << setw(20) << disease
                 << setw(15) << mobile << endl;
        }
    }

    static void viewDoctors() {
        ifstream f("doctors.txt");
        if (!f) { cout << "No doctor records.\n"; return; }

        cout << left << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(20) << "Speciality" << "\n";

        cout << string(55, '-') << endl;

        string line, id, name, age, spec;

        while (getline(f, line)) {
            int p1 = line.find("|");
            int p2 = line.find("|", p1+1);
            int p3 = line.find("|", p2+1);

            id = line.substr(0, p1);
            name = line.substr(p1+1, p2-p1-1);
            age = line.substr(p2+1, p3-p2-1);
            spec = line.substr(p3+1);

            cout << left << setw(10) << id
                 << setw(20) << name
                 << setw(20) << spec << endl;
        }
    }

    static void updatePatient() {
        int id;
        cout << "Enter Patient ID to update: ";
        id = getSafeInt();
        cin.ignore();

        ifstream f("patients.txt");
        ofstream temp("temp.txt");
        string line;
        bool found = false;

        while (getline(f, line)) {

            int p1 = line.find("|");
            int p2 = line.find("|", p1+1);
            int p3 = line.find("|", p2+1);
            int p4 = line.find("|", p3+1);

            int currentID = stoi(line.substr(0, p1));

            if (currentID == id) {
                found = true;

                string name = line.substr(p1+1, p2-p1-1);
                string age  = line.substr(p2+1, p3-p2-1);
                string disease = line.substr(p3+1, p4-p3-1);
                string mobile = line.substr(p4+1);

                cout << "\nWhat do you want to update?\n";
                cout << "1. Name\n2. Age\n3. Disease\n4. Mobile Number\nChoice: ";
                int ch = getSafeInt();
                cin.ignore();

                if (ch == 1) {
                    cout << "Enter new name: ";
                    getline(cin, name);
                }
                else if (ch == 2) {
                    cout << "Enter new age: ";
                    int newAge = getSafeInt();
                    age = to_string(newAge);
                    cin.ignore();
                }
                else if (ch == 3) {
                    cout << "Enter new disease: ";
                    getline(cin, disease);
                }
                else if (ch == 4) {
                    cout << "Enter new mobile: ";
                    getline(cin, mobile);
                }

                temp << id << "|" << name << "|" << age << "|" << disease << "|" << mobile << "\n";
            }
            else {
                temp << line << "\n";
            }
        }

        f.close();
        temp.close();
        remove("patients.txt");
        rename("temp.txt", "patients.txt");

        if (found) cout << "Patient Updated Successfully!\n";
        else cout << "Patient ID Not Found.\n";
    }
};

int main() {
    try {
        if (!loginSystem()) {
            cout << "\nAccess Denied!\n";
            return 0;
        }

        int choice;
        while (true) {
            cout << "\n---- Hospital Management ----\n";
            cout << "1. Add Patient\n";
            cout << "2. View Patients\n";
            cout << "3. Update Patient\n";
            cout << "4. Add Doctor\n";
            cout << "5. View Doctors\n";
            cout << "6. Book Appointment\n";
            cout << "7. Exit\n";
            cout << "Choice: ";
            choice = getSafeInt();

            if (choice == 1) {
                Patient p;
                p.input();
                FileManager::savePatient(p);
            }
            else if (choice == 2) FileManager::viewPatients();
            else if (choice == 3) FileManager::updatePatient();
            else if (choice == 4) {
                Doctor d;
                d.input();
                FileManager::saveDoctor(d);
            }
            else if (choice == 5) FileManager::viewDoctors();
            else if (choice == 6) {
                Appointment a;
                a.book();
                FileManager::saveAppointment(a);
            }
            else if (choice == 7) break;
            else cout << "Invalid choice.\n";
        }
    }
    catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }
}
