#pragma once

// Classe appelée pour chaque fixture trouvé pendant la recherche de correspondance AABB <> fixtures dans le monde
class AABBListCallback : public b2QueryCallback, public NonCopyable
{
public:
	AABBListCallback(bool onlyDynamicsBody = true, const b2Body* bodyToAvoid = nullptr)
		: mBodyToAvoid(bodyToAvoid)
	{
		mOnlyDynamicsBody = onlyDynamicsBody;
	}
	virtual ~AABBListCallback()
	{
	}

	virtual bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if ((body->GetType() == b2_dynamicBody || !mOnlyDynamicsBody) && mBodyToAvoid != body)
		{
			mFixtureList.push_back(fixture);
		}

		// Continue la recherche
		return true;
	}

	std::list<b2Fixture*> GetFixtureList() { return mFixtureList; }

	void ClearList() { mFixtureList.clear(); }

private:
	std::list<b2Fixture*> mFixtureList;
	bool mOnlyDynamicsBody;
	const b2Body* mBodyToAvoid;
};
