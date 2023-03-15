/*
 * hw01.h
 *
 *  Created on: Dec 13, 2020
 *      Author: kompalli
 */

#ifndef UNIQUEINT_H_
#define UNIQUEINT_H_

#include <string.h>
#include <iostream>
#include <fstream>
#include "util/LogManager.h"
#include <limits.h>
#include <math.h>
#include "util/GetMemUsage.h"
#include <unistd.h>

class UniqueInt{
	/**
	 * This function must take as input a file stream that is open, read the next line
	 * and return an integer if it is valid.
	 *
        a. If there are any lines with no inputs, those lines must be skipped.
        b. If there are any lines that contain a non-integer input, those lines must be skipped.
 	 	 If end of file is reached, it must throw an end of file error
	 */
	static int readNextItemFromFile(FILE* inputFileStream);

	static void gotoNextLineInFile(FILE* inputFileStream){
		char str[2];
		char* fileReadStatus = fgets(str, 2, inputFileStream);
		LogManager::writePrintfToLog(LogManager::Level::Status,
						"UniqueInt::gotoNextLineInFile",
						"str=%s++", str);

		while (! (fileReadStatus == NULL || str[0] == '\n')){
			fileReadStatus = fgets(str, 2, inputFileStream);
			LogManager::writePrintfToLog(LogManager::Level::Status,
							"UniqueInt::gotoNextLineInFile",
							"str=%s//", str);

		}
	};


public:
	/**
	 * This is the entry level function that must read an input file and give the output as needed
	 * for HW01.
	 *
	 * @param inputFilePath Path of the input file.
	 * @param outputFilePath Path of the output file.
	 *
	 * If the input file cannot be read throw an error of type ios_base::failure
	 * If the output file cannot be generated, then throw an error of type ios_base::failure
	 */
	static int processFile(char* inputFilePath, char* outputFilePath);
};

#endif /* UNIQUEINT_H_ */
