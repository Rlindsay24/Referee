#ifndef __CODEGEN_INCLUDED__
#define __CODEGEN_INCLUDED__

#include <string>
#include <vector>

int generateCode(const std::string &tokenFilePath, 
				 const std::string &outputFile, 
				 const std::string &translationTablePath, 
				 const std::string &cVLookupTable);

void resolveToken(std::string &token, 
				  std::vector<std::string>::iterator it, 
				  const std::string &codeTablePath,
				  const std::string &cVLookupTable);

#endif