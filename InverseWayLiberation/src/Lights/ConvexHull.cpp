#include "ConvexHull.h"
#include "../utils.h"
#include "../Tools/Segment.h"
#include <iostream>

// Ctor & Dtor
ConvexHull::ConvexHull(Body* body, bool isStatic, bool isActivated)
	: Hull(body, isStatic, isActivated)
{
	// Récupère les shapes du body
	for (b2Fixture *f = mBody->GetBody()->GetFixtureList(); f; f = f->GetNext())
	{
		mShapes.push_back((b2PolygonShape*) f->GetShape());
	}

	// Réserve la place
	mPoints.reserve(8U);
	mHidedPoints.reserve(8U);
	mCastedPoints.reserve(8U);
}
ConvexHull::~ConvexHull()
{
}

// Gestion des lampes
void ConvexHull::AddLight(Light* light)
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

	// Redimmentionne les conteneurs
	InitializeVectors(index);
}
void ConvexHull::RemoveLight(Light* light)
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
void ConvexHull::InitializeVectors(unsigned int lightID)
{
	Hull::InitializeVectors(lightID);

	// Redimmentionne les conteneurs
	mLinkingPoly.resize(mLinkingPoly.size() + 1U);
	mProjectionPoly.resize(mProjectionPoly.size() + 1U);

	sf::ConvexShape shape;
	shape.setFillColor(sf::Color(0, 0, 0, 200));
	mLinkingPoly[lightID].resize(mShapes.size(), shape);
	mProjectionPoly[lightID].resize(mShapes.size(), shape);
}
void ConvexHull::ClearVectors(unsigned int lightID)
{
	Hull::ClearVectors(lightID);

	// Vide les polys de cette lampe
	mLinkingPoly[lightID].clear();
	mProjectionPoly[lightID].clear();
}

// Mise à jour
void ConvexHull::Update()
{
	// TODO: Inverser les boucles lights et shapes pour ne pas avoir à recalculer les points

	if (!mIsActivated)
		return;

	unsigned int iLight = 0U;
	for (std::vector<Light*>::iterator light = mLights.begin(); light < mLights.end(); ++light, ++iLight)
	{
		//if ((*light)->GetEmitter() == mBody) continue;

		sf::Vector2f lightPos = (*light)->GetPosition();
		float lightRadius = (*light)->GetRadius();
		unsigned int iShape = 0U;
		for (std::vector<b2PolygonShape*>::iterator shape = mShapes.begin(); shape < mShapes.end(); ++shape, ++iShape)
		{
			// Vérifie que la lumière touche ce shape
			mIsNear[iLight] = (*light)->GetAABB().intersects(mBody->GetSprite()->getGlobalBounds());

			// Si on doit recalculer les ombres
			if (mIsNear[iLight] && (!mIsStatic || !mHasChanged || !(*light)->IsStatic() || !mInitialized[iLight]))
			{
				// Vide les tableaus
				mPoints.clear();
				mHidedPoints.clear();
				mCastedPoints.clear();

				// Récupère les positions des points
				mPoints.resize((*shape)->GetVertexCount());
				for (unsigned int i = 0; i < mPoints.size(); ++i)
				{
					mPoints[i] = b22sfVec(mBody->GetBody()->GetWorldPoint((*shape)->GetVertex(i)), mBody->GetWorld()->GetPPM());
				}

				// Regarde quels points sont cachés
				unsigned int nbHided = 0U;
				mHidedPoints.resize(mPoints.size(), false);
				for (unsigned int i = 0; i < mPoints.size(); ++i)
				{
					// Pour chaque points => pour chaque arête
					for (unsigned int j = 0; j < mPoints.size() && !mHidedPoints[i]; ++j)
					{
						// Calcule le nb du prochain sommet
						unsigned int k = (j + 1 < mPoints.size()) ? (j + 1) : 0;

						// L'arête n'est pas issue du sommet testé
						if (i != j && i != k)
							if (intersect((*light)->GetPosition(), mPoints[i], mPoints[j], mPoints[k]))
							{
								mHidedPoints[i] = true;
								++nbHided;
							}
					}
				}

				// Prend les deux points extrèmes non cachés
				mExtremPoint1 = mExtremPoint2 = 0U;
				bool b1 = false;
				// Trouve ptE1, càd celui juste avant les cachés
				for (unsigned int i = 0; i < mPoints.size(); ++i)
				{
					if (!mHidedPoints[i])
					{
						mExtremPoint1 = i;
						b1 = true;
					}
					else if (b1)
						break;
				}
				// Trouve ptE2, càd celui juste après les cachés
				// Il est possible que ptE2 ne soit pas changé ci-après, mais ça veut dire qu'il vaut 0
				bool vuCache = false;
				for (unsigned int i = 0; i < mPoints.size(); ++i)
				{
					if (mHidedPoints[i])
						vuCache = true;

					if (vuCache && !mHidedPoints[i])
					{
						mExtremPoint2 = i;
						vuCache = false;
						break;
					}
				}

				// Projette les points
				mProjectionPoly[iLight][iShape].setPointCount((*shape)->GetVertexCount());
				for (unsigned int i = 0U; i < mPoints.size(); ++i)
				{
					mCastedPoints.push_back(mPoints[i] + (((mPoints[i] - lightPos) / distance(mPoints[i], lightPos)) * lightRadius * 3.f));
					
					mProjectionPoly[iLight][iShape].setPoint(i, mCastedPoints[i]);
				}

				// Récupère les points à afficher
				unsigned int offset = 0U;
				mLinkingPoly[iLight][iShape].setPointCount((mPoints.size() - nbHided) + 2U);
				for (unsigned int i = 0U; i < mPoints.size(); ++i)
				{
					if (!mHidedPoints[i])
					{
						if (i == mExtremPoint2)
						{
							mLinkingPoly[iLight][iShape].setPoint(offset++, mCastedPoints[i]);
						}
						
						mLinkingPoly[iLight][iShape].setPoint(offset++, mPoints[i]);

						if (i == mExtremPoint1)
						{
							mLinkingPoly[iLight][iShape].setPoint(offset++, mCastedPoints[i]);
						}
					}
				}
			}
		}
		mInitialized[iLight] = true;
	}
	mHasChanged = false;
}

void ConvexHull::draw(sf::RenderTarget &target, sf::RenderStates states) const
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
					target.draw(mProjectionPoly[iL][iS], states);
				}
			}
		}
	}
}
