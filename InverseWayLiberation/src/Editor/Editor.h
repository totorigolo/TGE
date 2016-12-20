#pragma once

#include <SFGUI/SFGUI.hpp>
#include "../Lua/LuaMachine.h"
#include "../Resources/ResourceManager.h"
#include "../Editor/EditBox.h"
#include "../Editor/GUI/HumCreationWindow.h"
#include "../Editor/GUI/PolyCreationWindow.h"
#include "../Editor/GUI/DecoCreationWindow.h"
#include "../Editor/GUI/BasicBodyCreationWindow.h"
#include "../Editor/GUI/PointLightCreationWindow.h"
#include "../App/SpyedKey.h"

class PhysicManager;
class LevelManager;
class InputManager;
class Editor : public NonCopyable
{
public:
	// Ctor & dtor
	Editor(sf::RenderWindow &window);
	virtual ~Editor(void);
	
	// (ré)Initialiser
	void Init();

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

	// Ajout d'Entities
	PolyCreationWindow *mPolyCreationWindow;
	DecoCreationWindow *mDecoCreationWindow;
	HumCreationWindow *mHumCreationWindow;
	BasicBodyCreationWindow *mBasicBodyCreationWindow;
	PointLightCreationWindow *mPointLightCreationWindow;

	// Niveau
	LevelManager &mLevel;
	EntityManager &mEntityMgr;

	// Monde physique
	PhysicManager &mPhysicMgr;

	// Textures
	ResourceManager &mResourceManager;

	// Machine Lua
	LuaMachine mConsole;

	// Positions de la souris
	b2Vec2 mMp;

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
	std::unique_ptr<EditBox> mEditBox;
};
