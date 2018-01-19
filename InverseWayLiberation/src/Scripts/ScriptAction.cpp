#include "ScriptAction.h"
#include "ScriptArea.h"
#include "TriggersManager.h"

// Ctors
ScriptAction::ScriptAction(const std::string &_name, const std::string &_file, bool once)
	: mName(_name), mFile(_file), mOnce(once), mDone(false)
{
}
ScriptAction::ScriptAction(const std::string &_name, const std::string &_file, std::string &_function, bool once)
	: mName(_name), mFile(_file), mFunction(_function), mOnce(once), mDone(false)
{
}

// Exécute l'action
void ScriptAction::Execute(ScriptMachine *luaMachine)
{
	if (mOnce && mDone) return;

	// Si on a une fonction, on charge le fichier et exécute la fx
	if (HasFunction())
	{
		luaMachine->DoFile(mFile);

		try
		{
//			luabind::call_function<void>(luaMachine->GetLuaState(), mFunction.c_str());
			mDone = true;
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
		mDone = true;
	}

	// Gère la singularité
	if (mDone)
	{
		// Préviens toutes les areas liées à cette Action qu'elle vient d'être exécutée
		for (auto &&area : mAreas)
		{
			if (area.expired()) continue;
			area.lock().get()->Done();
		}

		// Demande à être supprimée
		if (mOnce)
			TriggersManager::GetInstance().ScheduleRemove(mName);
	}
}

// Gestion des Areas
void ScriptAction::RegisterArea(std::weak_ptr<ScriptArea> area)
{
	mAreas.push_back(area);
}

// Accesseurs
bool ScriptAction::IsOnce()
{
	return mOnce;
}
bool ScriptAction::IsDone()
{
	return mDone;
}
bool ScriptAction::HasFunction()
{
	return !mFunction.empty();
}
const std::string& ScriptAction::GetName()
{
	return mName;
}
const std::string& ScriptAction::GetFile()
{
	return mFile;
}
const std::string& ScriptAction::GetFunction()
{
	return mFunction;
}
