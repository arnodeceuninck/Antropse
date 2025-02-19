//
// Created by arno on 4/13/19.
//

//============================================================================
// Name        : AntropseUtils.cpp
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstdio>
#include <string>
#include <cstdlib>

using namespace std;

#include "AntropseUtils.h"

/**
Auxiliary functions for file manipulation.
*/
bool directoryExists(const std::string dirName) {
    struct stat st;
    return stat(dirName.c_str(), &st) == 0;
}

bool fileExists(const std::string fileName) {
    struct stat st;
    if (stat(fileName.c_str(), &st) != 0) return false;
    ifstream f(fileName.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

bool fileIsEmpty(const std::string fileName) {
    struct stat st;
    if (stat(fileName.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
    return st.st_size == 0;
}

bool fileCompare(const std::string leftFileName, const std::string rightFileName) {
    ifstream leftFile, rightFile;
    char leftRead, rightRead;
    bool result;

    // Open the two files.
    leftFile.open(leftFileName.c_str());
    if (!leftFile.is_open()) {
        return false;
    };
    rightFile.open(rightFileName.c_str());
    if (!rightFile.is_open()) {
        leftFile.close();
        return false;
    };

    result = true; // files exist and are open; assume equality unless a counterexamples shows up.
    while (result && leftFile.good() && rightFile.good()) {
        leftFile.get(leftRead);
        rightFile.get(rightRead);
        result = (leftRead == rightRead);
    };
    if (result) {
        // last read was still equal; are we at the end of both files ?
        result = (!leftFile.good()) && (!rightFile.good());
    };

    leftFile.close();
    rightFile.close();
    return result;
}

string toString(int x) {
    int length = snprintf(NULL, 0, "%d", x);
    char *buf = new char[length + 1];
    snprintf(buf, length + 1, "%d", x);
    string str(buf);
    delete[] buf;
    return str;
}

bool checkInt(const std::string nr) {
    return atoi(nr.c_str()) == atof(nr.c_str());
}
