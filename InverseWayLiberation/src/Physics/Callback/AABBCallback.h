#pragma once

// Classe appel�e pour chaque fixture trouv� pendant la recherche de correspondance AABB <> fixtures dans le monde
class AABBCallback : public b2QueryCallback, public NonCopyable
{
public:
	AABBCallback(bool onlyDynamicsBody = true, const b2Body* bodyToAvoid = nullptr)
		: mBodyToAvoid(bodyToAvoid)
	{
		mOnlyDynamicsBody = onlyDynamicsBody;
		mFixture = nullptr;
	}
	virtual ~AABBCallback()
	{
	}

	virtual bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if ((body->GetType() == b2_dynamicBody || !mOnlyDynamicsBody) && mBodyToAvoid != body)
		{
			mFixture = fixture;

			// On a trouv� donc on demande d'arreter la recherche
			return false;
		}

		// Continue la recherche
		return true;
	}

	b2Fixture* GetFixture() { return mFixture; }

	void Clear() { mFixture = nullptr; }

private:
	b2Fixture* mFixture;
	bool mOnlyDynamicsBody;
	const b2Body* mBodyToAvoid;
};
