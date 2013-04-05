#pragma once
#include <exception>
#include <sstream>
#include <string>
#include "Parser.h"

class Error : public std::exception
{
public:
	// Ctor & dtor
	//Error(const char *msg, const char *file, const char *func, int line);
	Error(const std::string &msg, const char *file, const char *func, int line);
    virtual ~Error() throw();

	// Obtention du message
    virtual const char* what() const throw();
 
private:
    std::string mMsg;
};

// Assertion pour les fonctions
#define myAssert(exp, msg) if(!(exp)) throw Error(msg, __FILE__, __FUNCTION__, __LINE__)
