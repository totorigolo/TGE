#pragma once
#include "../Tools/Singleton.h"
#include "Light.h"
#include "Hull.h"
#include <list>

class LightManager : public Singleton<LightManager>, public sf::Drawable
{
public:
	// Ctor & Dtor
	LightManager();
	virtual ~LightManager();
	
	// Gestion de l'affichage
	void Initialize(const sf::RenderTarget& target, sf::View* view);
	void Resize(const sf::RenderTarget& target);

	// Gestion de la vue
	void SetView(sf::View* view);
	const sf::View* GetView() const;

	// Mise à jour
	void Update();

	// Gestion des lumières
	// NE JAMAIS SUPPRIMER DES LUMIERS HORS DE DeleteLight(); !!
	void AddLight(Light* light);
	void DeleteLight(Light* light, bool _delete = true);
	void DeleteAllLights();

	// Gestion des objets
	void AddHull(Hull* hull);
	void DeleteHull(Hull* hull, bool _delete = true);
	void DeleteAllHulls();
	
	// MARCHE
	const sf::Texture& GetTexture() { return mRenderTexture.getTexture(); }

protected:
	// MARCHE PAS !!
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	// Etat
	bool mIsInitialized;

	// Rendu
	sf::View* mView;
	sf::RenderTexture mRenderTexture;
	sf::Sprite mRenderTextureSprite;
	sf::RectangleShape mObscurity;
	sf::RenderStates mLightStates;
	sf::RenderStates mShadowStates;
	
	// Lumières
	std::list<Light*> mLights;

	// Obstacles
	std::list<Hull*> mHulls;
};
