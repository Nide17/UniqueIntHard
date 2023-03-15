/*
 * hw01.cpp
 *
 *  Created on: Dec 13, 2020
 *      Author: kompalli
 */

#include "UniqueInt.h"

struct bit_array_struct
{
	int needed_bits_number;
	char *bit_values;

	// set the bit value at the given bit location
	void set_bit(int bit_loc, bool current_bit)
	{
		// check if the bit location is valid (in range)
		if (bit_loc < 0 || bit_loc >= needed_bits_number)
		{
			return;
		}

		// if the bit is not set, then set it
		if (current_bit && !(bit_values[bit_loc / 8] & (1 << (bit_loc % 8))))
		{
			bit_values[bit_loc / 8] |= (1 << (bit_loc % 8));
		}
	}

	// get the bit value at the given bit location
	int get_bit(int bit_loc)
	{
		// check if the bit location is valid (in range)
		if (bit_loc < 0 || bit_loc >= needed_bits_number)
		{
			// return the bit value at the given bit location
			return 0;
		}

		// if the bit value is 1, return 1
		else if (bit_values[bit_loc / 8] & (1 << (bit_loc % 8)))
		{
			return 1;
		}

		// if the bit value is 0, return 0
		else
		{
			return 0;
		}
	}
};

// a node for the binary search tree
struct Node
{
	int value;	 // value of the root node
	Node *left;	 // left node
	Node *right; // right node
};

// insertion of a node in the binary search tree
void node_insertion(Node *&root, int value)
{
	// if the root is null, create a new node
	if (root == nullptr)
	{
		// create a new node
		root = new Node{value, nullptr, nullptr};
	}

	// if the value is null, create a new node
	else if (value < root->value)
	{
		node_insertion(root->left, value);
	}

	// else, insert the new node to the right
	else
	{
		// insert the new node to the right
		node_insertion(root->right, value);
	}
}

// in-order traversal of the binary search tree
void in_order_traversal(Node *root, FILE *outputFileStream)
{
	// if the root is not null
	if (root != nullptr)
	{
		// shifting the root to the left
		in_order_traversal(root->left, outputFileStream);

		// writing the value of the root to the output file
		fprintf(outputFileStream, "%d\n", root->value);

		// shifting the root to the right
		in_order_traversal(root->right, outputFileStream);
	}

	// if the root is null, return
	else
	{
		return;
	}

	// delete the root
	delete root;

	// set the root to null
	root = nullptr;
}

/*
Program that Read from a file that has one integer on each line,
output a result file which contains a list of the unique integers in this file sorted in increasing order
*/
int UniqueInt::readNextItemFromFile(FILE *inputFileStream)
{
	if (!inputFileStream)
	{
		char message[35] = "Cannot open input file for reading";
		throw std::ios_base::failure(message);
	}

	int returnInteger = 1024;

	// Write code here to read the input file.
	int nextItem;

	// get lines of integers until the end of file
	while (true)
	{
		if (feof(inputFileStream))
		{
			break;
		}

		// character array to store one line of input
		char string_integer[2047];
		int integer;

		// parsing one line of input
		if (fgets(string_integer, 2047, inputFileStream))
		{
			int floating = false;
			int i = 0;
			int x;
			char y;

			// checking and skipping lines with floating numbers
			while (i < strlen(string_integer))
			{
				if (string_integer[i] == '.')
				{
					floating = true;
				}
				i++;
			}

			if (floating == true)
			{
				continue;
			}

			// skipping lines with in-between character spaces
			if (sscanf(string_integer, "%d %c", &x, &y) > 1)
			{
				continue;
			}

			// converting the parsed line into integer
			try
			{
				returnInteger = std::stoi(string_integer);
			}
			catch (const std::invalid_argument &e)
			{
				continue;
			}
			catch (const std::out_of_range &e)
			{
				continue;
			}
		}

		return returnInteger;
	}
}

// processing the input file and writing the output file
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
		// initializing a 2D array of 41944 BitArrays
		bit_array_struct two_d_arr[41944];

		// initializing each BitArray to be inside the 2D array
		int index = 0;
		while (index < 41944)
		{
			// initializing each BitArray with 102400 bits
			two_d_arr[index] = {102400, new char[102400]};
			index++;
		}

		// creating a root for the tree
		Node *root = nullptr;

		// reading the input file line by line
		while (true)
		{
			int number = readNextItemFromFile(inFileStream);
			LogManager::writePrintfToLog(LogManager::Level::Status, "UniqueInt::processFile",
										 "number=%d", number);

			// end of file - break the loop
			if (feof(inFileStream))
				break;

			// finding the index of the 2D array
			int index_of_2d_arr = (number + 2147483648) / 102400;
			int index_1d_arr = (number + 2147483648) % 102400;

			// if the bit is 0, set the bit to 1 and insert the number to the tree
			if (two_d_arr[index_of_2d_arr].get_bit(index_1d_arr) == 0)
			{
				// setting the bit to 1
				two_d_arr[index_of_2d_arr].set_bit(index_1d_arr, 1);

				// inserting the number to the tree
				node_insertion(root, number);
			}
		}

		// traversing the tree and inserting the number into the output file
		in_order_traversal(root, outFileStream);
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
