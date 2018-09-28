#ifndef __STRINGMANIP_INCLUDED__
#define __STRINGMANIP_INCLUDED__

#include <string>
#include <vector>

std::vector<std::string> split(const std::string &str, char delim);

int findReplace(std::vector<std::string> &container, const std::string &toFind, const std::string &replace);

bool is_numeric(const std::string&);

std::vector<std::string> getline(const std::string& filePath, const std::string &key);

#endif 
