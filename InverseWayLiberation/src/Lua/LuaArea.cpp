#include "LuaArea.h"
#include "TriggersManager.h"

// Ctors
LuaArea::LuaArea(b2AABB b2area, const std::string &action, bool once)
	: mAABB(b2area), mAction(action), mOnce(once), mDone(false)
{
}

// Accesseurs
void LuaArea::Done()
{
	mDone = true;

	// Demande à être supprimée si once
	if (mOnce)
		TriggersManager::GetInstance().ScheduleRemove(shared_from_this());
}
bool LuaArea::IsOnce()
{
	return mOnce;
}
bool LuaArea::IsDone()
{
	return mDone;
}
b2AABB LuaArea::GetAABB()
{
	return mAABB;
}
const std::string& LuaArea::GetAction()
{
	return mAction;
}
