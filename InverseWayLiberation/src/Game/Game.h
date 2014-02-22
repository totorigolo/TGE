#pragma once
#include "../Lua/LuaMachine.h"
#include "../Resources/ResourceManager.h"
#include "../App/SpyedKey.h"

class PhysicManager;
class LevelManager;
class InputManager;
class Grapnel;
class Game : public NonCopyable
{
public:
	// Ctor & dtor
	Game(sf::RenderWindow &window);
	virtual ~Game(void);
	
	/// Boucle de jeu
	void Run();

protected:
	/* Fonctions évènements */
	/// Initialise le jeu
	inline bool OnInit();

	/// Appelé quand la boucle commence
	inline void OnLoopBegin();

	/// Appelé pour les évènements
	inline void OnEvent();
	
	/// Appelé pour la logique
	inline void OnLogic();

	/// Appelé pour la physique
	inline void OnStepPhysics();
	
	/// Appelé pour les mises à jour
	inline void OnUpdate();

	/// Appelé pour le rendu
	inline void OnRender();

	/// Appelé quand la boucle se termine
	inline void OnLoopEnd();

	// Appelé quand le jeu se termine
	inline void OnQuit();
	
private:
	// Etats du jeu
	bool mPaused;
	bool mDebugDraw;

	// Fenêtre
	bool mQuit;
	sf::RenderWindow &mWindow;
	
	// Evènements
	InputManager &mInputManager;
	std::vector<SpyedKey::Ptr> mSpyedKeys;

	// Niveau
	LevelManager &mLevel;

	// Monde physique
	PhysicManager &mPhysicMgr;

	// Textures
	ResourceManager &mResourceManager;

	// Machine Lua
	LuaMachine mConsole;

	// Positions de la souris
	b2Vec2 mMp;

	// Eléments de la GUI
	sfg::SFGUI *mSfGUI;
	sfg::Desktop mDesktop;
	sf::Clock mGUIElapsedTime;
};
