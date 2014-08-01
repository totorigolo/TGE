#pragma once
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
	
	// (r�)Initialiser
	void Init();

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
	std::unique_ptr<EditBox> mEditBox;
};
