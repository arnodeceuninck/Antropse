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

/**
 * Check whether a directory exists
 * @param dirName The name of the directory
 * @return True when the directory exists
 */
bool directoryExists(const std::string dirName);

/**
 * Check whether a file with the given filename exists
 * @param fileName The name of the file
 * @return True when the file exists
 */
bool fileExists(const std::string fileName);

/**
 * Check whether a file is empty
 * @param fileName The name of the file you want to check
 * @return True when there is no text written in the file
 */
bool fileIsEmpty(const std::string fileName);

/**
 * Compare whether two files have exact the same text
 * @param leftFileName
 * @param rightFileName
 * @return True when the content of both files is identical
 */
bool fileCompare(const std::string leftFileName, const std::string rightFileName);

/**
 * Check whether the given string an integer is
 * @param nr The string that should represent an integer
 * @return True when the string is an integer
 */
bool checkInt(const std::string nr);

/**
 * Convert an int to a string
 * @param i the int you want to convert
 * @return A string containg the text value of the given int
 */
std::string toString(int i);

// Closing of the ``header guard''.
#endif