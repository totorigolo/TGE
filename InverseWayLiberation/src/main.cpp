#include "stdafx.h"
#include "App/App.h"
#include "Tools/Dialog.h"

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
		std::cerr << "Erreur fatale :\n" << std::string(e.what()) << std::endl;
		Dialog::Error("Erreur fatale :\n" + std::string(e.what()));
	}
	catch (...)
	{
		std::cerr << "Erreur fatale inconnue." << std::endl;
		Dialog::Error("Erreur fatale inconnue.");
	}

	// Termine l'application
	return r;
}
