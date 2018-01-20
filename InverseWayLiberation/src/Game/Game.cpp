#include "Game.h"
#include "../App/App.h"
#include "../App/InputManager.h"
#include "../Level/LevelLoader.h"
#include "../Level/LevelManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityManager.h"
#include "../Tools/utils.h"

// Ctor
Game::Game(sf::RenderWindow &window)
        : // Etats du jeu
          mPaused(false),
          mDebugDraw(false),
        // Fenêtre
          mQuit(false),
          mWindow(window),
        // Evènements
          mInputManager(InputManager::GetInstance()),
        // Niveau
          mLevel(LevelManager::GetInstance()),
        // Physique
          mPhysicMgr(PhysicManager::GetInstance()),
        // Ressources
          mResourceManager(ResourceManager::GetInstance()),
        // GUI
          mSfGUI(App::GetInstance().GetSfGUI()) {
    myAssert(mSfGUI, "La GUI n'a pas été créée.");
}

// Dtor
Game::~Game(void) {
}

// (ré)Initialiser
void Game::Init() {
    // Etats du jeu
    mPaused = false;
    mDebugDraw = false;
}

// Boucle de jeu
void Game::Run() {
    // Appel l'initialisation
    if (this->OnInit()) {
        // Tant que la fenêtre est ouverte
        while (mWindow.isOpen() && !mQuit) {
            // Gestion du focus
            bool focus = mInputManager.HasFocus();
            if (!focus)
                mWindow.setFramerateLimit(20U);
            else
                mWindow.setFramerateLimit(60U);

            if (focus) {
                // La boucle commence
                this->OnLoopBegin();
            }

            // Appel des évènements
            this->OnEvent();

            if (focus) {
                // Appel de la logique
                this->OnLogic();

                // Gestion de la physique
                this->OnStepPhysics();

                // Appel des mises à jour
                this->OnUpdate();
            }

            // Rendu
            this->OnRender();

            if (focus) {
                // La boucle se termine
                this->OnLoopEnd();
            }
        }
    }

    // On quite
    this->OnQuit();
}

/* Fonctions évènements */
/// Initialise le jeu
bool Game::OnInit() {
    // Initialise les variables
    mPaused = false;

    /* GUI */
    // Crée la Window et le Desktop
    mInputManager.AddDesktop(&mDesktop);
    mGUIElapsedTime.restart();

    // Charge le style de la GUI
    try {
        mDesktop.LoadThemeFromFile("data/style.css");
    }
    catch (const std::exception &e) {
        Dialog::Error(L"Erreur lors de la lecture du thème :\n" + sf::String(e.what()));
    }

    /* Physique */
    // Initialise le monde
    mPhysicMgr.SetTimeStep(1.f / 60.f);
    mPhysicMgr.SetDebugDrawTarget(&mWindow);

    /* Evènements */
    // Enregistre la vue
    mInputManager.SetView(mWindow.getDefaultView());

    // Demande l'espionnage de touches
    // Evènements
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::M)); // Pause physique
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::O)); // Debug Draw
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::R)); // Recharger
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::F12)); // Console de scripts

    // Initialise la machine de script
    mScriptMachine.RegisterJoint();
//	mScriptMachine.RegisterEntity();
    mScriptMachine.RegisterEntityFactory();
    mScriptMachine.RegisterLevelManager();
    mScriptMachine.RegisterGlobalVar("level", &mLevel);
    mScriptMachine.RegisterPhysicManager();
    mScriptMachine.RegisterGlobalVar("physicMgr", &mPhysicMgr);
    mScriptMachine.RegisterInputManager();
    mScriptMachine.RegisterGlobalVar("inputMgr", &mInputManager);
    mScriptMachine.RegisterResourceManager();
    mScriptMachine.RegisterGlobalVar("resourceMgr", &mResourceManager);

    // Enregistre la console
    mLevel.SetScriptMachine(&mScriptMachine);
    mScriptConsole.SetScriptMachine(&mScriptMachine);
    mScriptConsole.RegisterInDesktop(&mDesktop);
    mScriptMachine.SetConsole(&mScriptConsole);

    // Charge le niveau
    //Init();
    //mScriptMachine.Reset();
    LevelLoader("lvls/level_1.xvl");

    return true;
}

/// Appelé quand la boucle commence
void Game::OnLoopBegin() {
    // Sauvegarde la dernière position de la souris
    mMp = sf2b2Vec(mInputManager.GetMousePosRV(), mPhysicMgr.GetMPP()); // système Box2D
}

/// Appelé pour les évènements
void Game::OnEvent() {
    // Met à jour les évènements
    mInputManager.Update();

    // Vérifie les évènements
    if (mInputManager.HasQuitted()) {
        // Demande si on veut décharger le niveau actuel
        if (1 == Dialog::ButtonChoice("Quitter le jeu ?",
                                      L"Voulez-vous quitter le jeu ?\nToute progression non sauvegardée sera perdue.",
                                      "Oui", "Non"))
            mQuit = true;
    }
    if (!mInputManager.HasFocus()) {
        return;
    }

    // Retient si la touche Control est enfoncée
    bool ctrl =
            mInputManager.IsKeyPressed(sf::Keyboard::LControl) || mInputManager.IsKeyPressed(sf::Keyboard::RControl);

    // Pause physique
    if (mInputManager.KeyPressed(sf::Keyboard::M) && ctrl) {
        mPaused = !mPaused;
    }
    // DebugDraw
    if (mInputManager.KeyPressed(sf::Keyboard::O) && ctrl) {
        mDebugDraw = !mDebugDraw;
    }

    // DebugDraw
    if (mInputManager.KeyPressed(sf::Keyboard::F12)) {
        if (!mScriptConsole.IsVisible()) {
            mScriptConsole.Show();
        } else {
            mScriptConsole.Hide();
        }
    }

    // Recharger le niveau
    if (mInputManager.KeyPressed(sf::Keyboard::R) && ctrl) {
        // Charge le niveau
        Init();
        mScriptMachine.Reset();
        LevelLoader("lvls/level_1.xvl");
    }
}

/// Appelé pour la logique
void Game::OnLogic() {
}

/// Appelé pour la physique
void Game::OnStepPhysics() {
    // Simule
    if (!mPaused) {
        mPhysicMgr.Step(10, 4);

        mPhysicMgr.DestroyBodiesOut(b2Vec2(1000.f, -200.f), b2Vec2(-200.f, 200.f));
    }
}

/// Appelé pour les mises à jour
void Game::OnUpdate() {
    // Met à jour les Joints
    mPhysicMgr.UpdateJoints();

    // Met à jour le niveau
    mLevel.Update();

    // Met à jour la GUI
    mDesktop.Update(mGUIElapsedTime.getElapsedTime().asSeconds());
    mGUIElapsedTime.restart();
}

/// Appelé pour le rendu
void Game::OnRender() {
    // Rendu
    mWindow.clear(mLevel.GetBckgColor());
    mWindow.setView(mInputManager.GetView());

    // Affichage du Level
    mWindow.draw(mLevel);

    // Affichage du debug
    if (mDebugDraw) mPhysicMgr.DrawDebugData();

    // Affichage de la GUI
    mSfGUI->Display(mWindow);

    // Si on n'a pas le focus
    if (!mInputManager.HasFocus()) {
        static sf::Font f;
        static bool fontLoaded = false;
        if (!fontLoaded) {
            f.loadFromFile("data/default.otf"); // TODO: ResourceMgr
            fontLoaded = true;
        }
        sf::Text pause("Pause", f, 60U);
        sf::Vector2f pos(mInputManager.GetWindowView().getCenter().x - pause.getGlobalBounds().width / 2.f, 0.f);
        pos.y = mWindow.mapPixelToCoords(f2i(pos), mInputManager.GetWindowView()).y;
        pause.setPosition(pos);

        mWindow.setView(mInputManager.GetWindowView());
        mWindow.draw(pause);
        mWindow.setView(mInputManager.GetView());
    }

    // Affichage du tout
    mWindow.display();
}

/// Appelé quand la boucle se termine
void Game::OnLoopEnd() {
}

/// Appelé quand le jeu se termine
void Game::OnQuit() {
    Init();

    // Enlève le Desktop du InputManager
    mInputManager.RemoveDesktop(&mDesktop);

    // Supprime les SpyedKeys
    mSpyedKeys.clear();

    // Remet la vue par défaut
    mWindow.setView(mWindow.getDefaultView());
    mInputManager.SetView(mWindow.getDefaultView());

    // Vide le niveau
    mLevel.Clear();

    // Vide
    // TODO: Déplacer vers la ScriptMachine
    mScriptMachine.UnregisterGlobalVar("level");
    mScriptMachine.UnregisterGlobalVar("physicMgr");
    mScriptMachine.UnregisterGlobalVar("inputMgr");
}
