#pragma once
#include "LuaMachine.h"
#include "../Tools/NonCopyable.h"
#include "../Resources/ResourceManager.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <Box2D/Box2D.h>

class PhysicManager;
class LevelManager;
class InputManager;
class Grapnel;
class Box2DGame : public NonCopyable
{
public:
	// Ctor & dtor
	Box2DGame(sf::RenderWindow &window);
	virtual ~Box2DGame(void);
	
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

	// Grapin
	b2Body *mHookedSBody;
	b2Vec2 mHookedSAnchor;
	Grapnel *mGrapnel;
	sf::Clock mHookClock;

	// D�placement des objets
	bool mMouseJointCreated;
	int mMouseJointID;
	b2Body *mMouseMovingBody;
	b2Vec2 mMouseMovingBodyAnchor;

	// Variables pour la cr�ation de joints
	b2Body *mPinBodyA;
	b2Vec2 mPinAnchorA; // Ancres locales aux bodies
	b2Body *mPinBodyB;
	b2Vec2 mPinAnchorB;

	// El�ments de la GUI
	sfg::SFGUI *mSfGUI;
	sfg::Desktop mDesktop;
	sf::Clock mGUIElapsedTime;
	sfg::Window::Ptr mTestWindow;
	sfg::Button::Ptr mButton;
};
