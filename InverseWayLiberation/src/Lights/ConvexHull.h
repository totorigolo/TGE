#pragma once
#include "Light.h"
#include "Hull.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>

class ConvexHull : public Hull
{
public:
	// Ctor & Dtor
	ConvexHull(Body* body, bool isStatic, bool isActivated = true);
	virtual ~ConvexHull();
	
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
	** Pour les polygones :
	**   - le 1er vector est pour la lumière.
	**   - le 2nd vector est pour le shape.
	**
	** Pour mPoints : le premier vecteur est le shape.
	*/

	// Shapes du body
	std::vector<b2PolygonShape*> mShapes;

	// Polygons
	std::vector<std::vector<sf::ConvexShape>> mLinkingPoly;
	std::vector<std::vector<sf::ConvexShape>> mProjectionPoly;
	
	// Positions des points
	std::vector<std::vector<sf::Vector2f>> mPoints;
	std::vector<unsigned char> mHidedPoints;
	std::vector<sf::Vector2f> mCastedPoints;

	// Points extrèmes
	unsigned int mExtremPoint1;
	unsigned int mExtremPoint2;

};
