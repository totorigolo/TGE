#pragma once

#include "PolyChain.h"
#include "../Tools/vector_wrappers.h"

namespace EntityFactory {
    // Charge une texture
    void LoadTexture(const std::string &name, const std::string &path);

    // Crée une déco
    void CreateDeco(const b2Vec3 &posRot, const std::string &texture, int layer = 1);

    // Crée un BasicBody : Box
    void CreateBox(const b2Vec3 &posRot, b2BodyType type, const std::string &texture, int layer = 1);

    // Crée un BasicBody : Circle
    void CreateCircle(const b2Vec3 &posRot, b2BodyType type, const std::string &texture, int layer = 1);

    // Crée un PolyBody
    void CreatePolyBody(const vector_b2Vec2 &vectices, b2BodyType type, const std::string &texture, int layer = 1);

    // Crée un PolyChain
    void
    CreatePolyChain(const vector_b2Vec2 &vectices, PolyChain::Type type, const std::string &texture, int layer = 1);

    // Crée une PointLight
    void
    CreatePointLight(const b2Vec2 &pos, int lightRadius = 256, sf::Color lightColor = sf::Color::White, int layer = 1);

    // Crée un Ragdoll
    void CreateRagdoll(const b2Vec2 &position, int layer = 1);

    // Crée un Hum
    void CreateHum(b2Vec2 pos, double age, double strengh, sf::Color color = sf::Color(35, 35, 35), int layer = 1);

    // Crée un Player
    void CreatePlayer(b2Vec2 pos, double age, double strengh, sf::Color color = sf::Color(64, 0, 42));
}
