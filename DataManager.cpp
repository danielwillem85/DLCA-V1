#include "DataManager.h"  // Include the header file
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

DataManager::DataManager() {
    // Initialization if necessary
}

void DataManager::readData(std::string dataStr) {

    datmat.resize(N);
    for (int i = 0; i < N; i++) {
        datmat[i].resize(J);
    }

    //for (int i = 0; i < 10; i++) {
    //    for (int j = 0; j < J; j++) {
    //        std::cout << datmat[i][j] << " ";
    //    }
    //    std::cout << std::endl;
    //}

    ifstream dataStream(dataStr);

    for (int i = 0; i < N; i++)
    {
        string line = "";
        std::getline(dataStream, line);
        std::stringstream lineStream(line);

        for (int j = 0; j < J; j++)
        {
            string val = "";
            std::getline(lineStream, val, ',');
            datmat[i][j] = stoi(val);
        }
    }

    correctData();
    setStandardWeights();
}

void DataManager::generateRandomData() {

    datmat.resize(N);
    for (int i = 0; i < N; i++) {
        datmat[i].resize(J);
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < J; j++)
        {
            datmat[i][j] = (int)std::round(dist(mt));
        }
    }
}

void DataManager::printInfo() {
    cout << "nRows:" << N << endl;
    cout << "nVariables:" << J << endl;

    if (N >= 10)
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < J; j++)
            {
                cout << datmat[i][j] << " ";
            }
            cout << endl;
        }
    }
}

void DataManager::setDataSize(int j, int n) {
    J = j;
    N = n;
}

void DataManager::initializeRandomGenerator() {
    random_device rd;
    mt = mt19937(rd());
    dist = std::uniform_real_distribution<>(0.0, 1.0);
}

void DataManager::setStandardWeights() {
    weights.resize(N);
    for (int i = 0; i < N; i++) {
        weights[i] = 1.0L;
    }
}

void DataManager::determineUniqueData() {
    // Create a data structure with only the unique rows
    vector<string> uniquePatterns;
    vector<vector<int>> tempMat;
    vector<int> tempUniqueWeights;

    string pattern = "";
    vector<int> line(J);
    vector<vector<int>> patterns;
    vector<int> counts;

    bool present = false;
    bool match = true;

    for (int i = 0; i < N; i++) {
        present = false;
        int index = 0;

        for (int k = 0; k < patterns.size(); k++) {
            match = true;
            for (int h = 0; h < patterns[k].size(); h++) {
                if (datmat[i][h] != patterns[k][h]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                index = k;
                present = true;
                break;
            }
        }

        if (present) {
            counts[index] = counts[index] + 1;
        }
        else {
            vector<int> row;
            for (int k = 0; k < J; k++) {
                row.push_back(datmat[i][k]);
            }
            patterns.push_back(row);
            counts.push_back(1);
        }
    }

    uniqueN = patterns.size();
    uniqueMat.resize(uniqueN);
    for (int i = 0; i < uniqueN; i++) {
        uniqueMat[i].resize(J);
    }

    // Set unique weights and store data
    uniqueWeights.resize(uniqueN);

    for (int i = 0; i < uniqueN; i++) {
        uniqueWeights[i] = counts[i];

        for (int j = 0; j < J; j++) {
            uniqueMat[i][j] = patterns[i][j];
        }
    }

    correctUniqueData();
}

void DataManager::correctUniqueData() {
    // !
    // Ensure that data values range from 0 ... to max value (as they are used as array indices
    // Current data set has 1's and 2's. In general this is unknown so must be handled with a function
    // that checks whether the values are in the desired range.
    //cout << "datmat:" << endl;

    for (int j = 0; j < J; j++)
    {
        int minimum = 9999;

        for (int i = 0; i < uniqueN; i++)
        {
            if (uniqueMat[i][j] != 99) {
                if (uniqueMat[i][j] < minimum) minimum = uniqueMat[i][j];
            }
        }

        if (minimum < 0) {
            for (int i = 0; i < uniqueN; i++)
            {
                if (uniqueMat[i][j] != 99) {
                    uniqueMat[i][j] = uniqueMat[i][j] + abs(minimum);
                }
            }
        }
        else if (minimum > 0) {
            for (int i = 0; i < uniqueN; i++)
            {
                if (uniqueMat[i][j] != 99) {
                    uniqueMat[i][j] = uniqueMat[i][j] - minimum;
                }
            }
        }
    }
}

void DataManager::setOriginalCorrectData() {

    connectedMat.resize(N);
    for (int i = 0; i < N; i++) {
        connectedMat[i].resize(J);
    }

    vector<int> values;

    // Per variable, determine all present values
    for (int j = 0; j < J; j++) {

        values.clear();

        // Check if current value is present in values, if not add it
        for (int i = 0; i < N; i++) {
            bool present = false;
            for (int k = 0; k < values.size(); k++) {
                if (values[k] == datmat[i][j]) {
                    present = true;
                    break;
                }
            }
            if (present == false) {
                values.push_back(datmat[i][j]);
            }
        }

        std::sort(values.begin(), values.begin() + values.size());

        // 0, 1, 3, 7 -> 0, 1, 2, 3
        for (int i = 0; i < uniqueN; i++) {
            for (int k = 0; k < values.size(); k++) {
                if (datmat[i][j] == values[k]) {
                    datmat[i][j] = k;
                }
            }
        }
    }
}

void DataManager::setCorrectData() {

    correctMat.resize(uniqueN);
    for (int i = 0; i < uniqueN; i++) {
        correctMat[i].resize(J);
    }

    vector<int> values;

    // Per variable, determine all present values
    for (int j = 0; j < J; j++) {

        values.clear();

        // Check if current value is present in values, if not add it
        for (int i = 0; i < uniqueN; i++) {
            bool present = false;
            for (int k = 0; k < values.size(); k++) {
                if (values[k] == correctMat[i][j]) {
                    present = true;
                    break;
                }
            }
            if (present == false) {
                values.push_back(correctMat[i][j]);
            }
        }

        std::sort(values.begin(), values.begin() + values.size());

        // 0, 1, 3, 7 -> 0, 1, 2, 3
        for (int i = 0; i < uniqueN; i++) {
            for (int k = 0; k < values.size(); k++) {
                if (correctMat[i][j] == values[k]) {
                    correctMat[i][j] = k;
                }
            }
        }
    }
}

void DataManager::generateSyntheticData() {

    setDataSize(5, 10000);
    cprob = { 0.1, 0.3, 0.6 };
    condp = {
                {   {0.1, 0.9},
                    {0.3, 0.2, 0.5},
                    {0.2, 0.1, 0.7},
                    {0.3, 0.2, 0.2, 0.3},
                    {0.7, 0.3} },
                {   {0.3, 0.7},
                    {0.5, 0.1, 0.4},
                    {0.4, 0.3, 0.3},
                    {0.1, 0.5, 0.2, 0.2},
                    {0.5, 0.5} },
                {   {0.9, 0.1},
                    {0.5, 0.2, 0.3},
                    {0.1, 0.7, 0.2},
                    {0.6, 0.2, 0.1, 0.1},
                    {0.2, 0.8} }
    };

    datmat.resize(N);
    for (int i = 0; i < N; i++) {
        datmat[i].resize(J);
    }

    // For each respondent
    for (int i = 0; i < N; i++)
    {
        double draw = dist(mt);
        double cumu = 0;
        int classNr = 0;

        // Draw class membership
        for (int k = 0; k < cprob.size(); k++) {
            cumu = cumu + cprob[k];
            if (draw <= cumu) {
                classNr = k;
                break;
            }
        }

        // Draw variable responses
        for (int j = 0; j < J; j++)
        {
            draw = dist(mt);
            cumu = 0;

            for (int h = 0; h < condp[classNr][j].size(); h++) {
                cumu += condp[classNr][j][h];
                if (draw <= cumu) {
                    datmat[i][j] = h;
                    break;
                }
            }
        }
    }

}