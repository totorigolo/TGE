#pragma once
#include "Parser.h"
#include "Dialog.h"

#include <exception>
#include <sstream>
#include <string>

class Error : public std::exception
{
public:
	// Ctor & dtor
	Error(const std::string &msg, const char *file, const char *func, int line);
    virtual ~Error() throw();

	// Obtention du message
    virtual const char* what() const throw();
 
private:
    std::string mMsg;
};

// Assertion
#define myAssert(exp, msg) if(!(exp)) throw Error(msg, __FILE__, __FUNCTION__, __LINE__)

// Vérification d'erreur
#define myCheckError(exp, msg) if(!(exp)) { Dialog::Error(msg); return false; }
#define myCheckError_c(exp, msg) if(!(exp)) { Dialog::Error(msg); } // For constructors
