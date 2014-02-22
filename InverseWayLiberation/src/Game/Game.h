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
	/* Fonctions �v�nements */
	/// Initialise le jeu
	inline bool OnInit();

	/// Appel� quand la boucle commence
	inline void OnLoopBegin();

	/// Appel� pour les �v�nements
	inline void OnEvent();
	
	/// Appel� pour la logique
	inline void OnLogic();

	/// Appel� pour la physique
	inline void OnStepPhysics();
	
	/// Appel� pour les mises � jour
	inline void OnUpdate();

	/// Appel� pour le rendu
	inline void OnRender();

	/// Appel� quand la boucle se termine
	inline void OnLoopEnd();

	// Appel� quand le jeu se termine
	inline void OnQuit();
	
private:
	// Etats du jeu
	bool mPaused;
	bool mDebugDraw;

	// Fen�tre
	bool mQuit;
	sf::RenderWindow &mWindow;
	
	// Ev�nements
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

	// El�ments de la GUI
	sfg::SFGUI *mSfGUI;
	sfg::Desktop mDesktop;
	sf::Clock mGUIElapsedTime;
};
