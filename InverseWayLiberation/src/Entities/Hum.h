#pragma once
#include "Entity.h"
#include "Hull.h"
#include "../Resources/Texture.h"

class PhysicManager;
class Hum : public Entity
{
public:
	// Ctor & dtor
	Hum(int layer = 0);
	virtual ~Hum();

	// Crée le Hum
	// Age adulte à 18 ans et musculature entre -20 (obèse) à 20 (taillé en V)
	bool Create(b2Vec2 pos, double age, double strengh, sf::Color color = sf::Color(35, 35, 35));

	// Destruction du body
	void Destroy();

	// Mise à jour
	virtual void PreUpdate();
	virtual void Update();

	// Animation
	void LookAt(b2Vec2 location);
	void Blink(void);

	// Accesseurs
	b2Body* GetBody();
	const b2Body* GetBody() const;
	bool HasMoved() const;
	bool IsDead() const;
	bool CanJump() const;
	b2Vec2 GetPosition() const;
	double GetAge() const;
	double GetStrengh() const;
	double GetHeight() const;
	double GetTopWidth() const;
	double GetBottomWidth() const;
	double GetEyeHeight() const;
	sf::Color GetTrunkColor() const;

protected:
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	// Etat
	bool mHasMoved;
	bool mIsDead;
	bool mCanJump;

private:
	/* Pour le dessin */
	// Mensurations et infos
	double mAge;
	double mStrengh;
	double mHeight;
	double mAgeFactor;
	double mTopWidth;
	double mBottomWidth;
	double mEyeHeight;
	sf::Color mTrunkColor;

	// Animation -> TODO
	bool mBlinking;
	double mBlinkState;
	sf::Vector2f mLookAt;
	sf::Vector2f mPupilLeftPos;
	sf::Vector2f mPupilRightPos;

	// Sensors pour les déplacements
	b2Fixture *mSensorJump;

	// Formes SFML
	sf::ConvexShape mTrunk;
	sf::CircleShape mEyeLeft;
	sf::CircleShape mEyeRight;
	sf::CircleShape mPupilLeft;
	sf::CircleShape mPupilRight;

	// PhysicManager
	PhysicManager &mPhysicMgr;

	// Le body Box2D
	bool mBodyIsCreated;
	b2Body *mBody;

	// Ombres
	Hull mHull;
};