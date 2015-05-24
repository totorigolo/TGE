#pragma once
#include "../Lua/LuaMachine.h"

// std::pair<b2AABB, std::string>

class TriggersManager;
class LuaArea : public std::enable_shared_from_this<LuaArea>
{
public:
	// Ctors
	LuaArea(b2AABB b2area, const std::string &action, bool once = false);

	// Accesseurs
	void Done();
	bool IsOnce();
	bool IsDone();
	b2AABB GetAABB();
	const std::string& GetAction();
	
	// TODO: Les actions connaissent les areas : une fois faite, toutes les areas sont supprimées (faire une classe Area)
private:
	bool mDone;
	bool mOnce;
	b2AABB mAABB;
	std::string mAction;
};
