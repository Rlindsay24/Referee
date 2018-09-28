#include "stringmanip.h"
#include <string>
#include <fstream>
#include <vector>

using std::string;
using std::ifstream; using std::ofstream;
using std::vector;

vector<string> getline(const string& filePath, const string &key)	
{	// Returns the line in a specified file associated with a key value. The returned line is split into a vector.
	std::ifstream file(filePath);
	string line;
	while (std::getline(file, line)) {
		vector<string> splitline = split(line, ' ');
		if (splitline[0] == key) {
			file.close();
			vector<string> BNFtokens(splitline.begin() + 1, splitline.end());
			return BNFtokens;
		}
	}
	return { "NOTFOUND" };

}


vector<string> split(const string &str, char delim)
{	// Splits a string into a vector, with a given delimiter character.
	vector<string> splitLine;
	auto left = str.begin();
	auto right = left;
	while (right < str.end()) {							// While the line has not been fully checked
		while (right < str.end() && *right != delim) {	// Increment pointer
			if (*right == '\"') {						// Skip speech pairs, ignore \"
				++right;
				while (right < str.end() && *right != '\"' && *(right-1) != '\\') {
					++right; }
				if (*right == '\"') {
					++right;
				}
			} else {
				++right;
			}
		} 
		string part(left, right);			// Store the string between the two
		splitLine.push_back(part);			// pointers in the array
		if (right < str.end()) {
			left = ++right;
		}
	}
	return splitLine;
}

int findReplace(vector<string> &container, const string &toFind, const string &replace)
{	// Finds a string in a vector and replaces it with another.
	for (vector<string>::iterator i = container.begin();
	     i < container.end();
	     ++i) {
		if (*i == toFind) {
			*i = replace;
			return 0; 
		}
	}
	return -1; // Could not find term
}

bool is_numeric(const string &str)
{	// Determines if a string is numeric using ASCII values.
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (*it < 48 || *it > 57)
			return false;
	}
	return true;
}
