#pragma once
#include <SFML/Graphics.hpp>
#include "Light.h"

class Body;
class Hull
{
public:
	// Ctor & Dtor
	Hull(Body* body, bool isStatic, bool isActivated = true);
	virtual ~Hull();
	
	// Gestion des propriétés
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
	virtual void InitializeVectors(unsigned int lightID, bool isNew);
	virtual void ClearVectors(unsigned int lightID);

	// Mise à jour
	virtual void Update() = 0;

	// Accesseurs
	Body* GetBody() { return mBody; }

protected:
	// Propriétés
	bool mIsStatic;
	bool mIsActivated;
	sf::RenderStates mStates;

	// Lights
	std::vector<Light*> mLights;
	std::vector<unsigned char> mInitialized; // Ce Hull a-t-il déjà été calculé pour la lampe ?
	std::vector<unsigned char> mIsNear; // La lampe est elle proche ? / Doit-on affiche ce Hull ?

	// Body associé
	Body* mBody;

private:
};
