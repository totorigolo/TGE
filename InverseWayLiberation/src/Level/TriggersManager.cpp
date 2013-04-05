#include "TriggersManager.h"
#include "../Tools/Error.h"
#include "../Tools/Parser.h"
#include "../Tools/utils.h"
#include "../Physics/PhysicManager.h"
#include "../Physics/Callback/AABBCallback.h"
#include <iostream>

// Ctor
TriggersManager::TriggersManager()
	: mLuaMachine(nullptr),
	mPhysicMgr(PhysicManager::GetInstance())
{
}

// Dtor
TriggersManager::~TriggersManager()
{
}

// Mide à jour
void TriggersManager::Update()
{
	// Il nous faut une Machine Lua pour continuer
	if (!mLuaMachine) return;

	// Crée un callback
	AABBCallback callback(true);

	// Regarde dans chaque area si il y a des bodies
	for (auto it = mAreas.begin(); it != mAreas.end(); ++it)
	{
		// Query le monde
		callback.Clear();
		mPhysicMgr.GetWorld()->QueryAABB(&callback, it->first);

		// Si on a trouvé qqchose
		if (callback.GetFixture())
		{
			auto action = mActionMap.find(it->second);

			// Si l'action n'existe pas, on averti
			if (action == mActionMap.end())
			{
				std::cerr << "L'action n'existe pas." << std::endl;
			}

			// Sinon on l'exécute
			else
			{
				action->second->Execute(mLuaMachine);
			}
		}
	}
}

// Vidage
void TriggersManager::Clear()
{
	mActionMap.clear();
	mAreas.clear();
}

// Gestion des actions
// TODO: ActionManager
void TriggersManager::AddAction(LuaAction *action)
{
	myAssert(action, "L'action n'est pas valide.");

	mActionMap[action->GetName()] = action;
}
void TriggersManager::DeleteAction(const std::string &name)
{
	auto it = mActionMap.find(name);

	myAssert(it != mActionMap.end(), "L'action n'existe pas.");

	mActionMap.erase(it);
}
LuaAction* TriggersManager::GetAction(const std::string &name)
{
	auto it = mActionMap.find(name);

	myAssert(it != mActionMap.end(), "L'action n'existe pas.");

	return it->second;
}

// Gère les Areas
void TriggersManager::CreateArea(b2AABB area, const std::string &action)
{
	mAreas.push_back(std::make_pair(area, action));
}

// Gestion de la machine Lua
LuaMachine* TriggersManager::GetLuaMachine()
{
	return mLuaMachine;
}
void TriggersManager::SetLuaMachine(LuaMachine *luaMachine)
{
	myAssert(luaMachine, "La LuaMachine passée n'est pas valide.");

	mLuaMachine = luaMachine;
}

void TriggersManager::DebugDraw(sf::RenderTarget &target) const
{
	sf::VertexArray a(sf::LinesStrip, 5U);
	
	static sf::Font f;
	static bool fontLoaded = false;
	if (!fontLoaded)
	{
		f.loadFromFile("tex/calibri.ttf"); // TODO: ResourceMgr
		fontLoaded = true;
	}
	sf::Text text;
	text.setFont(f);
	text.setCharacterSize(40U);
	text.setColor(sf::Color::Green);

	// Dessine chaque area
	for (auto it = mAreas.begin(); it != mAreas.end(); ++it)
	{
		float width = it->first.upperBound.x - it->first.lowerBound.x;
		a[0] = sf::Vertex(b22sfVec(it->first.lowerBound, mPhysicMgr.GetPPM()), sf::Color::Blue);
		a[1] = sf::Vertex(b22sfVec(it->first.lowerBound + b2Vec2(width, 0.f), mPhysicMgr.GetPPM()), sf::Color::Blue);
		a[2] = sf::Vertex(b22sfVec(it->first.upperBound, mPhysicMgr.GetPPM()), sf::Color::Blue);
		a[3] = sf::Vertex(b22sfVec(it->first.upperBound - b2Vec2(width, 0.f), mPhysicMgr.GetPPM()), sf::Color::Blue);
		a[4] = sf::Vertex(b22sfVec(it->first.lowerBound, mPhysicMgr.GetPPM()), sf::Color::Blue);

		text.setPosition(b22sfVec(it->first.upperBound - b2Vec2(width, 0.f), mPhysicMgr.GetPPM()));
		text.setString(it->second);

		target.draw(a);
		target.draw(text);
	}
}
