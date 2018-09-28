#ifndef __SYNTAXANALYSIS_INCLUDED__
#define __SYNTAXANALYSIS_INCLUDED__

#include <string>
#include <vector>

int analyseSyntax(const std::string &tokenFilePath, const std::string &BNFPath);

bool checkLineSyntax(std::vector<std::string> tokens, const std::string& BNFPath);

std::vector<std::string> overallCommand(std::vector<std::string> line, const std::string& BNFPath);

std::vector<int> count(std::vector<std::string>::iterator start, 
					   std::vector<std::string>::iterator end, char delim);

std::vector<int> matchTokens(std::vector<std::string>::iterator BNFstartp, 
							 std::vector<std::string>::iterator BNFendp, 
							 std::vector<std::string>::iterator Tstartp, 
							 std::vector<std::string>::iterator Tendp,
							 const std::string &BNFPath);

#endif
