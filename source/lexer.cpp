#include <string>
#include <vector>
#include <fstream>
#include "stringmanip.h"
#include "lexer.h"

using std::string;
using std::vector;
using std::ofstream; using std::ifstream;

void convertToTokens(const string &inpFilePath, const string &translateFilePath, const string &tokenFilePath, const string &constVarPath)	// Performs Lexical Analysis using an input file and a correlating translation table.
{
	string line;
	vector<string> splitLine;
	bool match = false;
	string transKey;
	vector<string> splitKey;
	ifstream inpFile(inpFilePath);					// Open Shootout code
	ifstream translateFile(translateFilePath);		// Open Shootout -> Token translation table
	ofstream tokenFile(tokenFilePath);				// Open file to write tokens to

	while (std::getline(inpFile, line)) {
		splitLine = split(line, ' ');				// Read Shootout code into a vector
		for (auto it = splitLine.begin(); it != splitLine.end(); ++it) {
			translateFile.clear();					// Reset file to beginning
			translateFile.seekg(0);
			match = false;
			while (std::getline(translateFile, transKey) && match == false) {	// Match Shootout commands/variables with tokens
				splitKey = split(transKey, ' ');
				if (splitKey[0] == *it) {
					*it = splitKey[1];
					match = true;
				}
			}
			if (match == false)						// If command could not be matched, assume it is a constant or variable
				*it = constOrVar(*it, constVarPath);
			if (it + 1 != splitLine.end())			// If the line is not fully parsed, add a '-' character
				*it += '-';
			tokenFile << *it;						// Write the token to file
		}
		tokenFile << "\n";
	}
	inpFile.close();
	translateFile.close();
	tokenFile.close();
}

string constOrVar(const string &token, const string &lookupTable)
{	// Determines if the given token is a constant or variable, and stores it in a lookup table.
	static int constN = 0;
	static int varN = 0;
	ifstream ifile(lookupTable);
	string line;

	// Read the line and split into a vector.
	while (std::getline(ifile, line)) {
		vector<string> splitLine = split(line, ' ');
		if (splitLine[1] == token) {
			return splitLine[0];
		}
	}
	ifile.close();

	// Determine type of data and store in file.
	ofstream ofile(lookupTable, std::fstream::app);
	if (token[0] == '\"') {
		ofile << "CONSTANT" + std::to_string(constN) << " " << token << " " << "STRING" << "\n";
		ofile.close();
		return "CONSTANT" + std::to_string(constN++);
	}
	else if (is_numeric(token)) {
		ofile << "CONSTANT" + std::to_string(constN) << " " << token << " " << "INTEGER" << "\n";
		ofile.close();
		return "CONSTANT" + std::to_string(constN++);
	}
	else {
		ofile << "VARIABLE" + std::to_string(varN) << " " << token << "\n";
		ofile.close();
		return "VARIABLE" + std::to_string(varN++);
	}
}
