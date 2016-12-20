#pragma once

// Classe appelée pour chaque fixture trouvé pendant la recherche de correspondance AABB <> fixtures dans le monde
class PointCallback : public b2QueryCallback, public NonCopyable
{
public:
	PointCallback(const b2Vec2& point, bool onlyDynamicsBody = true)
	{
		mOnlyDynamicsBody = onlyDynamicsBody;
		mPoint = point;
		mFixture = nullptr;
		
		// Crée l'AABB
		b2Vec2 d(0.001f, 0.001f);
		mAABB.lowerBound = mPoint - d;
		mAABB.upperBound = mPoint + d;
	}
	virtual ~PointCallback()
	{
	}

	virtual bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody || !mOnlyDynamicsBody)
		{
			// Regarde si il ne s'agit pas d'un type de fixture dans TestPoint
			bool dontCheck = false;
			if (fixture->GetBody()->GetUserData())
			{
				Entity *e = (Entity*) fixture->GetBody()->GetUserData();

				if (e)
				{
					if (e->GetType() == EntityType::PolyChain)
						dontCheck = ((PolyChain*) e)->Getb2BodyType() == b2_dynamicBody || !mOnlyDynamicsBody;
				}
			}

			// Test si le point est effectivement dans le fixture
			if (fixture->TestPoint(mPoint) || dontCheck)
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
	const b2AABB& GetAABB() const { return mAABB; }

private:
	b2AABB mAABB;
	b2Vec2 mPoint;
	b2Fixture* mFixture;
	bool mOnlyDynamicsBody;
};
