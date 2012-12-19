#include "LightManager.h"
#include "../Tools/utils.h"

// Ctor & Dtor
LightManager::LightManager()
	: mIsInitialized(false), mView(nullptr)
{
	// Règle l'état d'affichage
	mShadowStates.blendMode = sf::BlendAlpha;
	mLightStates.blendMode = sf::BlendAlpha;

	// Réglage des objets
	mObscurity.setFillColor(sf::Color(0, 0, 0, 200));
}
LightManager::~LightManager()
{
}

// Gestion de l'affichage
void LightManager::Initialize(const sf::RenderTarget& target, sf::View* view)
{
	SetView(view);
	Resize(target);

	// La classe est initialisée
	mIsInitialized = true;
}
void LightManager::Resize(const sf::RenderTarget& target)
{
	// Création de la RenderTexture aux nouvelles dimensions
	mRenderTexture.create(target.getSize().x, target.getSize().y);

	// Mise à jour du sprite
	mRenderTextureSprite.setTexture(mRenderTexture.getTexture());

	// Réglage des objets
	mObscurity.setSize(mView->getSize());
	mObscurity.setOrigin(mObscurity.getSize() / 2.f);
}

// Gestion de la vue
void LightManager::SetView(sf::View* view)
{
	mView = view;
	mRenderTexture.setView(*mView);
}
const sf::View* LightManager::GetView() const
{
	return mView;
}

// Mise à jour
void LightManager::Update()
{
	/* Crée les ombres */
	mRenderTexture.clear(sf::Color(0, 0, 0, 0));

	// Mise à jour des objets
	mObscurity.setSize(mView->getSize());
	mObscurity.setOrigin(mObscurity.getSize() / 2.f);
	mObscurity.setPosition(mView->getCenter());

	// Affichage du masque d'obscurité
	mRenderTexture.draw(mObscurity, mShadowStates);
	
	// Mise à jour
	for (std::list<Light*>::iterator it = mLights.begin(); it != mLights.end(); ++it)
	{
		(*it)->UpdatePosition();
		(*it)->RenderLight();
	}
	for (std::list<Hull*>::iterator it = mHulls.begin(); it != mHulls.end(); ++it)
	{
		(*it)->Update();
	}

	// Affchage
	for (std::list<Light*>::iterator it = mLights.begin(); it != mLights.end(); ++it)
	{
		(*it)->DisplayTexture();
		mRenderTexture.draw(**it, mLightStates);
	}

	// Finalisation du rendu
	mRenderTexture.display();
}

// Gestion des lumières
void LightManager::AddLight(Light* light)
{
	mLights.push_back(light);

	for (std::list<Hull*>::iterator it = mHulls.begin(); it != mHulls.end(); ++it)
	{
		(*it)->AddLight(light);
	}
}
void LightManager::DeleteLight(Light* light, bool _delete)
{
	for (std::list<Hull*>::iterator it = mHulls.begin(); it != mHulls.end(); ++it)
	{
		(*it)->RemoveLight(light);
	}

	mLights.remove(light);

	if (_delete)
		delete light;
}
void LightManager::DeleteAllLights()
{
	Light *l = nullptr;
	for (std::list<Light*>::iterator it = mLights.begin(); it != mLights.end(); )
	{
		for (std::list<Hull*>::iterator ith = mHulls.begin(); ith != mHulls.end(); ++ith)
		{
			(*ith)->RemoveLight(*it);
		}

		l = *it;
		it = mLights.erase(it);
		delete l;
	}
}

// Gestion des objets
void LightManager::AddHull(Hull* hull)
{
	mHulls.push_back(hull);
	
	for (std::list<Light*>::iterator it = mLights.begin(); it != mLights.end(); ++it)
	{
		hull->AddLight(*it);
	}
}
void LightManager::DeleteHull(Hull* hull, bool _delete)
{
	if (hull)
	{
		mHulls.remove(hull);

		if (_delete)
		{
			delete hull;
		}
	}
}
void LightManager::DeleteAllHulls()
{
	Hull *h = nullptr;
	for (std::list<Hull*>::iterator it = mHulls.begin(); it != mHulls.end(); )
	{
		h = *it;
		it = mHulls.erase(it);
		delete h;
	}
}

// Affichage
void LightManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(mRenderTextureSprite, states);
}
