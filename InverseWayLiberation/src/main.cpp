#include "App/App.h"
#include <iostream>

int main()
{
	// Valeur de retour
	int r = 0;

	// Pas de gestion des exceptions globales en debug
#ifndef _DEBUG
	try {
#endif

		// Cr�e et lance l'App
		App app;
		r = app.Execute();

#ifndef _DEBUG
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
#endif

	// Termine l'application
    return r;
}
