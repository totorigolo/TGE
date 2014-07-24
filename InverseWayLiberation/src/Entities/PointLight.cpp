#include "stdafx.h"
#include "PointLight.h"
#include "../Graphics/LightEngine.h"
#include "PartitioningTree.h"
#include "Hull.h"
#include "Physics/PhysicManager.h"

// Ctor
PointLight::PointLight(int lightRadius, sf::Color lightColor, int layer)
	: Entity(layer), mEngine(LightEngine::GetInstance()),
	mLightColor(lightColor), mLightRadius(lightRadius), mUpdate(true)
{
	myAssert(mLightRadius > 0, "Light radius must be > 0");

	// Change le type
	mType = EntityType::PointLight;

	// Cr�e les textures
	CreateTextures();

	mIsAlive = true;
}

// Dtor
PointLight::~PointLight(void)
{
}

// Gestion de la position
void PointLight::SetPosition(const b2Vec2& pos)
{
	SetPosition(b22sfVec(pos, PhysicManager::GetInstance().GetPPM()));
}
void PointLight::SetPosition(const sf::Vector2f& pos)
{
	mPosition = pos;
	view.setCenter(mPosition);
	mRenderSprite.setPosition(mPosition - (u2f(shadowTex.getSize()) / 2.f));
	mUpdate = true;
}
void PointLight::Move(const b2Vec2& off)
{
	Move(b22sfVec(off, PhysicManager::GetInstance().GetPPM()));
}
void PointLight::Move(const sf::Vector2f& off)
{
	mPosition += off;
	view.setCenter(mPosition);
	mRenderSprite.setPosition(mPosition - (u2f(shadowTex.getSize()) / 2.f));
	mUpdate = true;
}
const b2Vec2& PointLight::GetPosition(void) const
{
	return sf2b2Vec(mPosition, PhysicManager::GetInstance().GetMPP());
}
const sf::Vector2f& PointLight::GetPosition_sf(void) const
{
	return mPosition;
}

// Mise � jour
void PointLight::Update(void)
{
	// Met � jour les ombres si besoin
	if (mIsAlive)
	{
		bool hullMoved = false;

		// R�cup�re un petit nom pour le PartitioningTree
		PartitioningTree &pt = PartitioningTree::GetInstance();

		/*// Regarde si au moins un hull a boug� dans le p�rim�tre de la lumi�re, et dessine les shadow casters
		pt.ApplyOnHulls(this->GetBoundingBox(), [this, &hullMoved](Hull *h) {
			if (h->HasMoved())
				hullMoved = true;

			if (h->IsDrawable())
				LightEngine::GetInstance().DrawHull(this, *h->GetShadowCaster());
			else if (h->IsPhysicallyDrawable())
				LightEngine::GetInstance().DrawPhysicalHull(this, *h->GetBodyShadowCaster());
		}, &hullMoved);*/

		// Regarde si au moins un hull a boug� dans le p�rim�tre de la lumi�re, et dessine les shadow casters
		hullMoved = pt.HasMovedIn(this->GetBoundingBox());

		// Si les ombres ne sont plus � jour
		if (mUpdate || hullMoved)
		{
			// Regarde si au moins un hull a boug� dans le p�rim�tre de la lumi�re, et dessine les shadow casters
			pt.ApplyOnHulls(this->GetBoundingBox(), [this, &hullMoved](Hull *h) {
				if (h->IsDrawable())
					LightEngine::GetInstance().DrawHull(this, *h->GetShadowCaster());
				else if (h->IsPhysicallyDrawable())
					LightEngine::GetInstance().DrawPhysicalHull(this, *h->GetBodyShadowCaster());
			});

			// Cr�e et affiche les ombres
			mEngine.CreateShadows(this);
		}
		else
		{
			// Efface la caster texture
			LightEngine::GetInstance().Clear(this);
		}

		mUpdate = false;
	}
}

// Retourne la texture ombr�e
const sf::Texture& PointLight::GetShadowTexture(void) const
{
	return shadowTex.getTexture();
}
const sf::Sprite& PointLight::GetShadowSprite(void) const
{
	return mRenderSprite;
}

// G�re la couleur de la lumi�re
sf::Color PointLight::GetLightColor(void) const
{
	return mLightColor;
}
void PointLight::SetLightColor(const sf::Color& lightColor)
{
	mLightColor = lightColor;
}

// G�re la taille de la lampe
unsigned int PointLight::GetLightRadius(void) const
{
	return mLightRadius;
}
void PointLight::SetLightRadius(unsigned int radius)
{
	mLightRadius = radius;
	CreateTextures();
}

// R�cup�re la bo�te englobante
sf::FloatRect PointLight::GetBoundingBox(void) const
{
	sf::FloatRect rect;
	rect.left = GetPosition_sf().x - mLightRadius;
	rect.top = GetPosition_sf().y - mLightRadius;
	rect.width = static_cast<float>(mLightRadius * 2);
	rect.height = static_cast<float>(mLightRadius * 2);

	return rect;
}

// Accesseurs
unsigned int PointLight::GetID() const
{
	return mID;
}

// Cr�e les textures
void PointLight::CreateTextures(void)
{
	// ShadowTexture
	shadowTex.create((int) this->GetBoundingBox().width, (int) this->GetBoundingBox().height);
	shadowTex.setSmooth(true);
	shadowSprite.setTexture(shadowTex.getTexture(), true);
	mRenderSprite.setTexture(shadowTex.getTexture(), true);

	// Obtient les textures de l'engine
	textures = mEngine.GetTextures(std::make_pair((int) this->GetBoundingBox().width, (int) this->GetBoundingBox().height));

	// R�gle la vue
	view = textures->casterTex.getDefaultView();

	// Initialise l'�tat OpenGL, pour �viter le reset plus tard
	textures->casterTex.resetGLStates();
}

// Rendu
void PointLight::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!mIsAlive) return;

	// Dessine la lumi�re avec les ombres
	target.draw(mRenderSprite, mEngine.addStates);
}
