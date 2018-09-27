#include <string>
#include <vector>
#include <fstream>
#include "stringmanip.h"
#include "lexer.h"

using std::string;
using std::vector;
using std::ofstream; using std::ifstream;


void convertToTokens(const string &inpFilePath, const string &translateFilePath, const string &tokenFilePath)	// Performs Lexical Analysis using an input file and a correlating translation table.
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
				*it = constOrVar(*it);
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
