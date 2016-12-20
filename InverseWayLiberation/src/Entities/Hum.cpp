#include <cmath>

#include "Hum.h"
#include "../Physics/PhysicManager.h"
#include "../Resources/ResourceManager.h"
#include "../Tools/utils.h"

// Ctor & dtor
Hum::Hum(int layer)
	: Entity(layer), mPhysicMgr(PhysicManager::GetInstance()), mBodyIsCreated(false), mBody(nullptr), mHull(nullptr),
	mAge(0.0), mStrengh(0.0), mHeight(0.0), mAgeFactor(0.0), mTopWidth(0.0), mBottomWidth(0.0), mEyeHeight(0.0),
	mHasMoved(true), mIsDead(true), mCanJump(true), mSensorJump(nullptr), mSpeaking(false)
{
	mType = EntityType::Hum;
	mIsAlive = false;

	// Bulle de texte
	ResourceManager::GetInstance().LoadFont("calibri", "data/calibri.ttf");
	mBallonText.setFont(*ResourceManager::GetInstance().GetFont("calibri"));
	mBallonBckg.setFillColor(sf::Color(30, 30, 30, 210));
	mBallonBckg.setOutlineColor(mBallonBckg.getFillColor());
	mBallonBckg.setOutlineThickness(5.f);
	mBallonBckg.setOrigin(0, -7.f);
}
Hum::~Hum()
{
	Destroy();
}

// Crée le Hum
// Age adulte à 18 ans et musculature entre -20 (obèse) à 20 (taillé en V)
bool Hum::Create(b2Vec2 pos, double age, double strengh, sf::Color color)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mBodyIsCreated)
		return false;

	/* Shape */
	mAge = age;
	mStrengh = strengh;
	// Propriétés du shape (en m) en fonction de l'age et de la force
	mHeight = 0.15 + 0.2 * (exp(0.4 * mAge - 3.0) / (1.0 + exp(0.4 * mAge - 3.0)));
	mAgeFactor = min(pow(0.15 * mAge, mAge * 2.0), 1.0);
	mTopWidth = 0.3 + 0.006 * mStrengh * mAgeFactor;
	mBottomWidth = 0.3 - 0.006 * mStrengh * mAgeFactor;
	mEyeHeight = 85.0 * mHeight + mStrengh * 0.4;

	// b2Shape
	b2Vec2 *v = new b2Vec2[4];
	v[0] = b2Vec2(static_cast<float>(-mBottomWidth / 2.0), 0.f); // bottom left
	v[1] = b2Vec2(static_cast<float>(mBottomWidth / 2.0), 0.f); // bottom right
	v[2] = b2Vec2(static_cast<float>(mTopWidth / 2.0), static_cast<float>(mHeight)); // top right
	v[3] = b2Vec2(static_cast<float>(-mTopWidth / 2.0), static_cast<float>(mHeight)); // top left
	b2PolygonShape shape;
	shape.Set(v, 4);

	// Corps
	mTrunkColor = color;
	mTrunk.setPointCount(4);
	for (unsigned int i = 0; i < 4; ++i)
		mTrunk.setPoint(i, b22sfVec(v[i], mPhysicMgr.GetPPM()));
	mTrunk.setFillColor(mTrunkColor);
	mTrunk.setOutlineColor(sf::Color(7, 7, 7));
	mTrunk.setOutlineThickness(2.f);

	// Yeux
	mEyeLeft.setRadius(4.f);
	mEyeLeft.setOutlineThickness(1.3f);
	mEyeLeft.setFillColor(sf::Color::White);
	mEyeLeft.setOutlineColor(sf::Color::Black);
	mEyeRight.setRadius(4.f);
	mEyeRight.setOutlineThickness(1.1f);
	mEyeRight.setFillColor(sf::Color::White);
	mEyeRight.setOutlineColor(sf::Color::Black);
	mPupilLeft.setRadius(2.f);
	mPupilLeft.setFillColor(sf::Color::Black);
	mPupilRight.setRadius(2.f);
	mPupilRight.setFillColor(sf::Color::Black);

	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.position = pos;
	bodyDef.angle = 0;
	bodyDef.type = b2BodyType::b2_dynamicBody;
	bodyDef.fixedRotation = true;
	mBody = mPhysicMgr.CreateBody(&bodyDef);

	// Fixture
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.8f;
	fixtureDef.restitution = 0.f;
	fixtureDef.filter.groupIndex = 1;
	fixtureDef.filter.categoryBits = 0x0001;
	fixtureDef.filter.maskBits = 0xFFFF;
	fixtureDef.shape = &shape;
	mBody->CreateFixture(&fixtureDef);

	// Shape & Fixture pour la détection du saut
	float x = (mTrunk.getGlobalBounds().width / 2.f) * mPhysicMgr.GetMPP();
	b2PolygonShape shape2;
	b2Vec2 center(0.f, -0.03f);
	shape2.SetAsBox(x - 0.02f, 0.03f, center, 0.f);
	b2FixtureDef fixtureDef2;
	fixtureDef2.shape = &shape2;
	fixtureDef2.isSensor = true;
	mSensorJump = mBody->CreateFixture(&fixtureDef2);

	// Enregistrements
	mBody->SetUserData(this);
	mBodyIsCreated = true;
	mIsAlive = true;
	mIsDead = false;

	// Active les ombres
	mHull.SetBodyShadowCaster(mBody);
	mHull.SetPhysicallyDrawable(true);
	mHull.Activate();

	return true;
}

// Destruction du body
void Hum::Destroy()
{
	if (mBody)
	{
		// Supprime le Body
		mPhysicMgr.DestroyBody(mBody);
		mBody = nullptr;
	}
	mBodyIsCreated = false;
	mIsAlive = false;
}

// Mise à jour
void Hum::PreUpdate()
{
	// Si le body est valide
	if (mBody && mBodyIsCreated && mIsAlive)
	{
		// Vérifie les Sensors
		CheckSensors();

		// Intelligence artificielle
		SimulateAI();

		/* Mise à jour des position des éléments dessinables */
		// Corps
		mTrunk.setPosition(b22sfVec(mBody->GetPosition(), mPhysicMgr.GetPPM()));
		mTrunk.setRotation(-mBody->GetAngle() * DPR);

		// Yeux
		mEyeLeft.setPosition(mTrunk.getPosition() + sf::Vector2f(4.f, (float) -mEyeHeight));
		mEyeRight.setPosition(mTrunk.getPosition() + sf::Vector2f(-(4.f + mEyeRight.getRadius() * 2.f), (float) -mEyeHeight));
		mPupilLeft.setPosition(mEyeLeft.getPosition() + sf::Vector2f(mEyeLeft.getRadius() - mPupilLeft.getRadius(), mEyeLeft.getRadius() - mPupilLeft.getRadius()));
		mPupilRight.setPosition(mEyeRight.getPosition() + sf::Vector2f(mEyeRight.getRadius() - mPupilRight.getRadius(), mEyeRight.getRadius() - mPupilRight.getRadius()));

		/* Mise à jour du Hull */
		if (mHasMoved || (mBody->IsAwake() && mBody->GetType() != b2BodyType::b2_staticBody))
		{
			auto gb = mTrunk.getGlobalBounds();
			mHull.SetPosAndSize(sf::Vector2f(gb.left, gb.top), sf::Vector2f(gb.width, gb.height));
		}
	}
}
void Hum::Update()
{
	// Si le body est valide
	if (mBody && mBodyIsCreated && mIsAlive)
	{
		mHasMoved = false;

		// Gestion du blabla
		if (mSpeaking)
		{
			// Regarde si le temps de parole est écoulé
			if (mSpeakClock.getElapsedTime().asMilliseconds() > mSpeakTime)
				mSpeaking = false;

			// Position de la bulle
			sf::Vector2f pos = b22sfVec(GetPosition(), mPhysicMgr.GetPPM());
			pos += sf::Vector2f(- mBallonText.getGlobalBounds().width / 2.f, - mBallonText.getGlobalBounds().height * 2.f);
			pos += b22sfVec(b2Vec2(0.f, 3.f / 2.f * static_cast<float>(this->GetHeight())), mPhysicMgr.GetPPM());
			mBallonText.setPosition(pos);
			mBallonBckg.setPosition(pos);
		}
	}
}
void Hum::CheckSensors()
{
	// Vérifie le saut
	if (mSensorJump)
		mCanJump = CheckOneSensor(mSensorJump);
}

// Intelligence artificielle
void Hum::SimulateAI()
{

}

// Animation
void Hum::Speak(const std::string &msg, float time)
{
	// Gestion du temps
	mSpeaking = true;
	mSpeakTime = time;
	mSpeakClock.restart();
	
	// Gestion du texte
	mBallonText.setString(msg);
	mBallonText.setFillColor(sf::Color::White);
	
	// Gestion du fond
	mBallonBckg.setSize(sf::Vector2f(mBallonText.getGlobalBounds().width, mBallonText.getGlobalBounds().height));
}

// Gestion de la couleur
void Hum::SetTrunkColor(const sf::Color &color)
{
	mTrunkColor = color;
	mTrunk.setFillColor(mTrunkColor);
}
sf::Color Hum::GetTrunkColor() const
{
	return mTrunkColor;
}

// Accesseurs
b2Body* Hum::GetBody()
{
	return mBody;
}
const b2Body* Hum::GetBody() const
{
	return mBody;
}
bool Hum::HasMoved() const
{
	return mHasMoved;
}
bool Hum::IsDead() const
{
	return mIsDead;
}
bool Hum::CanJump() const
{
	return mCanJump;
}
b2Vec2 Hum::GetPosition() const
{
	return mBody->GetPosition();
}
double Hum::GetAge() const
{
	return mAge;
}
double Hum::GetStrengh() const
{
	return mStrengh;
}
double Hum::GetHeight() const
{
	return mHeight;
}
double Hum::GetTopWidth() const
{
	return mTopWidth;
}
double Hum::GetBottomWidth() const
{
	return mBottomWidth;
}
double Hum::GetEyeHeight() const
{
	return mEyeHeight;
}
// Fonction à n'employer que pour éditer les niveaux
void Hum::SetTransform(const b2Vec2 &position)
{
	myAssert(mBody, "b2Body null");
	mBody->SetTransform(position, 0.f);
	mBody->SetAwake(true);
	mHasMoved = true;
}

// Mise à jour
bool Hum::CheckOneSensor(b2Fixture *f)
{
	for (b2ContactEdge* ce = mBody->GetContactList(); ce; ce = ce->next)
	{
		b2Contact* c = ce->contact;

		if (c->GetFixtureA() == f || c->GetFixtureB() == f)
		{
			if (c->IsTouching())
			{
				return true;
			}
		}
	}

	return false;
}

// Pour le rendu
void Hum::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!mIsAlive) return;

	target.draw(mTrunk, states);
	target.draw(mEyeLeft, states);
	target.draw(mEyeRight, states);
	target.draw(mPupilLeft, states);
	target.draw(mPupilRight, states);
	if (mSpeaking)
	{
		target.draw(mBallonBckg, states);
		target.draw(mBallonText, states);
	}
}
