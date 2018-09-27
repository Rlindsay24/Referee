#include "stringmanip.h"
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <fstream>
#include <iostream>
using std::cout; using std::endl;
#include <cctype>
#include <algorithm>


string dataType(string identifier);
bool checkLineSyntax(vector<string> tokens);
vector<string> overallCommand(vector<string> line);
vector<int> count(vector<string>::iterator start, vector<string>::iterator end, char delim);
vector<string> getline(const string& filePath, const string &key);

vector<int> matchTokens(vector<string>::iterator BNFstartp,
		 vector<string>::iterator BNFendp,
		 vector<string>::iterator Tstartp,
		 vector<string>::iterator Tendp)
{	
	vector<int> pos = count(BNFstartp, BNFendp, '|');	// Get positions of '|' characters
	int j = 0;
	vector<int> tokensUsed;
	vector<string>::iterator Tstartref = Tstartp;
	for (auto i = pos.begin(); i < pos.end(); ++i) {	// For each '|' set
		Tstartp = Tstartref;
		bool match = true;
		for (j; j < *i; ++j) {		// For each token in '|' set
			if (*(BNFstartp+j) == "|") ++j;
			if (Tstartp >= Tendp) {
				tokensUsed.push_back(-1);
				Tstartp = Tstartref;
				j = *i;
				match = false;
			} else {
				if (*((*(BNFstartp+j)).end()-1) == '*') {	// if last character == *	
					string key((*(BNFstartp+j)).begin(), (*(BNFstartp+j)).end()-1);
					string call = PATH + "Assets/BNFfile.txt";
					vector<string> newBNF = getline(call, key);
					vector<int> thistokenUsed = matchTokens(newBNF.begin(), newBNF.end(), Tstartp, Tendp); // Recursively check, returning the number of tokens used
					vector<int> tempQueue;
					for (auto poss = thistokenUsed.begin(); poss < thistokenUsed.end(); ++poss) {
						if (*poss == -1)
							continue;
						vector<int> correctPossibilities;
						if (BNFstartp+j+1 < BNFstartp+*i) {
							correctPossibilities = matchTokens(BNFstartp+j+1, BNFstartp+*i, Tstartp+*poss, Tendp);
						} else
							correctPossibilities = {0};
						for (auto n = correctPossibilities.begin(); n < correctPossibilities.end(); ++n) {
							if (*n != -1) {
								if (i == pos.begin()) {
									tempQueue.push_back(j+*n+*poss);
								} else
									tempQueue.push_back(j-1-*(i-1)+*n+*poss);
							}
						}
						
					}
					for (auto i = tempQueue.begin(); i < tempQueue.end(); ++i)
						tokensUsed.push_back(*i);
					Tstartp = Tstartref;
					j = *i;
					match = false;
				} else {
					while (std::isdigit(*((*Tstartp).end()-1))) { // If last character is a number
						(*Tstartp).resize((*Tstartp).length()-1);	//Resize string to remove number
					} 
					if (*Tstartp == *(BNFstartp+j) && Tstartp < Tendp) {
						++Tstartp;
					} else {
						Tstartp = Tstartref;	// Reset pointers and try next '|' set
						j = *i;
						match = false;
					}
				}
			}
		}
		if (j == *i && match == true) { // If all tokens were matched without error
			if (i == pos.begin()) {
				tokensUsed.push_back(*i);
			} else {			// Return the length of the tokens used
				tokensUsed.push_back(*i-1-*(i-1));
			}
		}
	}
	if (tokensUsed.size() == 0)
		return {-1};
	else
		return tokensUsed;
}

vector<int> count(vector<string>::iterator start, vector<string>::iterator end, char delim)	
{			// Function counts for '|' character, placing its position in an array when one is found
	vector<int> positions;
	int n = 0;
	for (n; start < end; ++start, ++n) {
		if (*start == "|") {
			positions.push_back(n);	
		}
	}
	positions.push_back(n);
	return positions;
}


vector<string> getline(const string& filePath, const string &key)	// Returns a split line, searching for a key
{
	std::ifstream file(filePath);
	string line;
	while (std::getline(file, line)) {
		vector<string> splitline = split(line, ' ');
		if (splitline[0] == key) {
			file.close();
			vector<string> BNFtokens(splitline.begin()+1, splitline.end());
			return BNFtokens;
		}
	}
	return {"NOTFOUND"};
	
}
	
bool checkLineSyntax(vector<string> tokens)
{
	static unsigned lineNum = 0;
	static vector<string> assignedVars;
	lineNum++;
	vector<string> original = tokens;
	vector<string> BNF = overallCommand(tokens);
	vector<int> tokensUsed;
	if (BNF[0] != "NOTFOUND")
		tokensUsed = matchTokens(BNF.begin(), BNF.end(), tokens.begin(), tokens.end());
	else {
		cout << "Error on line " << lineNum << ": "
		     << "The command of the line could not be found.\n"
		     << "You must use commands like CHANT and IF." << endl;
		return false;
	}
	bool valid = false;
	for (auto count = tokensUsed.begin(); count != tokensUsed.end() && valid == false; ++count) { // Check for a full token match
		if (tokens.size() == *count)
			valid = true;
		
	}
	if (!valid) {
		cout << "Error on line " << lineNum << ": "
		     << "The syntax of the line is incorrect." << endl;
		return false;
	} 
	if (BNF[0] == "VARIABLE") {
		assignedVars.push_back(original[0]);		// Store assigned variables
	} else if (BNF[0] == "ITERATIONFOR") {
		assignedVars.push_back(original[1]);
	}
	for (auto i = original.begin(); i != original.end(); ++i) { // Check variables for assignment
		if ((*i).substr(0, (*i).size()-1) == "VARIABLE" &&
		     BNF[0] != "VARIABLE") {
			valid = false;
			for (auto var = assignedVars.begin(); var < assignedVars.end(); ++var) {
				if (*var == (*i))
					valid = true;
			}
			if (!valid) {
				cout << "Error on line " << lineNum << ": "
				     << "The variable is not yet assigned" << endl;
				return false;
			}
		}
	}
	// Check set matches; ENDIF and ELSE must have an IF
	return true;

} 

vector<string> overallCommand(vector<string> tokens)
{
	string key = tokens[0];
	while (std::isdigit(*(key.end()-1))) {
		key.resize(key.length()-1);	
	}
	string call = PATH + "Assets/BNFfile.txt";
	std::ifstream file(call);
	string line;
	while (std::getline(file, line)) {
		vector<string> splitline = split(line, ' ');
		if (splitline[1] == key) {
			file.close();
			string command = splitline[0];
			vector<string> BNF(splitline.begin()+1, splitline.end());
			return BNF;
		}
	}
	return {"NOTFOUND"};
}
