#pragma once

#include <Box2D/Box2D.h>
#include "Entity.h"

class LightEngine;

struct TextureHolder;

class PointLight : public Entity {
public:
    // Ctor & dtor
    PointLight(int lightRadius = 256, sf::Color lightColor = sf::Color::White, int layer = 10);

    virtual ~PointLight(void);

    // Gestion de la position
    void SetPosition(const b2Vec2 &pos);

    void SetPosition(const sf::Vector2f &pos);

    void Move(const b2Vec2 &off);

    void Move(const sf::Vector2f &off);

    const b2Vec2 GetPosition(void) const;

    const sf::Vector2f &GetPosition_sf(void) const;

    // Mise à jour
    void Update(void);

    // Crée les textures
    void CreateTextures(void);

    // Retourne les ombres
    const sf::VertexArray &GetVertexArray(void) const;

    // Gère la couleur de la lumière
    sf::Color GetLightColor(void) const;

    void SetLightColor(const sf::Color &lightColor);

    // Gère la taille de la lampe
    unsigned int GetLightRadius(void) const;

    void SetLightRadius(unsigned int radius);

    // Récupère la boîte englobante
    sf::FloatRect GetBoundingBox(void) const;

    // Accesseurs
    unsigned int GetID() const;

protected:
    // Accès du LightEngine
    friend class LightEngine;

    // Vue
    sf::View view;

    // Géométrie des ombres
    sf::VertexArray mShadowsVertexArray;

    // Lumière
    sf::RenderTexture mLightTex;
    sf::Sprite mLightSprite;

private:
    friend class LightManager;

    // Identifiant (uniques)
    unsigned int mID;

protected:
    // Rendu
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    // Propriétés
    bool mUpdate;

    // LightEngine
    LightEngine &mEngine;

    // Position
    sf::Vector2f mPosition;

    // Propriétés de la lumières
    sf::Color mLightColor;
    unsigned int mLightRadius;
};
