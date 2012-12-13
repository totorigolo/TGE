#include "CircleHull.h"
#include "../utils.h"
#include "../Tools/Segment.h"
#include <iostream>

// Ctor & Dtor
CircleHull::CircleHull(Body* body, bool isStatic, bool isActivated)
	: Hull(body, isStatic, isActivated)
{
	// Récupère les shapes du body
	for (b2Fixture *f = mBody->GetBody()->GetFixtureList(); f; f = f->GetNext())
	{
		mShapes.push_back((b2CircleShape*) f->GetShape());
	}

	// Redimentionne les tableaux
	mCenter.resize(mShapes.size());
	mRadius.resize(mShapes.size(), 0.f);
}
CircleHull::~CircleHull()
{
}

// Gestion des conteneurs
void CircleHull::InitializeVectors(unsigned int lightID, bool isNew)
{
	Hull::InitializeVectors(lightID, isNew);

	// Redimmentionne les conteneurs
	if (isNew)
	{
		mOriginCircle.resize(mOriginCircle.size() + 1U);
		mLinkingPoly.resize(mLinkingPoly.size() + 1U);
		
		sf::ConvexShape polyS;
		polyS.setFillColor(sf::Color(0, 0, 0, 200));
		sf::CircleShape cS;
		cS.setFillColor(sf::Color(0, 0, 0, 200));
		mLinkingPoly[lightID].resize(mShapes.size(), polyS);
		mOriginCircle[lightID].resize(mShapes.size(), cS);
	}
}
void CircleHull::ClearVectors(unsigned int lightID)
{
	Hull::ClearVectors(lightID);

	// Vide les polys de cette lampe
	mLinkingPoly[lightID].clear();
	mOriginCircle[lightID].clear();
}

// Mise à jour
void CircleHull::Update()
{
	if (!mIsActivated)
		return;

	unsigned int iShape = 0U;
	for (std::vector<b2CircleShape*>::iterator shape = mShapes.begin(); shape < mShapes.end(); ++shape, ++iShape)
	{
		// Centre et rayon du cercle
		if (!mIsStatic || mHasChanged)
		{
			mRadius[iShape] = (*shape)->m_radius * mBody->GetWorld()->GetPPM();
			mCenter[iShape] = b22sfVec(mBody->GetBody()->GetWorldPoint((*shape)->m_p), mBody->GetWorld()->GetPPM());
		}

		unsigned int iLight = 0U;
		for (std::vector<Light*>::iterator light = mLights.begin(); light < mLights.end(); ++light, ++iLight)
		{
			sf::Vector2f lightPos = (*light)->GetPosition();
			float lightRadius = (*light)->GetRadius();

			// Vérifie que la lumière n'est pas cachée
			if ((*light)->IsHiden())
				continue;

			// Vérifie que la lumière touche ce shape
			mIsNear[iLight] = (*light)->GetAABB().intersects(mBody->GetSprite()->getGlobalBounds());

			// Si on doit recalculer les ombres
			if (mIsNear[iLight] && (!mIsStatic || !mHasChanged || !(*light)->IsStatic() || !mInitialized[iLight]))
			{
				// Positionne l'ombre du cercle
				mOriginCircle[iShape][iLight].setPosition(mCenter[iShape]);
				mOriginCircle[iShape][iLight].setRadius(mRadius[iShape]);
				mOriginCircle[iShape][iLight].setOrigin(mRadius[iShape], mRadius[iShape]);

				// Calcule l'ombre
				// Vecteur lumière -> centre
				sf::Vector2f u = mCenter[iShape] - lightPos;
				float dist = sqrt(u.x * u.x + u.y * u.y);

				// On regarde si la lumière n'est pas dans le cercle
				if (dist < mRadius[iShape])
				{
					(*light)->IsHiden(true);
					continue;
				}

				// Vecteur parallèle à u
				sf::Vector2f v(- 1.f * u.y, 1.f * u.x);
				v /= sqrt(v.x * v.x + v.y * v.y);
				v *= mRadius[iShape];

				// Points extrèmes sur le cercle
				sf::Vector2f ptE1 = mCenter[iShape] + v;
				sf::Vector2f ptE2 = mCenter[iShape] - v;
				
				// Points projetés
				sf::Vector2f ptEE1 = ptE1 + ((ptE1 - lightPos) / dist * lightRadius);
				sf::Vector2f ptEE2 = ptE2 + ((ptE2 - lightPos) / dist * lightRadius);

				// Création du poly de l'ombre
				mLinkingPoly[iShape][iLight].setPointCount(4U);
				mLinkingPoly[iShape][iLight].setPoint(0U, ptE1);
				mLinkingPoly[iShape][iLight].setPoint(1U, ptE2);
				mLinkingPoly[iShape][iLight].setPoint(2U, ptEE2);
				mLinkingPoly[iShape][iLight].setPoint(3U, ptEE1);
			}
			mInitialized[iLight] = true;
		}
	}
	mHasChanged = false;
}

void CircleHull::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsActivated)
	{
		// Affiche les ombres
		for (unsigned int iL = 0u; iL < mLights.size(); ++iL)
		{
			if (mIsNear[iL])
			{
				for (unsigned int iS = 0u; iS < mShapes.size(); ++iS)
				{
					target.draw(mLinkingPoly[iL][iS], states);
					target.draw(mOriginCircle[iL][iS], states);
				}
			}
		}
	}
}
