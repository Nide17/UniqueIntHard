/*
 * hw01.cpp
 *
 *  Created on: Dec 13, 2020
 *      Author: kompalli
 */

#include "UniqueInt.h"
#define BIT_ARRAY_SIZE 102400
#define NUMBER_OF_BIT_ARRAYS 41944
#define MIN_VALUE -2147483648

/**
 * IMPLEMENTATION OF THE BitArray CLASS FROM BitVector.h
 */
// CONSTRUCTING THE ARRAY OF BITS WITH SIZE 102400 BITS
BitArray::BitArray()
{
	this->numberOfBitsNeeded = BIT_ARRAY_SIZE;
	this->bits = new char[BIT_ARRAY_SIZE];
}

// CONSTRUCTING THE ARRAY OF BITS WITH SIZE numberOfBitsNeeded BITS
BitArray::BitArray(int numberOfBitsNeeded)
{
	this->numberOfBitsNeeded = numberOfBitsNeeded;
	this->bits = new char[numberOfBitsNeeded];
}

// SETTING THE BIT VALUE
void BitArray::setBit(int locationOfBit)
{
	// CHECKING IF THE locationOfBit IS VALID (IN RANGE)
	if (locationOfBit < 0 || locationOfBit >= numberOfBitsNeeded)
	{
		char *message = new char[2048];
		sprintf(message, "Invalid location %d", locationOfBit);
		throw std::out_of_range(message);
		delete[] message;
	}

	// IF THE BIT IS NOT SET, THEN SET IT
	if (!(bits[locationOfBit / 8] & (1 << (locationOfBit % 8))))
	{
		bits[locationOfBit / 8] |= (1 << (locationOfBit % 8));
	}
}

// GETTING THE BIT VALUE
bool BitArray::getBit(int locationOfBit)
{
	// CHECKING IF THE locationOfBit IS VALID (IN RANGE)
	if (locationOfBit < 0 || locationOfBit >= numberOfBitsNeeded)
	{
		char *message = new char[2048];
		sprintf(message, "Invalid location %d", locationOfBit);
		throw std::out_of_range(message);
		delete[] message;
	}

// IF THE BIT VALUE IS 1, RETURN TRUE
	if (bits[locationOfBit / 8] & (1 << (locationOfBit % 8)))
	{
		return true;
	}

	// IF THE BIT VALUE IS 0, RETURN FALSE
	else
	{
		return false;
	}
}

// CREATE A NODE FOR THE BINARY SEARCH TREE
struct Node
{
	int value;	 // VALUE OF THE NODE (INTEGER)
	Node *left;	 // POINTER TO THE LEFT SUB-TREE
	Node *right; // POINTER TO THE RIGHT SUB-TREE
};

// FUNCTION TO INSERT A NODE IN THE BINARY SEARCH TREE
void insertNode(Node *&root, int value)
{
	// IF THE ROOT IS NULL, CREATE A NEW NODE
	if (root == nullptr)
	{
		// CREATING A NEW NODE BY DYNAMICALLY ALLOCATING MEMORY FOR IT
		root = new Node{value, nullptr, nullptr};
	}

	// IF THE VALUE IS LESS THAN THE ROOT, INSERT THE NODE IN THE LEFT SUB-TREE
	else if (value < root->value)
	{
		// RECURSIVELY CALLING THE FUNCTION TO INSERT THE NODE IN THE LEFT SUB-TREE
		insertNode(root->left, value);
	}

	// IF THE VALUE IS GREATER THAN THE ROOT, INSERT THE NODE IN THE RIGHT SUB-TREE
	else
	{
		// RECURSIVELY CALLING THE FUNCTION TO INSERT THE NODE IN THE RIGHT SUB-TREE
		insertNode(root->right, value);
	}
}

// FUNCTION TO TRAVERSE THE BINARY SEARCH TREE INORDER & WRITE THE VALUES TO THE OUTPUT FILE
void traverseInorder(Node *root, FILE *outputFileStream)
{
	// CHECKING IF THE ROOT IS NOT NULL
	if (root != nullptr)
	{
		// TRAVERSING THE LEFT SUB-TREE
		traverseInorder(root->left, outputFileStream);

		// WRITING THE VALUES TO THE OUTPUT FILE
		fprintf(outputFileStream, "%d\n", root->value);

		// SHIFTING THE ROOT TO THE RIGHT
		traverseInorder(root->right, outputFileStream);
	}

	// DELETING THE ROOT
	delete root;

	// SETTING THE ROOT TO NULL
	root = nullptr;
}

// FUNCTION TO READ THE NEXT INTEGER FROM THE INPUT FILE
int UniqueInt::readNextItemFromFile(FILE *inputFileStream)
{
	if (!inputFileStream)
	{
		char message[35] = "Cannot open input file for reading";
		throw std::ios_base::failure(message);
	}

	// READ LINE BY LINE & RETURN INTEGER IF THE LINE IS VALID
	int returnInteger = INT_MIN;

	// READ ONE LINE AT A TIME FROM THE INPUT FILE - UNTIL EOF IS REACHED
	while (true)
	{
		char lineBuffer[2047];
		int part1;
		char part2[2047];
		int number;

		// IF EOF IS NOT REACHED, READ ONE LINE AT A TIME FROM THE INPUT FILE
		if (!feof(inputFileStream) && fgets(lineBuffer, 2047, inputFileStream) != NULL)
		{
			// CHECK THAT INTEGER IF IT IS NOT IN INT RANGE USING strtol FUNCTION
			if (strtol(lineBuffer, NULL, 10) < INT_MIN || strtol(lineBuffer, NULL, 10) > INT_MAX)
				continue;

			// IF THE LINE HAS SPACE IN-BETWEEN CONTENTS, SKIP THE LINE
			if (sscanf(lineBuffer, "%d %s", &part1, part2) != 1)
				continue;

			// IF THE LINE CAN BE CONVERTED TO INTEGER, STORE THE INTEGER IN number
			if (sscanf(lineBuffer, "%d", &number) == 1)
				returnInteger = number;

			// IF THE LINE CAN'T BE CONVERTED TO INTEGER, SKIP THE LINE
			else
				continue;
		}

		// BREAK THE IF ABOVE CONDITION IS NOT SATISFIED
		break;
	}

	return returnInteger;
}

// FUNCTION TO PROCESS THE INPUT FILE AND WRITE THE UNIQUE INTEGERS TO THE OUTPUT FILE
int UniqueInt::processFile(char *inputFilePath, char *outputFilePath)
{
	FILE *inFileStream = fopen(inputFilePath, "r");
	if (!inFileStream)
	{
		char message[1024];
		sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
		throw std::ios_base::failure(message);
	}

	FILE *outFileStream = fopen(outputFilePath, "w");
	if (!outFileStream)
	{
		char message[1024];
		sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
		throw std::ios_base::failure(message);
	}
	LogManager::writePrintfToLog(LogManager::Level::Status,
								 "UniqueInt::processFile",
								 "Starting the processing inFile=%s", inputFilePath);
	try
	{
		// CREATING A CONTAINING ARRAY OF 41944 BitArrays USING THE DEFAULT CONSTRUCTOR
		BitArray *bitArrays = new BitArray[NUMBER_OF_BIT_ARRAYS];

		// INITIALIZE EVERY BitArray OBJECT IN THE CONTAINING ARRAY
		for (int i = 0; i < NUMBER_OF_BIT_ARRAYS; i++)
		{
			// CREATING A BitArray OBJECT USING THE PARAMETERIZED CONSTRUCTOR
			bitArrays[i] = BitArray(BIT_ARRAY_SIZE); // 102400 BITS PER BitArray OBJECT
		}

		// CREATING A BINARY SEARCH TREE ROOT NODE
		Node *root = nullptr;

		// READ ONE LINE AT A TIME FROM THE INPUT FILE - UNTIL EOF IS REACHED
		while (true)
		{
			int number = readNextItemFromFile(inFileStream);
			LogManager::writePrintfToLog(LogManager::Level::Status, "UniqueInt::processFile",
										 "number=%d", number);

			// IF EOF IS REACHED, BREAK THE LOOP
			if (feof(inFileStream))
				break;

			// FINDING THE INDEX FOR INSIDE BitArray THAT CONTAINS THE number FROM THE CONTAINING
			int outerBitArrIdx = (number - MIN_VALUE) / BIT_ARRAY_SIZE;

			// FINDING THE INDEX OF THE BIT CORRESPONDING TO THE number IN THE INSIDE BitArray
			int innerBitArrIdx = (number - MIN_VALUE) % BIT_ARRAY_SIZE;

			// IF THE BIT IS NOT SET, SET THE BIT AND ADD THE number TO THE BINARY SEARCH TREE
			if (bitArrays[outerBitArrIdx].getBit(innerBitArrIdx) == 0) {

				// SETTING THE BIT TO 1 IF IT IS NOT ALREADY SET
				bitArrays[outerBitArrIdx].setBit(innerBitArrIdx);

				// INSERTING THE number TO THE BST IF IT IS NOT ALREADY PRESENT
				insertNode(root, number);
			}
		}

		// TRAVERSING INORDER AND PRINTING THE UNIQUE INTEGERS TO THE OUTPUT FILE
		traverseInorder(root, outFileStream);
	}

	catch (std::invalid_argument &e)
	{
		LogManager::writePrintfToLog(LogManager::Level::Status,
									 "UniqueInt::processFile",
									 "invalid_argument exception caught=%s\n", e.what());
		if (strcmp(e.what(), "EoF reached") == 0)
		{
			LogManager::writePrintfToLog(LogManager::Level::Status,
										 "UniqueInt::processFile",
										 "OK to continue");
		}
		else
		{
			return -1;
		}
	}

	fflush(outFileStream);
	fclose(outFileStream);

	fclose(inFileStream);
}
