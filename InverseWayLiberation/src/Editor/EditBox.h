#pragma once

#include "GUI/LevelWindow.h"
#include "GUI/DecoScenario.h"
#include "GUI/EmptyScenario.h"
#include "GUI/HumScenario.h"
#include "GUI/BodyScenario.h"
#include "GUI/TexturesWindow.h"
#include "GUI/ScriptConsoleWindow.h"
#include "GUI/HumCreationWindow.h"
#include "GUI/PointLightScenario.h"
#include "GUI/ColFilteringWindow.h"
#include "GUI/DecoCreationWindow.h"
#include "GUI/PolyCreationWindow.h"
#include "GUI/BasicBodyCreationWindow.h"
#include "GUI/PointLightCreationWindow.h"
#include "../Entities/Entity.h"
#include "../Physics/Joint.h"
#include "../Scripts/TriggersManager.h"
#include "../Level/LevelManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityManager.h"

class EditBox : public NonCopyable {
public:
    enum class SelectionType {
        Null,
        Entity,
        BasicBody,
        PolyBody,
        PolyChain,
        Deco,
        PointLight,
        Grapnel,
        Hum,
        Player,
        Joint,
        DistanceJoint,
        FrictionJoint,
        GearJoint,
        PrismaticJoint,
        PulleyJoint,
        RevoluteJoint,
        RopeJoint,
        WeldJoint,
        WheelJoint
    };

    std::string SelectionTypeToString(const SelectionType &type);

public:
    // Ctor & Dtor
    EditBox(sfg::Desktop &desktop);

    virtual ~EditBox();

    // Actions différées
    void DoScheduledTasks();

    void ScheduleUpdate();

    void ScheduleUnselection();

    // Change l'objet actuel
    void ChangeSelectedObject(Entity *entity);

    void ChangeSelectedObject(Joint *joint);

    void Unselect();

    SelectionType GetSelectionType();

    Entity *GetSelectedEntity();

    Joint *GetSelectedJoint();

    // Vide la EditBox
    void EmptyGUI();

    // Mise à jour des Widgets en f° de la sélection
    void UpdateGUI();

    // Gestion de la Machine de script
    void SetScriptMachine(ScriptMachine *luaMachine);

    ScriptMachine *GetScriptMachine();

    // Obtient le repère de l'objet sélectionné
    sf::CircleShape GetSelectionMark();

    // Mise en place des différents scénarios
    void ShowEmptyScenario();

    void OnShowLevelWindow(); // Callback
    void ShowHumScenario();

    void ShowBodyScenario();

    void ShowDecoScenario();

    void ShowPointLightScenario();

    void ShowHumCreationWindow();

    void OnShowScriptConsoleWindow(); // Callback
    void OnShowColFilteringWindow(); // Callback
    void ShowTexturesWindow();

    void ShowDecoCreationWindow();

    void ShowPolyCreationWindow();

    void ShowBasicBodyCreationWindow();

    void ShowPointLightCreationWindow();

    // Fermeture des fenêtres / scénarios
    void OnCloseLevelWindow(); // Callback

    // Accesseurs des fenêtres et scénarii
    EmptyScenario *GetEmptyScenario();

    LevelWindow *GetLevelWindow();

    HumScenario *GetHumScenario();

    DecoScenario *GetDecoScenario();

    BodyScenario *GetBodyScenario();

    TexturesWindow *GetTexturesWindow();

    ScriptConsoleWindow *GetScriptConsoleWindow();

    HumCreationWindow *GetHumCreationWindow();

    PointLightScenario *GetPointLightScenario();

    ColFilteringWindow *GetColFilteringWindow();

    DecoCreationWindow *GetDecoCreationWindow();

    PolyCreationWindow *GetPolyCreationWindow();

    BasicBodyCreationWindow *GetBasicBodyCreationWindow();

    PointLightCreationWindow *GetPointLightCreationWindow();

private:
    // Actions différées
    bool mUpdateScheduled;
    bool mUnselectionScheduled;

    // Fenêtre
    sfg::Desktop &mDesktop;
    sfg::Window::Ptr mWindow;

    // LevelMgr, PhysicMgr, EntityManager
    LevelManager &mLevelMgr;
    PhysicManager &mPhysicMgr;
    EntityManager &mEntityMgr;

    // Objet sélectionné
    Entity *mSelectedEntity;
    Joint *mSelectedJoint;
    SelectionType mSelectionType;
    bool mSelectionChanged;

    // Différentes fenêtres / scénarios
    HumScenario mHumScenario;
    DecoScenario mDecoScenario;
    BodyScenario mBodyScenario;
    EmptyScenario mEmptyScenario;
    PointLightScenario mPointLightScenario;
    LevelWindow mLevelWindow;
    TexturesWindow mTexturesWindow;
    HumCreationWindow mHumCreationWindow;
    ColFilteringWindow mColFilteringWindow;
    DecoCreationWindow mDecoCreationWindow;
    PolyCreationWindow mPolyCreationWindow;
    ScriptConsoleWindow mScriptConsoleWindow;
    BasicBodyCreationWindow mBasicBodyCreationWindow;
    PointLightCreationWindow mPointLightCreationWindow;

    // Etat des fenêtres / scénarios
    bool mLevelWindowAdded;
    bool mEditBoxGUICreated;
    bool mTexturesWindowAdded;
    bool mHumCreationWindowAdded;
    bool mColFilteringWindowAdded;
    bool mDecoCreationWindowAdded;
    bool mPolyCreationWindowAdded;
    bool mScriptConsoleWindowAdded;
    bool mBasicBodyCreationWindowAdded;
    bool mPointLightCreationWindowAdded;
};
