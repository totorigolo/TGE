#pragma once
#include "../../Tools/NonCopyable.h"
#include <Box2D/Box2D.h>

// Ce callback est utilisé pour trouver le body le plus près lors d'un raycast
class FirstBodyRaycastCallback : public b2RayCastCallback, public NonCopyable
{
public:
	// Ctor & Dtor
	FirstBodyRaycastCallback(b2Body *body = nullptr)
		: mFixture(nullptr), mBody(body), mHit(false)
	{
	}
	virtual ~FirstBodyRaycastCallback()
	{
	}

	void SetBody(b2Body *body)
	{
		mBody = body;
	}

	virtual float32 ReportFixture(b2Fixture* fixture, b2Vec2 const& point, b2Vec2 const& normal, float32 fraction)
	{
		// Si on a un body
		if (mBody)
		{
			// On cherche si le point appartient au body
			if (fixture->GetBody() == mBody)
			{
				mHit = true;
				mFixture = fixture;
				mPoint = point;
				mNormal = normal;
				mFraction = fraction;

				return fraction;
			}
			else
			{
				return -1;
			}
		}

		// Sinon, on garde le point
		mHit = true;
		mFixture = fixture;
		mPoint = point;
		mNormal = normal;
		mFraction = fraction;

		return fraction;
	}

	bool mHit;
	b2Body *mBody;
	b2Fixture* mFixture;
	b2Vec2 mPoint;
	b2Vec2 mNormal;
	float32 mFraction;
};
