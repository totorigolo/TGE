#include "Hull.h"

// Ctor & Dtor
Hull::Hull(Body* body, bool isStatic, bool isActivated)
	: mBody(body), mIsStatic(isStatic), mIsActivated(isActivated)
{
	mStates.blendMode = sf::BlendNone;
}
Hull::~Hull()
{
}

// Gestion des propriétés
bool Hull::IsStatic() const
{
	return mIsStatic;
}
void Hull::SetStatic(bool isStatic)
{
	mIsStatic = isStatic;
}
bool Hull::IsActivated() const
{
	return mIsActivated;
}
void Hull::Activate()
{
	mIsActivated = true;
}
void Hull::Deactivate()
{
	mIsActivated = false;
}
void Hull::SetActivated(bool activated)
{
	mIsActivated = activated;
}

// Gestion des lampes
void Hull::AddLight(Light* light)
{
	// Regarde s'il n'y a pas une place de libre
	unsigned int index = 0U;
	bool found = false;
	for (unsigned int i = 0; i < mLights.size() && !found; ++i)
	{
		// Supprime la lampe
		if (!mLights[i])
		{
			index = i;
			mLights[i] = light;
			found = true;
		}
	}

	// Si on a pas trouvé de place, on l'ajoute à la fin
	if (!found)
	{
		mLights.push_back(light);
		index = mLights.size() - 1U;
	}

	InitializeVectors(index, !found);
}
void Hull::RemoveLight(Light* light)
{
	for (unsigned int i = 0; i < mLights.size(); ++i)
	{
		// Supprime la lampe
		if (mLights[i] == light)
		{
			mLights[i] = nullptr;
			ClearVectors(i);
		}
	}
}

// Gestion des conteneurs
void Hull::InitializeVectors(unsigned int lightID, bool isNew)
{
	// Redimmentionne les conteneurs
	if (isNew)
	{
		mInitialized.resize(mInitialized.size() + 1U, false);
		mIsNear.resize(mIsNear.size() + 1U, false);
	}
}
void Hull::ClearVectors(unsigned int lightID)
{
	// Vide les infos de cette lampe
	mInitialized[lightID] = false;
	mIsNear[lightID] = false;
}
