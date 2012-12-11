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
	
	// Gestion des lampes
	void AddLight(Light* light);
	void RemoveLight(Light* light);

	// Gestion des conteneurs
	virtual void InitializeVectors(unsigned int lightID);
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
	**   - le 1er vector est pour la lumière, et existe que si elle est statique, // TODO: le "si elle est statique
	**   - le 2nd vector est pour le shape, et existe que si ce hull est statique.
	*/

	// Shapes du body
	std::vector<b2PolygonShape*> mShapes;

	// Polygons
	std::vector<std::vector<sf::ConvexShape>> mLinkingPoly;
	std::vector<std::vector<sf::ConvexShape>> mProjectionPoly;

	// Ces attributs sont là juste pour optimiser : pas besoin de les créer à chaque Update()
	// Positions des points
	std::vector<sf::Vector2f> mPoints;
	std::vector<unsigned char> mHidedPoints;
	std::vector<sf::Vector2f> mCastedPoints;

	// Points extrèmes
	unsigned int mExtremPoint1;
	unsigned int mExtremPoint2;

};
