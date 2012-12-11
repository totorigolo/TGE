#include "LightManager.h"
#include "../utils.h"

// Ctor & Dtor
LightManager::LightManager()
	: mIsInitialized(false), mView(nullptr)
{
	// R�gle l'�tat d'affichage
	mShadowStates.blendMode = sf::BlendNone;

	// R�glage des objets
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

	// La classe est initialis�e
	mIsInitialized = true;
}
void LightManager::Resize(const sf::RenderTarget& target)
{
	// Cr�ation de la RenderTexture aux nouvelles dimensions
	mRenderTexture.create(target.getSize().x, target.getSize().y);

	// Mise � jour du sprite
	mRenderTextureSprite.setTexture(mRenderTexture.getTexture());

	// R�glage des objets
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

// Mise � jour
void LightManager::Update()
{
	/* Cr�e les ombres */
	mRenderTexture.clear(sf::Color(0, 0, 0, 0));
	mRenderTexture.setView(*mView);

	// Mise � jour des objets
	mObscurity.setSize(mView->getSize());
	mObscurity.setOrigin(mObscurity.getSize() / 2.f);
	mObscurity.setPosition(mView->getCenter());

	// Affichage du masque d'obscurit�
	mRenderTexture.draw(mObscurity, mShadowStates);
	
	// Affchage des lumi�res
	for (std::list<Light*>::iterator it = mLights.begin(); it != mLights.end(); ++it)
	{
		(*it)->Update();
		mRenderTexture.draw(**it, mShadowStates);
	}

	// Affchage des ombres
	for (std::list<Hull*>::iterator it = mHulls.begin(); it != mHulls.end(); ++it)
	{
		(*it)->Update();
		mRenderTexture.draw(**it, mShadowStates);
	}

	// Finalisation du rendu
	mRenderTexture.display();
}

// Gestion des lumi�res
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
	mHulls.remove(hull);

	if (_delete)
		delete hull;
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
