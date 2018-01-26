#include "Editor.h"
#include "../App/App.h"
#include "../App/InputManager.h"
#include "../Level/LevelManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/Deco.h"
#include "../Entities/Entity.h"
#include "../Entities/Player.h"
#include "../Entities/BasicBody.h"
#include "../Entities/PointLight.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/EntityManager.h"
#include "../Entities/PartitioningTree.h"
#include "../Physics/Joints/MouseJoint.h"
#include "../Physics/Joints/DistanceJoint.h"
#include "../Physics/Callback/PointCallback.h"
#include "../Graphics/LightEngine.h"
#include "../Tools/utils.h"

// Ctor
Editor::Editor(sf::RenderWindow &window)
        : // Etats du jeu
        mPaused(false),
        mDebugDraw(false),
        // Fenêtre
        mQuit(false),
        mWindow(window),
        // Evènements
        mInputManager(InputManager::GetInstance()),
        // Ajout d'Entities
        mPolyCreationWindow(nullptr),
        mAreaCreationWindow(nullptr),
        mDecoCreationWindow(nullptr),
        mHumCreationWindow(nullptr),
        mBasicBodyCreationWindow(nullptr),
        mPointLightCreationWindow(nullptr),
        // Niveau
        mLevel(LevelManager::GetInstance()),
        mEntityMgr(EntityManager::GetInstance()),
        // Monde physique
        mPhysicMgr(PhysicManager::GetInstance()),
        // Textures
        mResourceManager(ResourceManager::GetInstance()),
        // Déplacement des objets
        mMouseJointCreated(false),
        mMouseJointID(-1),
        mMouseMovingBody(nullptr),
        // Variables pour la création de joints
        mPinBodyA(nullptr),
        mPinBodyB(nullptr),
        // GUI
        mSfGUI(App::GetInstance().GetSfGUI()) {
    myAssert(mSfGUI, "La GUI n'a pas été créée.");
}

// Dtor
Editor::~Editor(void) {
}

// (ré)Initialiser
void Editor::Init() {
    // Etats du jeu
    mPaused = false;
    mDebugDraw = false;
    // Autre
    mMouseMovingBody = nullptr;
    mMouseJointCreated = false;
    mMouseJointID = -1;
    mPinBodyA = nullptr;
    mPinBodyB = nullptr;
}

// Boucle de jeu
void Editor::Run() {
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
            }

            // Gestion de la physique
            this->OnStepPhysics(focus);

            if (focus) {
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
bool Editor::OnInit() {
    // Initialise les variables
    mPaused = false;

    /* GUI */
    // Crée la Window et le Desktop
    mInputManager.AddDesktop(&mDesktop);
    mGUIElapsedTime.restart();
    mEditBox = std::unique_ptr<EditBox>(new EditBox(mDesktop));
    mHumCreationWindow = mEditBox->GetHumCreationWindow();
    mPolyCreationWindow = mEditBox->GetPolyCreationWindow();
    mAreaCreationWindow = mEditBox->GetAreaCreationWindow();
    mDecoCreationWindow = mEditBox->GetDecoCreationWindow();
    mBasicBodyCreationWindow = mEditBox->GetBasicBodyCreationWindow();
    mPointLightCreationWindow = mEditBox->GetPointLightCreationWindow();

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
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::R)); // Reload (ex: shaders)
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::M)); // Pause physique
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::O)); // Debug Draw
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::P)); // Pin
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::H)); // Hum -> TODO: Delete
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::L)); // Toggle LightEngine
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::Add)); // Zoom in
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::Subtract)); // Zoom out
    mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::Numpad0)); // Reset vue

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

    // Enregistre la machine de script
    mLevel.SetScriptMachine(&mScriptMachine);
    mEditBox->SetScriptMachine(&mScriptMachine);
    mScriptMachine.SetConsole(mEditBox->GetScriptConsoleWindow());

    // Passe les objets à la LevelWindow
    mEditBox->GetLevelWindow()->SetEditor(this);
    mEditBox->GetLevelWindow()->SetEditBox(&*mEditBox);
    mEditBox->GetLevelWindow()->SetScriptMachine(&mScriptMachine);

    // Charge les textures vides et la police
    mResourceManager.LoadTexture("none", "tex/none.png");
    mResourceManager.LoadTexture("unknown", "tex/unknown.png");
    mResourceManager.LoadFont("default", "data/default.otf");
    mResourceManager.LoadFont("gui", "data/gui.otf");

    return true;
}

/// Appelé quand la boucle commence
void Editor::OnLoopBegin() {
    // Sauvegarde la dernière position de la souris
    mMp = sf2b2Vec(mInputManager.GetMousePosRV(), mPhysicMgr.GetMPP()); // système Box2D
}

/// Appelé pour les évènements
void Editor::OnEvent() {
    // Met à jour les évènements
    mInputManager.Update();

    // Exécute les actions planifiées de la GUI
    mEditBox->DoScheduledTasks();

    // Retient si la touche Control est enfoncée
    bool ctrl =
            mInputManager.IsKeyPressed(sf::Keyboard::LControl) || mInputManager.IsKeyPressed(sf::Keyboard::RControl);

    // Vérifie les évènements
    if (mInputManager.HasQuitted()) {
        // Demande si on veut décharger le niveau actuel
        if (1 == Dialog::ButtonChoice("Quitter l'éditeur ?",
                                      L"Voulez-vous quitter l'éditeur ?\nTout changement non sauvegardé sera perdu.",
                                      "Oui", "Non"))
            mQuit = true;
    }
    if (!mInputManager.HasFocus()) {
        return;
    }

    // Reload (ex: shaders)
    if (mInputManager.KeyPressed(sf::Keyboard::R) && ctrl) {
        std::cout << "Reload shaders" << std::endl;
        LightEngine::GetInstance().LoadShaders();
    }

    // Pause physique
    if (mInputManager.KeyPressed(sf::Keyboard::M) && ctrl) {
        mPaused = !mPaused;
    }
    // DebugDraw
    if (mInputManager.KeyPressed(sf::Keyboard::O) && ctrl) {
        mDebugDraw = !mDebugDraw;
    }
    // Light Draw toggle
    if (mInputManager.KeyPressed(sf::Keyboard::L) && ctrl) {
        LightEngine::GetInstance().SetActive(!LightEngine::GetInstance().IsActive());
    }

    // Test des Hums
    if (mInputManager.KeyPressed(sf::Keyboard::H) && ctrl) {
        EntityFactory::CreateHum(b2Vec2(-3, 8), 2, 8);
        EntityFactory::CreateHum(b2Vec2(-1, 8), 8, 20);
        EntityFactory::CreateHum(b2Vec2(1, 8), 20, -20);
        EntityFactory::CreateHum(b2Vec2(3, 8), 60, -8);
    }

    // Epingle un objet
    if (mInputManager.KeyPressed(sf::Keyboard::P) && ctrl) {
        // Enregistre les bobies et leurs ancres
        if (!mPinBodyA || !mPinBodyB) {
            // Demande au monde les formes qui sont sous l'AABB
            PointCallback callback(mMp, false);
            mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

            // Il y a un objet, on le retient
            if (callback.GetFixture()) {
                // Enregistre le body appuyé
                if (!mPinBodyA && mPinBodyB != callback.GetFixture()->GetBody()) {
                    mPinBodyA = callback.GetFixture()->GetBody();
                    mPinAnchorA = mPinBodyA->GetLocalPoint(mMp);
                } else if (!mPinBodyB && mPinBodyA != callback.GetFixture()->GetBody()) {
                    mPinBodyB = callback.GetFixture()->GetBody();
                    mPinAnchorB = mPinBodyB->GetLocalPoint(mMp);
                } else {
                    mPinBodyA = nullptr;
                    mPinBodyB = nullptr;
                }
            }

                // Si on a cliqué sur rien, on oublie les deux
            else {
                mPinBodyA = nullptr;
                mPinBodyB = nullptr;
            }
        }

        // Crée le joint
        if (mPinBodyA && mPinBodyB) {
            DistanceJointDef def(mPinBodyA, mPinAnchorA, mPinBodyB, mPinAnchorB);
            new DistanceJoint(def);

            mPinBodyA = nullptr;
            mPinBodyB = nullptr;
        }
    }

    // EditBox : Poly Creation
    if (mInputManager.IsLMBClicked() && ctrl && mPolyCreationWindow) {
        // Si la fenêtre de création de polygones est en mode création, on transmet les clics
        if (mPolyCreationWindow->IsInEditMode()) {
            mPolyCreationWindow->AddPoint(mMp);
        }
    }

    // EditBox : Area Creation
    if (mInputManager.IsLMBClicked() && ctrl && mAreaCreationWindow) {
        // Si la fenêtre de création est en mode ajout, on transmet les clics
        if (mAreaCreationWindow->IsInAddMode()) {
            mAreaCreationWindow->Add(mMp);
        }
    }

    // EditBox : Deco Creation
    if (mInputManager.IsLMBClicked() && ctrl && mDecoCreationWindow) {
        // Si la fenêtre de création est en mode ajout, on transmet les clics
        if (mDecoCreationWindow->IsInAddMode()) {
            mDecoCreationWindow->Add(mMp);
        }
    }

    // EditBox : PointLight Creation
    if (mInputManager.IsLMBClicked() && ctrl && mPointLightCreationWindow) {
        // Si la fenêtre de création est en mode ajout, on transmet les clics
        if (mPointLightCreationWindow->IsInAddMode()) {
            mPointLightCreationWindow->Add(mMp);
        }
    }

    // EditBox : BasicBody Creation
    if (mInputManager.IsLMBClicked() && ctrl && mBasicBodyCreationWindow) {
        // Si la fenêtre de création est en mode ajout, on transmet les clics
        if (mBasicBodyCreationWindow->IsInAddMode()) {
            mBasicBodyCreationWindow->Add(mMp);
        }
    }

    // EditBox : BasicBody Creation
    if (mInputManager.IsLMBClicked() && ctrl && mHumCreationWindow) {
        // Si la fenêtre de création est en mode ajout, on transmet les clics
        if (mHumCreationWindow->IsInAddMode()) {
            mHumCreationWindow->Add(mMp);
        }
    }

    // Déplacements des objets
    if (!mPaused) // Physique en continue, on utilise le MouseJoint
    {
        if (mInputManager.GetRMBState()) // Crée ou met à jour le joint
        {
            // Si la souris est déjà attachée, on met à jour la position
            if (mMouseJointCreated) {
                MouseJoint *j = ((MouseJoint *) mPhysicMgr.GetJoint(mMouseJointID));
                if (j) {
                    j->SetTarget(mMp);
                }
                    // Le joint n'existe plus
                else {
                    mMouseJointCreated = false;
                    mMouseJointID = -1;
                }
            }

                // Sinon on recherche l'objet sous la souris et on l'attache
            else {
                // Demande au monde les formes qui sont sous l'AABB
                PointCallback callback(mMp);
                mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

                // Il y a un objet, on l'attache
                if (callback.GetFixture()) {
                    // On cherche un StaticBody disponible
                    b2Body *staticBody = mPhysicMgr.GetAnyStaticBody();
                    if (staticBody) // Si il y en a un
                    {
                        // Récupère le Body
                        b2Body *body = callback.GetFixture()->GetBody();
                        MouseJointDef def(body, staticBody, mMp, 100000000000.f * body->GetMass());
                        MouseJoint *j = new MouseJoint(def);
                        mMouseJointID = j->GetID();
                        mMouseJointCreated = true;
                        mMouseMovingBody = body;
                    }
                }
            }
        } else if (mMouseJointCreated) // Supprime le joint
        {
            // Supprime le joint
            if (mPhysicMgr.JointExists(mMouseJointID)) {
                mPhysicMgr.DestroyJoint(mMouseJointID);
                mMouseJointID = -1;
                mMouseJointCreated = false;
                mMouseMovingBody = nullptr;
            }
                // Le joint n'existe plus
            else {
                mMouseJointCreated = false;
                mMouseJointID = -1;
                mMouseMovingBody = nullptr;
            }
        }
    } else // Physique en pause, on téléporte les objets
    {
        // Si on appuie sur le clic droit, et tant qu'on appuie dessus
        if (mInputManager.GetRMBState()) {
            if (!mMouseMovingBody) // Si on n'a pas encore d'objet, on le cherche
            {
                // Demande au monde les formes qui sont sous l'AABB
                PointCallback callback(mMp, false);
                mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

                // Il y a un objet, on l'attache
                if (callback.GetFixture()) {
                    mMouseMovingBody = callback.GetFixture()->GetBody();
                    mMouseMovingBodyAnchor = mMp - mMouseMovingBody->GetWorldCenter();
                }
            }
            if (mMouseMovingBody) {
                // Déplace le Body
                mMouseMovingBody->SetTransform(mMp - mMouseMovingBodyAnchor, mMouseMovingBody->GetAngle());
                mMouseMovingBody->SetAwake(true);
            }
        } else if (mMouseMovingBody) // Oublie le Body dès qu'on relache le bouton
        {
            mMouseMovingBody = nullptr;
        }
    }

    // EditBox : Sélection des objets
    if (mMouseMovingBody) {
        myAssert(mMouseMovingBody->GetUserData(), "Un b2Body sans Entity a été sélectionné.");
        mEditBox->ChangeSelectedObject((Entity *) mMouseMovingBody->GetUserData());
    } else if (mInputManager.GetRMBState()) {
        bool found = false;
        int layer = 30000000;

        // Demande au monde les formes qui sont sous l'AABB
        PointCallback callback(mMp, false);
        mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

        // Il y a un objet, on l'attache
        if (callback.GetFixture()) {
            // Récupère le Body
            Entity * e = (Entity *) callback.GetFixture()->GetBody()->GetUserData();

            // Ajoute le Body à l'EditBox
            myAssert(e, "Un b2Body sans Entity a été sélectionné.");
            mEditBox->ChangeSelectedObject(e);
            found = true;
            layer = e->GetLayer();
        }

        // On cherche ce qui n'est pas physique, et qui est au dessus (layer)

        // Récupère la position de la souris en mode SFML
        sf::Vector2f mousePos = mInputManager.GetMousePosRV();

        // Les Entities (Déco et PointLight)
        std::list<Entity *> &entities(mEntityMgr.GetEntities());
        for (auto it = entities.rbegin(); it != entities.rend() && (*it)->GetLayer() < layer; ++it) // Par Layer
        {
            // Pour chaque type
            if ((*it)->GetType() == EntityType::Deco) {
                // Vérifie si le curseur est dessus
                Deco * d = (Deco *) (*it);
                if (d->GetSprite()->getGlobalBounds().contains(mousePos)) {
                    mEditBox->ChangeSelectedObject(d);
                    found = true;
                    break;
                }
            } else if ((*it)->GetType() == EntityType::PointLight) {
                // Vérifie si le curseur est dessus
                PointLight * pl = (PointLight *) (*it);
                if (pl->GetBoundingBox().contains(mousePos)) {
                    mEditBox->ChangeSelectedObject(pl);
                    found = true;
                    break;
                }
            }

            // Les ScriptArea
            TriggersManager &triggersMgr = TriggersManager::GetInstance();
            auto &areas = triggersMgr.GetAreas();
            b2Vec2 b2MousePos = sf2b2Vec(mousePos, mPhysicMgr.GetMPP());
            for (auto it = areas.rbegin(); it != areas.rend(); ++it) {
                b2Vec2 mouseToArea = b2MousePos - (*it)->GetAABB().upperBound;
                float distSquared = mouseToArea.LengthSquared() * mPhysicMgr.GetPPM();

                if (distSquared < 30) {
                    mEditBox->ChangeSelectedObject((*it).get());
                    found = true;
                    break;
                }
            }
        }

        // Déselectione le body courant si on n'en a pas trouvé de nouveau
        if (!found)
            mEditBox->Unselect();
    }

    // Gestion du zoom et déplacement de la vue
    if (mInputManager.KeyPressed(sf::Keyboard::Add) && ctrl) {
        mInputManager.Zoom(0.8f);
    }
    if (mInputManager.KeyPressed(sf::Keyboard::Subtract) && ctrl) {
        mInputManager.Zoom(1.2f);
    }
    if (mInputManager.KeyPressed(sf::Keyboard::Numpad0) && ctrl) {
        mInputManager.SetZoom(mLevel.GetDefaultZoom());
        mInputManager.SetCenter(mLevel.GetDefaultCenter());
    }
    if (mInputManager.GetMouseWheelState()) {
        // Zoom
        if (mInputManager.GetMouseWheelDelta() > 0)
            mInputManager.Zoom(0.8f);

            // Dézoom
        else
            mInputManager.Zoom(1.2f);
    }
    if (mInputManager.GetMMBState()) {
        mInputManager.MoveCenter(
                -i2f(mInputManager.GetMousePos() - mInputManager.GetLastMousePos()) * mInputManager.GetCurrentZoom());
    }
    if (mInputManager.IsKeyPressed(sf::Keyboard::Numpad8)) {
        mInputManager.MoveCenter(sf::Vector2f(0.f, -20.f));
    }
    if (mInputManager.IsKeyPressed(sf::Keyboard::Numpad2)) {
        mInputManager.MoveCenter(sf::Vector2f(0.f, 20.f));
    }
    if (mInputManager.IsKeyPressed(sf::Keyboard::Numpad4)) {
        mInputManager.MoveCenter(sf::Vector2f(-20.f, 0.f));
    }
    if (mInputManager.IsKeyPressed(sf::Keyboard::Numpad6)) {
        mInputManager.MoveCenter(sf::Vector2f(20.f, 0.f));
    }
}

/// Appelé pour la logique
void Editor::OnLogic() {
}

/// Appelé pour la physique
void Editor::OnStepPhysics(bool hasFocus) {
    // Simule
    mPhysicMgr.Step(10, 4, !hasFocus || mPaused);

    // Suppression des objets hors zone
    if (!mPaused) {
        b2Body *b = nullptr;
        Entity * e = mEditBox->GetSelectedEntity();
        if (e) {
            if (e->GetType() == EntityType::BasicBody || e->GetType() == EntityType::PolyBody)
                b = ((BaseBody *) e)->GetBody();
            else if (e->GetType() == EntityType::Player)
                b = ((Player *) e)->GetBody();
        }
        mPhysicMgr.DestroyBodiesOut(b2Vec2(1000.f, -200.f), b2Vec2(-200.f, 200.f), b);
    }
}

/// Appelé pour les mises à jour
void Editor::OnUpdate() {
    // Met à jour les Joints
    mPhysicMgr.UpdateJoints();

    // Met à jour le niveau
    mLevel.Update();

    // Met à jour la GUI
    mDesktop.Update(mGUIElapsedTime.getElapsedTime().asSeconds());
    mGUIElapsedTime.restart();
}

/// Appelé pour le rendu
void Editor::OnRender() {
    // Rendu
    mWindow.clear(mLevel.GetBckgColor());
    mWindow.setView(mInputManager.GetView());

    // Affichage du Level
    mWindow.draw(mLevel);

    // Affichage du debug
    if (!mDebugDraw) {
        mPhysicMgr.DrawDebugData();
        PartitioningTree::GetInstance().DrawDebug(mWindow);
        TriggersManager::GetInstance().DebugDraw(mWindow);
    }

    // Affichage de la GUI
    if (mEditBox.get()) mWindow.draw(mEditBox->GetSelectionMark());
    if (mPolyCreationWindow) mPolyCreationWindow->DrawPointsOn(mWindow);
    mSfGUI->Display(mWindow);

    // Si on n'a pas le focus
    if (!mInputManager.HasFocus()) {
        sf::Text pause("Pause", *ResourceManager::GetInstance().GetFont("gui"), 60U);
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
void Editor::OnLoopEnd() {
}

/// Appelé quand le jeu se termine
void Editor::OnQuit() {
    Init();

    // Enlève le Desktop du InputManager
    mInputManager.RemoveDesktop(&mDesktop);

    // Supprime l'EditBox
    mEditBox.reset();

    // Supprime les SpyedKeys
    mSpyedKeys.clear();

    // Remet la vue par défaut
    mWindow.setView(mWindow.getDefaultView());
    mInputManager.SetView(mWindow.getDefaultView());

    // Supprime le joint de la souris
    mMouseJointCreated = false; // Suppr auto
    mMouseJointID = -1;

    // Vide le niveau
    mLevel.Clear();

    // Vide
    // TODO: Déplacer vers la ScriptMachine
    mScriptMachine.UnregisterGlobalVar("level");
    mScriptMachine.UnregisterGlobalVar("physicMgr");
    mScriptMachine.UnregisterGlobalVar("inputMgr");
}
