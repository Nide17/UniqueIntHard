/*
 * hw01.cpp
 *
 *  Created on: Dec 13, 2020
 *      Author: kompalli
 * 		Co-Author: Niyomwungeri Parmenide ISHIMWE
 */

#include "UniqueInt.h"

// CREATE A NODE FOR THE BINARY SEARCH TREE
struct NodeBST
{
	int number;	 // VALUE OF THE NODE (INTEGER)
	NodeBST *leftChild; // POINTER TO THE LEFT SUB-TREE
	NodeBST *rightChild; // POINTER TO THE RIGHT SUB-TREE

	// CONSTRUCTOR TO INITIALIZE THE NODE
	NodeBST(int number, NodeBST *leftChild, NodeBST *rightChild)
	{
		this->number = number;
		this->leftChild = leftChild;
		this->rightChild = rightChild;
	}
};

// FUNCTION TO INSERT A NODE IN THE BINARY SEARCH TREE
void insert(NodeBST **root, int num)
{
	// IF THE ROOT IS NULL, CREATE A NEW NODE
	if (*root == nullptr)
	{
		// CREATING A NEW NODE BY DYNAMICALLY ALLOCATING MEMORY FOR IT
		*root = new NodeBST(num, nullptr, nullptr);
	}

	// IF THE number IS EQUAL TO THE ROOT, DO NOTHING
	else if (num == (*root)->number)
	{
		return;
	}

	// IF THE number IS LESS THAN THE ROOT, INSERT THE NODE IN THE LEFT SUB-TREE
	else if (num < (*root)->number)
	{
		// RECURSIVELY CALLING THE FUNCTION TO INSERT THE NODE IN THE LEFT SUB-TREE
		insert(&(*root)->leftChild, num);
	}

	// IF THE VALUE IS GREATER THAN THE ROOT, INSERT THE NODE IN THE RIGHT SUB-TREE
	else
	{
		// RECURSIVELY CALLING THE FUNCTION TO INSERT THE NODE IN THE RIGHT SUB-TREE
		insert(&(*root)->rightChild, num);
	}
}

// FUNCTION TO TRAVERSE THE BINARY SEARCH TREE INORDER & WRITE THE VALUES TO THE OUTPUT FILE
void traversingInOrder(NodeBST *root, FILE *outputFileStream)
{
	// CHECKING IF THE ROOT IS NOT NULL
	if (root != nullptr)
	{
		// TRAVERSING THE LEFT SUB-TREE
		traversingInOrder(root->leftChild, outputFileStream);

		// WRITING THE NUMBERS TO THE OUTPUT FILE
		fprintf(outputFileStream, "%d\n", root->number);

		// SHIFTING THE ROOT TO THE RIGHT
		traversingInOrder(root->rightChild, outputFileStream);

		// DELETE CHILD NODES
		delete root->leftChild;
		delete root->rightChild;

		// SET THE CHILD NODES TO NULL
		root->leftChild = nullptr;
		root->rightChild = nullptr;
	}

	// SET THE ROOT TO NULL
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
			{
				continue;
			}

			// IF THE LINE HAS SPACE IN-BETWEEN CONTENTS, SKIP THE LINE
			if (sscanf(lineBuffer, "%d %s", &part1, part2) != 1)
			{
				continue;
			}

			// IF THE LINE CAN BE CONVERTED TO INTEGER, STORE THE INTEGER IN number
			if (sscanf(lineBuffer, "%d", &number) == 1)
			{
				returnInteger = number; 
			}

			// IF THE LINE CAN'T BE CONVERTED TO INTEGER, SKIP THE LINE
			else
			{
				continue;
			}
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
		// CREATING A BINARY SEARCH TREE ROOT NODE
		NodeBST *root = nullptr;

		// READ ONE LINE AT A TIME FROM THE INPUT FILE - UNTIL EOF IS REACHED
		while (true)
		{
			int number = readNextItemFromFile(inFileStream);
			LogManager::writePrintfToLog(LogManager::Level::Status, "UniqueInt::processFile",
										 "number=%d", number);

			// IF EOF IS REACHED, BREAK THE LOOP
			if (feof(inFileStream))
				break;

			// INSERT THE INTEGER IN THE BINARY SEARCH TREE AS A NODE
			insert(&root, number);
		}

		// TRAVERSING THE TREE IN ORDER AND WRITING THE OUTPUT FILE
		traversingInOrder(root, outFileStream);
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
