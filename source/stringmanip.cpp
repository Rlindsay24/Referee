#include "stringmanip.h"
#include <string>
#include <fstream>
#include <vector>

using std::string;
using std::ifstream; using std::ofstream;
using std::vector;

vector<string> getline(const string& filePath, const string &key)	// Returns a split line, searching for a key
{
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

string constOrVar(const string &token)
{
	static int constN = 0;
	static int varN = 0; 
	string call = "Assets\\constVarTable.txt";
	ifstream ifile(call);
	string line;
	while (std::getline(ifile, line)) {
		vector<string> splitLine = split(line, ' ');
		if (splitLine[1] == token) {
			return splitLine[0];
		}
	}
	ifile.close();
	ofstream ofile(call, std::fstream::app);
	if (token[0] == '\"') {
		ofile << "CONSTANT" + std::to_string(constN) << " " << token << " " << "STRING" << "\n";
		ofile.close();
		return "CONSTANT" + std::to_string(constN++);
	} else if (is_numeric(token)) {
		ofile << "CONSTANT" + std::to_string(constN) << " " << token << " " << "INTEGER" << "\n";
		ofile.close();
		return "CONSTANT" + std::to_string(constN++);
	} else {
		ofile << "VARIABLE" + std::to_string(varN) << " " << token << "\n";
		ofile.close();
		return "VARIABLE" + std::to_string(varN++);
	}

}

vector<string> split(const string &str, char delim)
{	
	vector<string> splitLine;
	auto left = str.begin();
	auto right = left;
	while (right < str.end()) { // While the line has not been fully checked
		while (right < str.end() && *right != delim) { // Increment pointer
			if (*right == '\"') { // Skip speech pairs, ignore \"
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

		string part(left, right); // Store the string between the two
		splitLine.push_back(part); // pointers in the array
		if (right < str.end()) {
			left = ++right;
		}
	}
	return splitLine;
}

int findReplace(vector<string> &container, const string &toFind, const string &replace)
{
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
{
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (*it < 48 || *it > 57)
			return false;
	}
	return true;
}
