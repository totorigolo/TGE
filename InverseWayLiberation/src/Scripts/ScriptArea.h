#pragma once

#include <memory>
#include <Box2D/Collision/b2Collision.h>
#include "../Scripts/ScriptMachine.h"

// std::pair<b2AABB, std::string>

class TriggersManager;

class ScriptArea : public std::enable_shared_from_this<ScriptArea> {
public:
    // Ctors
    ScriptArea(b2AABB b2area, const std::string &action, bool once = false);

    // Change l'action
    void SetOnce(bool once);
    bool SetAction(const std::string &action);

    // Accesseurs
    void Done(); //
    bool IsOnce(); //
    bool IsDone(); //
    b2AABB &GetAABB(); //
    const std::string &GetAction(); //

    // TODO: Les actions connaissent les areas : une fois faite, toutes les areas sont supprimées (faire une classe Area)
private:
    bool mDone;
    bool mOnce;
    b2AABB mAABB;
    std::string mAction;
};
