#include "stdafx.h"
#include "PointLight.h"
#include "../Graphics/LightEngine.h"
#include "PartitioningTree.h"
#include "Hull.h"
#include "../Physics/PhysicManager.h"

// Ctor
PointLight::PointLight(int lightRadius, sf::Color lightColor, int layer)
	: Entity(layer), mEngine(LightEngine::GetInstance()),
	mLightColor(lightColor), mLightRadius(lightRadius), mUpdate(true)
{
	myAssert(mLightRadius > 0, "Light radius must be > 0");

	// Change le type
	mType = EntityType::PointLight;

	// Crée les textures
	CreateTextures();

	// Configure le VertexArray
	mShadowsVertexArray.setPrimitiveType(sf::PrimitiveType::Quads);

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
	mLightSprite.setPosition(mPosition - (u2f(mLightTex.getSize()) / 2.f));
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
	mLightSprite.setPosition(mPosition - (u2f(mLightTex.getSize()) / 2.f));
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

// Mise à jour
void PointLight::Update(void)
{
	// Met à jour les ombres si besoin
	if (mIsAlive && mEngine.IsActive())
	{
		bool hullMoved = false;

		// Récupère un petit nom pour le PartitioningTree
		PartitioningTree &pt = PartitioningTree::GetInstance();

		// Regarde si au moins un hull a bougé dans le périmètre de la lumière, et dessine les shadow casters
		if (!mUpdate)
			hullMoved = pt.HasMovedIn(this->GetBoundingBox());

		// Si les ombres ne sont plus à jour
		if (mUpdate || hullMoved)
		{
			mShadowsVertexArray.clear();
			mShadowsVertexArray.resize(4);
			auto bb = GetBoundingBox();
			const sf::Color &b = sf::Color::Blue;
			mShadowsVertexArray[0] = sf::Vertex(sf::Vector2f(bb.left, bb.top), b, mEngine.mOne);
			mShadowsVertexArray[1] = sf::Vertex(sf::Vector2f(bb.left + bb.width, bb.top), b, mEngine.mOne);
			mShadowsVertexArray[2] = sf::Vertex(sf::Vector2f(bb.left + bb.width, bb.top + bb.height), b, mEngine.mOne);
			mShadowsVertexArray[3] = sf::Vertex(sf::Vector2f(bb.left, bb.top + bb.height), b, mEngine.mOne);

			// Regarde si au moins un hull a bougé dans le périmètre de la lumière, et dessine les shadow casters
			pt.ApplyOnHulls(this->GetBoundingBox(), [this, &hullMoved](Hull *h) {
				if (h->IsPhysicallyDrawable())
					mEngine.DrawPhysicalHull(this, *h->GetBodyShadowCaster());
			});

			// Crée et affiche les ombres
			mEngine.CreateShadows(this);
		}
		else
		{
			// Efface la caster texture // TODO : Euh...
			mEngine.Clear(this);
		}

		mUpdate = false;
	}
}

// Crée les textures
void PointLight::CreateTextures(void)
{
	// LightTexture
	auto bb = this->GetBoundingBox();
	mLightTex.create((int) bb.width, (int) bb.height);
	mLightTex.setSmooth(true);
	mLightSprite.setTexture(mLightTex.getTexture(), true);

	// Règle la vue
	view = mLightTex.getDefaultView();
}

// Retourne la texture ombrée
const sf::VertexArray& PointLight::GetVertexArray(void) const
{
	return mShadowsVertexArray;
}

// Gère la couleur de la lumière
sf::Color PointLight::GetLightColor(void) const
{
	return mLightColor;
}
void PointLight::SetLightColor(const sf::Color& lightColor)
{
	mLightColor = lightColor;
	mUpdate = true;
}

// Gère la taille de la lampe
unsigned int PointLight::GetLightRadius(void) const
{
	return mLightRadius;
}
void PointLight::SetLightRadius(unsigned int radius)
{
	mUpdate = true;
	mLightRadius = radius;
	CreateTextures();
	view.setCenter(mPosition);
	mLightSprite.setPosition(mPosition - (u2f(mLightTex.getSize()) / 2.f));
}

// Récupère la boîte englobante
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

// Rendu
void PointLight::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!mIsAlive || !mEngine.IsActive()) return;

	// Dessine la lumière avec les ombres
	target.draw(mLightSprite, mEngine.addStates);
}
