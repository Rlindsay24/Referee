#ifndef __LEXER_INCLUDED__
#define __LEXER_INCLUDED__

#include <string>

void convertToTokens(const std::string &inpFilePath, const std::string &translateFilePath, const std::string &tokenFilePath, const std::string &constVarPath);

std::string constOrVar(const std::string &token, const std::string &lookupTable);


#endif 
