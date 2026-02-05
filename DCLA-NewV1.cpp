// DCLA-NewV1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DataManager.h"

int main()
{
    DataManager dm;  // Create an instance
    dm.readData("C:\\Apps\\DCLA-NewV1\\datmat.dat");  // Call on the instance
    dm.determineUniqueData();

    return 0;
}

