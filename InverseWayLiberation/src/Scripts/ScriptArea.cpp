#include "ScriptArea.h"
#include "TriggersManager.h"

// Ctors
ScriptArea::ScriptArea(b2AABB b2area, const std::string &action, bool once)
	: mAABB(b2area), mAction(action), mOnce(once), mDone(false)
{
}

// Accesseurs
void ScriptArea::Done()
{
	mDone = true;

	// Demande à être supprimée si once
	if (mOnce)
		TriggersManager::GetInstance().ScheduleRemove(shared_from_this());
}
bool ScriptArea::IsOnce()
{
	return mOnce;
}
bool ScriptArea::IsDone()
{
	return mDone;
}
b2AABB ScriptArea::GetAABB()
{
	return mAABB;
}
const std::string& ScriptArea::GetAction()
{
	return mAction;
}
