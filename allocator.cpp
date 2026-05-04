#include "allocator.h"
#include <iostream>

// 🔹 Distance Mapping (Village → Hospital Location)
float SmartAllocator::getDistance(const string &village,
                                  const string &hospitalLocation) const
{
    // You can expand this mapping as needed

    if(village == hospitalLocation)
        return 0;

    // Waknaghat region mapping
    if(village == "Waknaghat")
    {
        if(hospitalLocation == "Kandaghat") return 6;
        if(hospitalLocation == "Shoghi") return 8;
        if(hospitalLocation == "Solan") return 22;
        if(hospitalLocation == "Shimla") return 25;
    }

    if(village == "Barog")
    {
        if(hospitalLocation == "Kandaghat") return 10;
        if(hospitalLocation == "Solan") return 8;
        if(hospitalLocation == "Shimla") return 30;
    }

    if(village == "Kaithlighat")
    {
        if(hospitalLocation == "Kandaghat") return 5;
        if(hospitalLocation == "Solan") return 20;
        if(hospitalLocation == "Shimla") return 18;
    }

    // Default (far distance)
    return 50;
}

// 🔹 Scoring Function
float SmartAllocator::calculateScore(const Hospital &h,
                                     int severity) const
{
    int beds = h.getEffectiveBeds();

    float score = (severity * 10)
                + (100 - h.getDistance())
                + (beds * 2);

    return score;
}

// 🔹 Main Allocation Logic
int SmartAllocator::findBestHospital(Hospital h[], int size,
                                     int severity,
                                     string patientVillage)
{
    // 🔴 Exception 1: Invalid severity
    if(severity < 1 || severity > 5)
        throw invalid_argument("Severity must be between 1 and 5.");

    // 🔴 Exception 2: Empty dataset
    if(size == 0)
        throw runtime_error("Hospital list is empty.");

    int bestIndex = -1;
    float bestScore = -1;

    for(int i = 0; i < size; i++)
    {
        // 🔹 Set distance dynamically
        float dist = getDistance(patientVillage, h[i].getLocation());
        h[i].setDistance(dist);

        int beds = h[i].getEffectiveBeds();

        // 🔹 Step 1: Hard Constraints
        if(beds <= 0)
            continue;

        if(severity >= 4 && !h[i].isEmergencyAvailable())
            continue;

        // 🔹 Step 2: Calculate Score
        float score = calculateScore(h[i], severity);

        // 🔹 Step 3: Select Best
        if(score > bestScore)
        {
            bestScore = score;
            bestIndex = i;
        }
    }

    // 🔴 Exception 3: No valid hospital found
    if(bestIndex == -1)
        throw runtime_error("No suitable hospital found.");

    return bestIndex;
}
