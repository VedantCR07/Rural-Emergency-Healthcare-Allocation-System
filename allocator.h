#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "hospital.h"
#include <string>
#include <stdexcept>
using namespace std;

// 🔷 Abstract Base Class (Abstraction)
class AllocationStrategy
{
public:
    // Core allocation function
    virtual int findBestHospital(Hospital h[], int size,
                                 int severity,
                                 string patientVillage) = 0;

    virtual ~AllocationStrategy() {}
};


class SmartAllocator : public AllocationStrategy
{
private:

    // 🔹 Distance mapping (village → hospital location)
    float getDistance(const string &village,
                      const string &hospitalLocation) const;

    // 🔹 Scoring function
    float calculateScore(const Hospital &h,
                         int severity) const;

public:

  
    int findBestHospital(Hospital h[], int size,
                         int severity,
                         string patientVillage);
};

#endif
