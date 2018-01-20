#pragma once

#include "../Resources/ResourceManager.h"

class Player;

class LevelSaver;

class LevelLoader;

class EntityManager;

class TriggersManager;

class PartitioningTree;

class PhysicManager;

class ScriptMachine;

class LevelManager : public sf::Drawable, public Singleton<LevelManager> {
    friend class LevelSaver;

    friend class LevelLoader;

protected:
    friend class Singleton<LevelManager>;

    // Ctor & dtor
    LevelManager();

    virtual ~LevelManager(void);

public:
    // Charge un niveau à partir d'un XVL
    void LoadFromFile(const std::string &path);

    // Vide tout le niveau
    void Clear();

    // Mise à jour
    void Update();

    // Appelé juste avant la boucle de jeu, après son remplissage
    void PrepareForGame();

    // Pour le rendu
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    // Réglages de la vue initiale
    void SetDefaultZoom(float zoom); //
    void SetDefaultCenter(const sf::Vector2f &center); //
    float GetDefaultZoom() const; //
    const sf::Vector2f &GetDefaultCenter() const; //

    /* Accesseurs */
    // Etat
    bool IsCharged() const; //
    void SetCharged(bool charged); //

    // Player
    void SetPlayer(Player *player); //
    Player *GetPlayer(); //
    void MakePlayerSpeak(const std::string &msg, float time); //

    // Couleur de fond
    void SetBckgColor(const sf::Color &color); //
    sf::Color const &GetBckgColor() const; //

    // TODO
    void SetScriptMachine(ScriptMachine *machine);

private:
    bool mIsCharged;

    // Monde
    b2Vec2 mGravity;
    PhysicManager &mPhysicMgr;

    // Entities
    EntityManager &mEntityManager;
    PartitioningTree &mPartitioningTree;

    // Triggers
    TriggersManager &mTriggersManager;

    // Config de la fenêtre de rendu
    sf::Color mBckgC;

    // Vue initiale
    float mDefaultZoom;
    sf::Vector2f mDefaultCenter;

    // Textures
    ResourceManager &mResourceManager;

    // Joueur
    Player *mPlayer;
};

