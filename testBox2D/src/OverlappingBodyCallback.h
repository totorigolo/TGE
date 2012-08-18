#pragma once
#include <Box2D/Box2D.h>

// Classe appelée pour chaque fixture trouvé pendant la recherche de correspondance AABB <> fixtures dans le monde
class OverlappingBodyCallback : public b2QueryCallback
{
public:
	OverlappingBodyCallback(const b2Vec2& point)
	{
		mPoint = point;
		mFixture = nullptr;
	}

	virtual bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			if (fixture->TestPoint(mPoint))
			{
				mFixture = fixture;

				// On a trouvé donc on demande d'arreter la recherche
				return false;
			}
		}

		// Continue la recherche
		return true;
	}
	
	b2Vec2 GetPoint() { return mPoint; }
	b2Fixture* GetFixture() { return mFixture; }

private:
	b2Vec2 mPoint;
	b2Fixture* mFixture;
};
