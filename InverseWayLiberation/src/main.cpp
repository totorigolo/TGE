#include "App/App.h"
#include "Tools/Dialog.h"
#include <iostream>
#include <exception>

int main()
{
	// Valeur de retour
	int r = 0;

	try {
		// Exécute l'App
		r = App::GetInstance().Execute();
	}
	catch (const std::exception &e)
	{
		Dialog::Error("Erreur fatale :\n" + std::string(e.what()), true);
	}
	catch (...)
	{
		Dialog::Error("Erreur fatale inconnue.", true);
	}

	// Termine l'application
    return r;
}
