#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include "codeGen.h"
#include "stringmanip.h"

using std::ofstream; using std::ifstream;
using std::vector;
using std::string;


int generateCode(const string &tokenFilePath, const string &outputFile, const string &translationTablePath, const string &cVLookupTable)
{
	ifstream storedTokens(tokenFilePath);
	storedTokens.clear();
	storedTokens.seekg(0);
	ofstream outFile(outputFile);
	string line;
	vector<string> splitLine;
	string translatedLine;
	string translation;
	string end;
	int tabindex = 0;
	int lNum = 0;
	while (std::getline(storedTokens, line)) {
		splitLine = split(line, '-');
		translatedLine = "";
		end = "";
		translation = "";

		// Check for special cases affecting indentation, then apply indents
		if (splitLine[0] == "START" || splitLine[0] == "END")	// Skip line if no Python equivalent
			continue;
		if (splitLine[0] == "ELSE") {
			--tabindex;
			for (int i = 0; i < tabindex; ++i) {
				translatedLine += "\t";
			}
			if (splitLine.size() > 2 && splitLine[1] == "CONDITIONALIF") {				// Special case for elif statements
				translatedLine += "el";
				splitLine = vector<string>(splitLine.begin() + 1, splitLine.end());
			}
			else {
				translatedLine = translatedLine.substr(0, translatedLine.size() - 1);	// Special case for regular else statements
				if (splitLine.size() == 1)
					end = ":";
			}
		}
		else {
			for (int i = 0; i < tabindex; ++i) {	// Apply indents
				translatedLine += "\t";
			}
		}

		resolveToken(translation, splitLine.begin(), translationTablePath, cVLookupTable);			// Translate the next token according to the code table
		translatedLine += translation;

		// Handle special cases post-indentation
		if (translation == "print") {													// Print special case: handle multiple prints by adding a comma
			translatedLine += ' ';
			for (auto it = splitLine.begin() + 1; it < splitLine.end(); ++it) {
				resolveToken(translation, it, translationTablePath, cVLookupTable);
				if (it + 1 < splitLine.end())
					translatedLine += translation + ",";
				else
					translatedLine += translation;
			}
		}
		else if (translation == "raw_input()" || translation == "int(raw_input())") {	// Input special case: swap places of variable and input command
			string end = translation;
			resolveToken(translation, splitLine.begin() + 1, translationTablePath, cVLookupTable);
			translatedLine = "";
			for (int i = 0; i < tabindex; ++i)
				translatedLine += "\t";
			translatedLine += translation + " = " + end;
		}
		else if (translation == "for") {												// For loop special case: construct a for loop block with indentation
			++tabindex;
			resolveToken(translation, splitLine.begin() + 1, translationTablePath, cVLookupTable);
			translatedLine += " " + translation + " in range(";
			vector<string>::iterator pos = splitLine.begin() + 3;
			for (pos; *pos != "TO" && pos != splitLine.end(); ++pos) {
				resolveToken(translation, pos, translationTablePath, cVLookupTable);
				translatedLine += translation;
			}
			for (pos; pos != splitLine.end(); ++pos) {
				resolveToken(translation, pos, translationTablePath, cVLookupTable);
				translatedLine += translation;
			}
			translatedLine += "):";
		}
		else if (splitLine[0] == "ENDIF" || splitLine[0] == "NEXT" || splitLine[0] == "ENDWHILE") {	// End of block special case: reduce the indentation level
			--tabindex;
			continue;
		}
		else {
			if (splitLine[0] == "CONDITIONALIF" || splitLine[0] == "FOR")				// Start of block special case: increase indentation level
				++tabindex;
			else if (splitLine[0] == "ITERATIONWHILE") {								// While special case: increase indentation and add ':' character
				++tabindex;
				end = ":";
			}
			for (auto it = splitLine.begin() + 1; it < splitLine.end(); ++it) {			// Resolve all other tokens in the line (Expressions, assignments) 
				resolveToken(translation, it, translationTablePath, cVLookupTable);
				translatedLine += " " + translation;
			}
		}
		outFile << translatedLine + end + "\n";			// Write the generated code to file
		++lNum;
	}
	outFile.close();
	storedTokens.close();
	return lNum;
}

void resolveToken(string &token, vector<string>::iterator it, const string &codeTablePath, const string &cVLookupTable)
{
	string temptoken = *it;
	while (std::isdigit(*(temptoken.end() - 1))) {	// If last character is a number
		temptoken.resize(temptoken.length() - 1);		// Resize string to remove number
	}
	if (temptoken == "VARIABLE" || temptoken == "CONSTANT") {
		vector<string> line;
		line = getline("Assets\\constVarTable.txt", *it);
		token = line[0];
	}
	else {
		token = *(getline("Assets\\codeTable.txt", *it).begin());
	}
}
