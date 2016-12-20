#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "../Tools/Singleton.h"

class PointLight;
class LightEngine : public Singleton<LightEngine>
{
protected:
	friend class Singleton<LightEngine>;

	// Ctor & dtor
	LightEngine(void);
	virtual ~LightEngine(void);

public:
	// Charge les shaders
	void LoadShaders(void);

	// Dessiner les obstables
	void DrawPhysicalHull(PointLight *light, const b2Body& body);
	void Clear(PointLight *light);

	// Crée les ombres
	void CreateShadows(PointLight *light);

	// Gère l'état du LightEngine
	void Activate();
	void Deactivate();
	void SetActive(bool state);
	bool IsActive() const;

private:
	// Accès de PointLight
	friend class PointLight;

	// State of the LightEngine
	bool mIsActive;

	// States
	sf::RenderStates addStates;

	// Shader
	sf::Shader mShadowShader;

	// sf::Vector particuliers pour le ShadowVertexArray
	/* L'idée est que quand on dessine les ombres, on mets les coordonnées de texture à 0, alors que
	 * le fond dessiné sous les ombres sur toute la surface de la lumière a ses coordonnées à 1.
	 * De cette façon on sait très facilement si on est dans une ombre ou non.
	 */
	sf::Vector2f mZero;
	sf::Vector2f mOne;
};

