#include "stdafx.h"
#include "LuaAction.h"

// Ctors
LuaAction::LuaAction(const std::string &_name, const std::string &_file)
	: mName(_name), mFile(_file)
{
}
LuaAction::LuaAction(const std::string &_name, const std::string &_file, std::string &_function)
	: mName(_name), mFile(_file), mFunction(_function)
{
}

// Exécute l'action
void LuaAction::Execute(LuaMachine *luaMachine)
{
	// Si on a une fonction, on charge le fichier et exécute la fx
	if (HasFunction())
	{
		luaMachine->DoFile(mFile);

		try
		{
			luabind::call_function<void>(luaMachine->GetLuaState(), mFunction.c_str());
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// Sinon on exécute le fichier
	else
	{
		luaMachine->DoFile(mFile);
	}
}

// Accesseurs
bool LuaAction::HasFunction()
{
	return !mFunction.empty();
}
const std::string& LuaAction::GetName()
{
	return mName;
}
const std::string& LuaAction::GetFile()
{
	return mFile;
}
const std::string& LuaAction::GetFunction()
{
	return mFunction;
}
