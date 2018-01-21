#include "TriggersManager.h"
#include "../Physics/PhysicManager.h"
#include "../Resources/ResourceManager.h"
#include "../Physics/Callback/AABBCallback.h"
#include "../Tools/utils.h"
#include "../Entities/Player.h"

// Ctor
TriggersManager::TriggersManager()
        : mScriptMachine(nullptr),
          mPhysicMgr(PhysicManager::GetInstance()) {
}

// Dtor
TriggersManager::~TriggersManager() {
}

// Mise à jour
void TriggersManager::Update(Player *player) {
    // Il nous faut une Machine de script pour continuer, et un joueur
    if (!mScriptMachine || !player) return;

    // Crée un callback
//	AABBCallback callback(true);

    // Regarde dans chaque area si il y a des bodies
    for (auto it = mAreas.begin(); it != mAreas.end(); ++it) {
        // Query le monde
//		callback.Clear();
//		mPhysicMgr.GetWorld()->QueryAABB(&callback, it->get()->GetAABB());

        // Si on a trouvé qqchose
//		if (callback.GetFixture())
        if ((*it)->GetAABB().Contains(player->GetBody()->GetFixtureList()->GetAABB(0))) {
            auto action = mActionMap.find(it->get()->GetAction());

            // Si l'action n'existe pas, on averti
            if (action == mActionMap.end()) {
                std::cerr << "L'action n'existe pas." << std::endl;
                ScheduleRemove(*it);
            }

                // Sinon on l'exécute
            else {
                action->second->Execute(mScriptMachine);
                it->get()->Done();
            }
        }
    }

    // Suppressions planifiées
    for (auto it = mAreasToDelete.begin(); it != mAreasToDelete.end();) {
        mAreas.remove(it->lock());
        it = mAreasToDelete.erase(it);
    }
    for (auto it = mActionsToDelete.begin(); it != mActionsToDelete.end();) {
        mActionMap.erase(*it);
        it = mActionsToDelete.erase(it);
    }
}

// Vidage
void TriggersManager::Clear() {
    mActionMap.clear();
    mAreas.clear();
}

// Gestion des actions
void TriggersManager::AddAction(ScriptAction *action) {
    myAssert(action, "L'action n'est pas valide.");

    mActionMap[action->GetName()] = std::shared_ptr<ScriptAction>(action);
}

void TriggersManager::DeleteAction(const std::string &name) {
    auto it = mActionMap.find(name);
    if (it != mActionMap.end()) return;
    mActionMap.erase(it);
}

std::shared_ptr<ScriptAction> TriggersManager::GetAction(const std::string &name) {
    return mActionMap[name];
}

ActionMap &TriggersManager::GetActionMap() {
    return mActionMap;
}

const ActionMap &TriggersManager::GetActionMap() const {
    return mActionMap;
}

// Gère les Areas
void TriggersManager::CreateArea(b2AABB area, const std::string &action, bool once) {
    mAreas.push_back(std::make_shared<ScriptArea>(area, action, once));

    // Enregistre l'area envers l'action
    mActionMap[action]->RegisterArea(mAreas.back());
}

void TriggersManager::DestroyArea(ScriptArea *area) {
    mAreas.remove(area->shared_from_this());
}

std::list<std::shared_ptr<ScriptArea>> &TriggersManager::GetAreas() {
    return mAreas;
}

const std::list<std::shared_ptr<ScriptArea>> &TriggersManager::GetAreas() const {
    return mAreas;
}

// Suppressions planifiées
void TriggersManager::ScheduleRemove(std::shared_ptr<ScriptArea> area) {
    mAreasToDelete.push_back(area);
}

void TriggersManager::ScheduleRemove(const std::string &action) {
    mActionsToDelete.push_back(action);
}

// Gestion de la machine de script
ScriptMachine *TriggersManager::GetScriptMachine() {
    return mScriptMachine;
}

void TriggersManager::SetScriptMachine(ScriptMachine *luaMachine) {
    myAssert(luaMachine, "La ScriptMachine passée n'est pas valide.");

    mScriptMachine = luaMachine;
}

void TriggersManager::DebugDraw(sf::RenderTarget &target) const {
    sf::VertexArray a(sf::LinesStrip, 5U);

    sf::Text text;
    text.setFont(*ResourceManager::GetInstance().GetFont("default"));
    text.setCharacterSize(40U);
    text.setFillColor(sf::Color::Green);

    // Dessine chaque area
    for (auto it = mAreas.begin(); it != mAreas.end(); ++it) {
        auto aabb = it->get()->GetAABB();
        auto action = it->get()->GetAction();
        float width = aabb.upperBound.x - aabb.lowerBound.x;
        a[0] = sf::Vertex(b22sfVec(aabb.lowerBound, mPhysicMgr.GetPPM()), sf::Color::Blue);
        a[1] = sf::Vertex(b22sfVec(aabb.lowerBound + b2Vec2(width, 0.f), mPhysicMgr.GetPPM()), sf::Color::Blue);
        a[2] = sf::Vertex(b22sfVec(aabb.upperBound, mPhysicMgr.GetPPM()), sf::Color::Blue);
        a[3] = sf::Vertex(b22sfVec(aabb.upperBound - b2Vec2(width, 0.f), mPhysicMgr.GetPPM()), sf::Color::Blue);
        a[4] = sf::Vertex(b22sfVec(aabb.lowerBound, mPhysicMgr.GetPPM()), sf::Color::Blue);

        text.setPosition(b22sfVec(aabb.upperBound - b2Vec2(width, 0.f), mPhysicMgr.GetPPM()));
        text.setString(action);

        target.draw(a);
        target.draw(text);
    }
}
