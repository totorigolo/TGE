#pragma once
#include <SFML/Graphics.hpp>
#include "../Physics/Bodies/Body.h"
#include "Light.h"

class Body;
class Hull : public sf::Drawable
{
public:
	// Ctor & Dtor
	Hull(Body* body, bool isStatic, bool isActivated = true);
	virtual ~Hull();
	
	// Gestion des propri�t�s
	bool IsStatic() const;
	void SetStatic(bool isStatic);
	bool IsActivated() const;
	void Activate();
	void Deactivate();
	void SetActivated(bool activated);
	
	// Gestion des lampes
	virtual void AddLight(Light* light);
	virtual void RemoveLight(Light* light);
	
	// Gestion des conteneurs
	virtual void InitializeVectors(unsigned int lightID);
	virtual void ClearVectors(unsigned int lightID);

	// Mise � jour
	virtual void Update() = 0;

	// Accesseurs
	Body* GetBody() { return mBody; }

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
	
	// Propri�t�s
	bool mIsActivated;
	bool mIsStatic;
	bool mHasChanged;

	// Lights
	std::vector<Light*> mLights;
	std::vector<unsigned char> mInitialized; // Ce Hull a-t-il d�j� �t� calcul� pour la lampe ?
	std::vector<unsigned char> mIsNear; // La lampe est elle proche ? / Doit-on affiche ce Hull ?

	// Body associ�
	Body* mBody;

private:
};
