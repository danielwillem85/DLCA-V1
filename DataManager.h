#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime> 
#include <random>
#include <unordered_set>

#include <sys/types.h>
#include <sys/stat.h>

//struct stat info;

using namespace std;

class DataManager {

private:
    // Data members 
    mt19937 mt;

public:
    DataManager();

    std::uniform_real_distribution<> dist;

    vector<vector<int>> datmat;
    vector<vector<int>> uniqueMat;
    vector<vector<int>> correctMat;
    vector<vector<int>> connectedMat;

    vector<long double> weights;
    vector<long double> uniqueWeights;

    int J = 0;
    int N = 0;

    int uniqueN = 0;

    vector<double> cprob;
    vector<vector<vector<double>>> condp;

    // Member functions() 
    void printInfo();

    void setDataSize(int j, int n);

    void initializeRandomGenerator();

    void generateRandomData();

    void readData(string dataStr);

    void setStandardWeights();

    void determineUniqueData();

    void correctData() {
        // !
        // Ensure that data values range from 0 ... to max value (as they are used as array indices
        // Current data set has 1's and 2's. In general this is unknown so must be handled with a function
        // that checks whether the values are in the desired range.
        //cout << "datmat:" << endl;

        for (int j = 0; j < J; j++)
        {
            int minimum = 9999;

            for (int i = 0; i < N; i++)
            {
                if (datmat[i][j] != 99) {
                    if (datmat[i][j] < minimum) minimum = datmat[i][j];
                }
            }

            if (minimum < 0) {
                for (int i = 0; i < N; i++)
                {
                    if (datmat[i][j] != 99) {
                        datmat[i][j] = datmat[i][j] + abs(minimum);
                    }
                }
            }
            else if (minimum > 0) {
                for (int i = 0; i < N; i++)
                {
                    if (datmat[i][j] != 99) {
                        datmat[i][j] = datmat[i][j] - minimum;
                    }
                }
            }
        }
    }

    void correctUniqueData();

    // Ensure that data values run consecutively from 0 to 'X'.
    // So if a column contains values 0, 1, 4, 7, it will be stored as 0, 1, 2, 3, so that
    // data values can be used as indices to access data in certain datastructures
    void setOriginalCorrectData();

    // Ensure that data values run consecutively from 0 to 'X'.
    // So if a column contains values 0, 1, 4, 7, it will be stored as 0, 1, 2, 3, so that
    // data values can be used as indices to access data in certain datastructures
    void setCorrectData();

    void generateSyntheticData();
};