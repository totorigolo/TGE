#include "CircleHull.h"
#include "../Tools/utils.h"
#include "../Tools/Segment.h"
#include <iostream>

// Ctor & Dtor
CircleHull::CircleHull(Body* body, bool isStatic, bool isActivated)
	: Hull(body, isStatic, isActivated)
{
	// R�cup�re les shapes du body
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

// Mise � jour
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

			// V�rifie que la lumi�re n'est pas cach�e
			if ((*light)->IsHidden())
			{
				continue;
			}

			// V�rifie que la lumi�re touche ce shape
			mIsNear[iLight] = (*light)->GetAABB().intersects(mBody->GetSprite()->getGlobalBounds());

			// Si on doit recalculer les ombres
			if (mIsNear[iLight] && (!mIsStatic || !mHasChanged || !(*light)->IsStatic() || !mInitialized[iLight]))
			{
				// Positionne l'ombre du cercle
				mOriginCircle[iLight][iShape].setPosition(mCenter[iShape]);
				mOriginCircle[iLight][iShape].setRadius(mRadius[iShape]);
				mOriginCircle[iLight][iShape].setOrigin(mRadius[iShape], mRadius[iShape]);

				// Calcule l'ombre
				// Vecteur lumi�re -> centre
				sf::Vector2f u = mCenter[iShape] - lightPos;
				float dist = sqrt(u.x * u.x + u.y * u.y);

				// On regarde si la lumi�re n'est pas dans le cercle
				if (dist < mRadius[iShape])
				{
					// La lumi�re est cach� si un objet autre que l'�metteur la cache
					if ((*light)->GetEmitter() != mBody)
						(*light)->IsHidden(true);
					continue;
				}

				// Vecteur parall�le � u
				sf::Vector2f v(- 1.f * u.y, 1.f * u.x);
				v /= sqrt(v.x * v.x + v.y * v.y);
				v *= mRadius[iShape];

				// Points extr�mes sur le cercle
				sf::Vector2f ptE1 = mCenter[iShape] + v;
				sf::Vector2f ptE2 = mCenter[iShape] - v;
				
				// Points projet�s
				sf::Vector2f ptEE1 = ptE1 + ((ptE1 - lightPos) / dist * lightRadius);
				sf::Vector2f ptEE2 = ptE2 + ((ptE2 - lightPos) / dist * lightRadius);

				// Cr�ation du poly de l'ombre
				mLinkingPoly[iLight][iShape].setPointCount(4U);
				mLinkingPoly[iLight][iShape].setPoint(0U, ptE1);
				mLinkingPoly[iLight][iShape].setPoint(1U, ptE2);
				mLinkingPoly[iLight][iShape].setPoint(2U, ptEE2);
				mLinkingPoly[iLight][iShape].setPoint(3U, ptEE1);

				mInitialized[iLight] = true;
			}
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
