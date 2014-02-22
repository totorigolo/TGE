#include "stdafx.h"

// Ctor
Error::Error(const std::string &msg, const char *file, const char *func, int line)
{
	std::string prettyFile = file;
	prettyFile = prettyFile.substr(prettyFile.find_last_of('\\') + 1);
	
    std::ostringstream oss;
	oss << msg << std::endl
		<< std::endl
		<< "Fonction \"" << func << "\"," << std::endl
		<< "dans \"" << prettyFile << "\", ligne " << line << ".";

    mMsg = oss.str();
}
// Dtor
Error::~Error() throw()
{
}

// Obtention du message
const char* Error::what() const throw()
{
    return mMsg.c_str();
}
