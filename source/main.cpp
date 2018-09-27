/*
	Referee: A Compiler from 'Shootout', a BASIC-like language to Python 2.7.
	Takes an input text file written in Shootout, and produces an output .py file.

	Referee works in three steps:
	Lexical Analysis -	Analysis is performed on the code, converting the it into tokens defining its basic form. The tokens are defined in translateFile.txt
						The tokens are stored in a file named tokenFile. Constants and varialbes are stored in a lookup table, constVarTable.txt.
	
	Syntax Check -		Each line of the tokenFile is checked against the allowable syntax of Shootout, defined in BNF in BNFfile.txt.
						Errors are reported, if none found then the stage is completed.
						
	Code Generation -	The syntactically correct tokens are used to generate Python code, by referencing the tokens with codeTable.txt.
						The Python code is saved to a file specified by the input arguments.

	Usage: ref.exe 'Path\To\Shootout_code.txt' 'Name\Of\Output_code.py'
	Richard Lindsay, 2018
*/


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <chrono>
#include "stringmanip.h"
#include "syntaxanalysis.h"
#include "codeGen.h"
#include "lexer.h"

using std::string;
using std::vector;
using std::ofstream; using std::ifstream;
using std::cin; using std::cout; using std::endl;


int main(int argc, char *argv[])
{
	auto startTime = std::chrono::system_clock::now();

	const string S2T_TABLE= "Assets\\translateFile.txt";	// Shootout to tokens translation table
	const string TOKEN_FILE = "Assets\\tokenFile.txt";		// File to write tokens to
	const string BNF_PATTERNS = "Assets\\BNFfile.txt";		// Defines allowable syntax patterns in Backus-Naur Form
	const string T2P_TABLE = "Assets\\codeTable.txt";		// Shootout to tokens translation table
	const string DATA_TABLE = "Assets\\constVarTable.txt";	// Constants and vars lookup table

	// Cleanup past runs
	remove("Assets\\constVarTable.txt");
	remove("Assets\\tokenFile.txt");

	// Handle arguments
	if (argc < 3) {
		cout << "Error: Referee requires 2 arguments; an input, and an output file" << endl;
		return -1;
	} else if (0xffffffff == GetFileAttributes(argv[1])) {
		cout << "Error: the input or output file was an invalid location" << endl;
		return -1;
	}

	//Lexical Analysis stage
	convertToTokens(argv[1], S2T_TABLE, TOKEN_FILE);


	// Syntax Analysis stage
	int code = analyseSyntax(TOKEN_FILE, BNF_PATTERNS);
	switch (code) {
		case 1: cout << "Syntax Error" << endl;	return -1;
		case -1: cout << "Syntax Error: SHOOT! expected and not found. Ensure each IF is terminated by a SHOOT!." << endl; return -1;
		case -2: cout << "Syntax Error: DRIBBLE expected and not found. Ensure each FOR is terminated by a DRIBBLE." << endl; return -1;
	}
	if (code != 0)
		return -2;

	// Code Generation
	int lNum = generateCode(TOKEN_FILE, argv[2], S2T_TABLE, DATA_TABLE);

	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> s = endTime - startTime;
	cout << lNum << " lines of code generated in " << s.count() << " seconds" << endl;
	return 0;
}