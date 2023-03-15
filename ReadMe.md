## UniqueInt, Harder version (v3)

This is a C++ program that reads a list of integers from an input file, removes any duplicates, and writes the resulting list of unique integers to an output file. The program uses a binary search tree data structure to store and remove duplicates.

#### Prerequisites

This program was developed and tested on a Linux operating system. It requires a C++ compiler and the standard C++ libraries to be installed. The program also depends on the following files, which must be present in the same directory as the program:

    hw01.cpp: the main source code file for the program.
    hw01.h: the header file for the program.
    util/LogManager.h: a utility file used for logging.
    util/GetMemUsage.h: a utility file used for measuring memory usage.

### Usage

The program can be compiled using any standard C++ compiler, e.g., g++ hw01.cpp -o hw01.

The program takes two arguments: the path to the input file and the path to the output file. These should be passed to the processFile function of the UniqueInt class as character pointers. If the input file cannot be read or the output file cannot be generated, the program will throw an error of type ios_base::failure.

##### Example usage:

c++

#include "UniqueInt.h"
#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        UniqueInt::processFile(argv[1], argv[2]);
    }
    catch (const std::ios_base::failure& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

### Class structure

The UniqueInt class has the following static methods:

###### int processFile(char* inputFilePath, char* outputFilePath)

This is the entry level function that reads the input file and generates the output file. It takes two arguments: the path to the input file and the path to the output file. It returns an integer representing the number of unique integers found in the input file.

###### static int readNextItemFromFile(FILE* inputFileStream)

This function reads the next line from the input file stream and returns an integer if it is valid. It skips any lines with no inputs or non-integer inputs. If the end of file is reached, it throws an end-of-file error.

###### static void gotoNextLineInFile(FILE* inputFileStream)

This function skips to the next line in the input file stream.

#### Data structure

The program uses a binary search tree data structure to store and remove duplicates from the list of integers. The NodeBST struct represents a node in the binary search tree. The insert function inserts a new node into the tree. The traversingInOrder function traverses the tree in-order and writes the unique integers to the output file. The child nodes of the root node are deleted and set to nullptr after the traversal is complete.

#### Authors


    Suryaprakash Kompalli
    Niyomwungeri Parmenide Ishimwe
