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

	// Déplacement des objets
	bool mMouseJointCreated;
	int mMouseJointID;
	b2Body *mMouseMovingBody;
	b2Vec2 mMouseMovingBodyAnchor;

	// Variables pour la création de joints
	b2Body *mPinBodyA;
	b2Vec2 mPinAnchorA; // Ancres locales aux bodies
	b2Body *mPinBodyB;
	b2Vec2 mPinAnchorB;

	// Eléments de la GUI
	sfg::SFGUI *mSfGUI;
	sfg::Desktop mDesktop;
	sf::Clock mGUIElapsedTime;
	sfg::Window::Ptr mTestWindow;
	sfg::Button::Ptr mButton;
};
