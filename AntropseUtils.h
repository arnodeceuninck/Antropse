//============================================================================
// Name        : AntropseUtils.h
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

// Below are two lines serving as a so-called ``header guard''.
// Prevents from loading the same .h file multiple times
#ifndef AntropseUtils_H
#define AntropseUtils_H


#include <iostream>

bool directoryExists(const std::string dirName);

bool fileExists(const std::string fileName);

bool fileIsEmpty(const std::string fileName);

bool fileCompare(const std::string leftFileName, const std::string rightFileName);

std::string toString(int i);

// Closing of the ``header guard''.
#endif