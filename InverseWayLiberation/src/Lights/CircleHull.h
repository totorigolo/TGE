#pragma once
#include "Light.h"
#include "Hull.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>

class CircleHull : public Hull
{
public:
	// Ctor & Dtor
	CircleHull(Body* body, bool isStatic, bool isActivated = true);
	virtual ~CircleHull();
	
	// Gestion des conteneurs
	virtual void InitializeVectors(unsigned int lightID, bool isNew);
	virtual void ClearVectors(unsigned int lightID);

	// Mise à jour
	virtual void Update();

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	/*
	** Les polygones sont sauvegardés pour accélérer le rendu.
	**
	** Pour les formes :
	**   - le 1er vector est pour la lumière.
	**   - le 2nd vector est pour le shape.
	**
	** Pour les propriétés, c'est pour le shape.
	*/

	// Shapes du body
	std::vector<b2CircleShape*> mShapes;
	
	// Propriétés
	std::vector<sf::Vector2f> mCenter;
	std::vector<float> mRadius;

	// Formes
	std::vector<std::vector<sf::ConvexShape>> mLinkingPoly;
	std::vector<std::vector<sf::CircleShape>> mOriginCircle;
};
