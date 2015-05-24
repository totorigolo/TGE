#include "stdafx.h"
#include "LuaAction.h"
#include "LuaArea.h"
#include "TriggersManager.h"

// Ctors
LuaAction::LuaAction(const std::string &_name, const std::string &_file, bool once)
	: mName(_name), mFile(_file), mOnce(once), mDone(false)
{
}
LuaAction::LuaAction(const std::string &_name, const std::string &_file, std::string &_function, bool once)
	: mName(_name), mFile(_file), mFunction(_function), mOnce(once), mDone(false)
{
}

// Ex�cute l'action
void LuaAction::Execute(LuaMachine *luaMachine)
{
	if (mOnce && mDone) return;

	// Si on a une fonction, on charge le fichier et ex�cute la fx
	if (HasFunction())
	{
		luaMachine->DoFile(mFile);

		try
		{
			luabind::call_function<void>(luaMachine->GetLuaState(), mFunction.c_str());
			mDone = true;
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// Sinon on ex�cute le fichier
	else
	{
		luaMachine->DoFile(mFile);
		mDone = true;
	}

	// G�re la singularit�
	if (mDone)
	{
		// Pr�viens toutes les areas li�es � cette Action qu'elle vient d'�tre ex�cut�e
		for each (auto area in mAreas)
		{
			if (area.expired()) continue;
			area.lock().get()->Done();
		}

		// Demande � �tre supprim�e
		if (mOnce)
			TriggersManager::GetInstance().ScheduleRemove(mName);
	}
}

// Gestion des Areas
void LuaAction::RegisterArea(std::weak_ptr<LuaArea> area)
{
	mAreas.push_back(area);
}

// Accesseurs
bool LuaAction::IsOnce()
{
	return mOnce;
}
bool LuaAction::IsDone()
{
	return mDone;
}
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
