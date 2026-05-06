#include "patient.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>
using namespace std;

// ============================================================
//  Static counter initialisation
// ============================================================

int Patient::patientCounter = 0;

// ============================================================
//  Helper — Auto-generate Patient ID (P001, P002 ...)
// ============================================================

string Patient::generateID() {
    patientCounter++;
    ostringstream oss;
    oss << "P" << setw(3) << setfill('0') << patientCounter;
    return oss.str();
}

// ============================================================
//  Constructor
// ============================================================

Patient::Patient(const string& id,
                 const string& name,
                 int age,
                 const string& gender,
                 const string& village,
                 const string& nearestHub,
                 int severity,
                 const string& assignedHospitalID,
                 const string& assignedHospitalName)
    : Entity(id, name),
      age(age),
      gender(gender),
      village(village),
      nearestHub(nearestHub),
      severity(severity),
      assignedHospitalID(assignedHospitalID),
      assignedHospitalName(assignedHospitalName)
{
    // Validate severity
    if (severity < 1 || severity > 5) {
        throw InvalidSeverityException();
    }
    // Validate name
    if (name.empty()) {
        throw InvalidPatientDataException("Error: Patient name cannot be empty.");
    }
    // Validate age
    if (age <= 0 || age > 120) {
        throw InvalidPatientDataException("Error: Patient age must be between 1 and 120.");
    }
    // Validate gender
    if (gender.empty()) {
        throw InvalidPatientDataException("Error: Gender cannot be empty.");
    }
}

// ============================================================
//  Factory Method — Interactive Registration
// ============================================================

Patient Patient::registerNewPatient(const vector<string>& validVillages,
                                    const vector<pair<string,string>>& villageHubMap) {
    string name, gender, village, nearestHub;
    int age, severity;

    // --- Name ---
    while (true) {
        cout << "  Enter Name       : ";
        getline(cin, name);
        if (!name.empty()) break;
        cout << "  [!] Name cannot be empty. Try again.\n";
    }

    // --- Age ---
    while (true) {
        cout << "  Enter Age        : ";
        if (cin >> age && age > 0 && age <= 120) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Invalid age. Enter a value between 1 and 120.\n";
    }

    // --- Gender ---
    while (true) {
        cout << "  Enter Gender (M/F/Other): ";
        getline(cin, gender);
        if (!gender.empty()) break;
        cout << "  [!] Gender cannot be empty. Try again.\n";
    }

    // --- Village with validation ---
    while (true) {
        cout << "  Enter Village    : ";
        getline(cin, village);

        // Case-insensitive search
        auto it = find_if(validVillages.begin(), validVillages.end(),
            [&](const string& v) {
                string a = v, b = village;
                transform(a.begin(), a.end(), a.begin(), ::tolower);
                transform(b.begin(), b.end(), b.begin(), ::tolower);
                return a == b;
            });

        if (it != validVillages.end()) {
            village = *it; // use correctly cased version from dataset

            // Find nearest hub for this village
            for (const auto& pair : villageHubMap) {
                string a = pair.first, b = village;
                transform(a.begin(), a.end(), a.begin(), ::tolower);
                transform(b.begin(), b.end(), b.begin(), ::tolower);
                if (a == b) {
                    nearestHub = pair.second;
                    break;
                }
            }
            break;
        }

        cout << "  [!] Village not found in system.\n";
        cout << "  Available villages:\n  ";
        for (int i = 0; i < (int)validVillages.size(); i++) {
            cout << validVillages[i];
            if (i < (int)validVillages.size() - 1) cout << ", ";
            if ((i + 1) % 5 == 0) cout << "\n  ";
        }
        cout << "\n  Try again.\n";
    }

    // --- Severity ---
    while (true) {
        cout << "  Enter Severity   : \n";
        cout << "    1 - Very Low (Minor)\n";
        cout << "    2 - Low\n";
        cout << "    3 - Moderate\n";
        cout << "    4 - High (Emergency)\n";
        cout << "    5 - Critical (Life-threatening)\n";
        cout << "  Choice: ";
        if (cin >> severity && severity >= 1 && severity <= 5) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Severity must be between 1 and 5.\n";
    }

    string newID = generateID();
    return Patient(newID, name, age, gender, village, nearestHub, severity);
}

// ============================================================
//  Overridden from Entity
// ============================================================

string Patient::getID() const {
    return id;
}

void Patient::display() const {
    string severityLabel;
    switch(severity) {
        case 1: severityLabel = "Very Low (Minor)";        break;
        case 2: severityLabel = "Low";                     break;
        case 3: severityLabel = "Moderate";                break;
        case 4: severityLabel = "High (Emergency)";        break;
        case 5: severityLabel = "Critical (Life-threatening)"; break;
        default: severityLabel = "Unknown";
    }

    cout << "\n  +-------------------------------------------------+\n";
    cout << "  | Patient ID    : " << id << "\n";
    cout << "  | Name          : " << name << "\n";
    cout << "  | Age           : " << age << "\n";
    cout << "  | Gender        : " << gender << "\n";
    cout << "  | Village       : " << village << "\n";
    cout << "  | Nearest Hub   : " << nearestHub << "\n";
    cout << "  | Severity      : " << severity << " — " << severityLabel << "\n";
    cout << "  | Assigned To   : " << assignedHospitalName << " (" << assignedHospitalID << ")\n";
    cout << "  +-------------------------------------------------+\n";
}

// ============================================================
//  Getters
// ============================================================

int Patient::getAge() const           { return age; }
string Patient::getGender() const     { return gender; }
string Patient::getVillage() const    { return village; }
string Patient::getNearestHub() const { return nearestHub; }
int Patient::getSeverity() const      { return severity; }

string Patient::getAssignedHospitalID() const {
    return assignedHospitalID;
}

string Patient::getAssignedHospitalName() const {
    return assignedHospitalName;
}

// ============================================================
//  Setter — called by Allocator
// ============================================================

void Patient::setAssignedHospital(const string& hospID, const string& hospName) {
    assignedHospitalID   = hospID;
    assignedHospitalName = hospName;
}

// ============================================================
//  File Serialisation
//  Format: ID,Name,Age,Gender,Village,Hub,Severity,HospID,HospName
// ============================================================

string Patient::toFileString() const {
    return id + "," + name + "," + to_string(age) + "," +
           gender + "," + village + "," + nearestHub + "," +
           to_string(severity) + "," + assignedHospitalID + "," +
           assignedHospitalName;
}
