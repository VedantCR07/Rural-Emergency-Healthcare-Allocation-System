#ifndef PATIENT_H
#define PATIENT_H

#include "entity.h"
#include <string>
#include <vector>
using namespace std;

class InvalidSeverityException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: Severity must be between 1 and 5.";
    }
};

class InvalidPatientDataException : public exception {
private:
    string message;
public:
    InvalidPatientDataException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidVillageException : public exception {
public:
    const char* what() const noexcept override {
        return "Error: Village not found in the system. Please enter a valid village.";
    }
};

class Patient : public Entity {
private:
    int age;
    string gender;
    string village;
    string nearestHub;
    int severity;
    string assignedHospitalID;   // set after allocation
    string assignedHospitalName; // set after allocation

    // auto-generatinon of patient IDs
    static int patientCounter;

    // Helper to generate ID like P001, P002...
    static string generateID();

public:
    /
    Patient(const string& id,
            const string& name,
            int age,
            const string& gender,
            const string& village,
            const string& nearestHub,
            int severity,
            const string& assignedHospitalID = "NONE",
            const string& assignedHospitalName = "Not Allocated");

    // Factory method — used for new registration (auto-generates ID)
    static Patient registerNewPatient(const vector<string>& validVillages,
                                      const vector<pair<string,string>>& villagHubMap);

    // fucntion overridding from Entity
    void display() const override;
    string getID() const override;

    int getAge() const;
    string getGender() const;
    string getVillage() const;
    string getNearestHub() const;
    int getSeverity() const;
    string getAssignedHospitalID() const;
    string getAssignedHospitalName() const;

    void setAssignedHospital(const string& hospID, const string& hospName);

    string toFileString() const; //saves files
};

#endif //
