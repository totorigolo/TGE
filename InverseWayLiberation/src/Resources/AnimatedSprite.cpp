#include "stdafx.h"
#include "AnimatedSprite.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class Animation

// Ctors
Animation::Animation()
	: mIsPlaying(false), mSpeed(0), mCurrentKey(0)
{
	mClock.restart();
	mLastTime = 0.f;
	mElapsedTime = 0.f;
}
Animation::Animation(int speed, unsigned int defaultKey, bool play)
	: mIsPlaying(play), mSpeed(speed), mCurrentKey(defaultKey)
{
	mClock.restart();
	mLastTime = 0.f;
	mElapsedTime = 0.f;
}

// Gestion de l'animation
void Animation::Start()
{
	mIsPlaying = true;
	mClock.restart();
	mLastTime = 0.f;
}
void Animation::Pause()
{
	mIsPlaying = false;
}
void Animation::TogglePlayingState()
{
	if (!mIsPlaying)
		Start();
	else
		Pause();
}
void Animation::Stop()
{
	mIsPlaying = false;
	mCurrentKey = 0;
}

// Mise � jour
void Animation::Update()
{
	mElapsedTime = mLastTime - mClock.getElapsedTime().asSeconds();

	// Avance l'animation
	while (mElapsedTime >= 1 / mSpeed)
	{
		mElapsedTime -= 1 / mSpeed;
		mCurrentKey = ++mCurrentKey % mKeys.size();
	}

	mLastTime = mClock.getElapsedTime().asSeconds();
}

// Accesseurs
bool Animation::IsPlaying() const
{
	return mIsPlaying;
}
int Animation::GetSpeed() const
{
	return mSpeed;
}
unsigned int Animation::GetCurrentKeyID() const
{
	return mCurrentKey;
}
const sf::IntRect& Animation::GetCurrentKey() const
{
	return mKeys[mCurrentKey];
}
const std::vector<sf::IntRect>& Animation::GetKeys() const
{
	return mKeys;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class AnimatedSprite

// Ctor & dtor
AnimatedSprite::AnimatedSprite(const std::string &animationPath)
	: Loader(animationPath)
{
}
AnimatedSprite::~AnimatedSprite()
{
}

// Gestion de l'animation
bool AnimatedSprite::SetAnimation(const std::string& animation)
{
	// Si l'animation est valide
	if (mIsValid)
	{
		// Si l'animation demand�e existe
		if (mAnimationMap.find(animation) != mAnimationMap.end())
		{
			mCurrentAnimation = animation;
			return true;
		}
		else
			return false;
	}
	// Sinon elle n'est pas valide
	return false;
}

// Mise � jour
void AnimatedSprite::Update()
{
	// Si l'animation est valide
	if (mIsValid)
	{
		// Met � jour l'animation en cours
		mAnimationMap[mCurrentAnimation].Update();

		// Met � jour le Subrect du sf::Sprite
		this->setTextureRect(mAnimationMap[mCurrentAnimation].GetCurrentKey());
	}
}

// Chargement de l'animation
bool AnimatedSprite::Process()
{
	/*
	// R�cup�re <animatedsprite> et <animationmap>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle animatedsprite = hdl.FirstChildElement("animatedsprite");
	tinyxml2::XMLHandle animationMap = animatedsprite.FirstChildElement("animationmap");

	// V�rifie que <animatedsprite> et <animationmap> existent
	if (!animatedsprite.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement de l'animation.\n<animatedsprite> non trouv� (" + mPath + ").", true);
		return false;
	}
	if (!animationMap.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement de l'animation.\n<animationmap> non trouv� (" + mPath + ").", true);
		return false;
	}
	
	// On cr�e les attributs
	std::string defaultAnimation;
	
	// Pour toutes les animations
	tinyxml2::XMLElement *entity = entities.FirstChildElement().ToElement();
	while (entity)
	{
		// R�initialise les attributs
		e = nullptr;
		layer = 1;
		type = "";
		animation = "";
		position = b2Vec2_zero;

		// R�cup�re le type
		type = entity->Name();

		// R�cup�re la position et la rotation
		if (entity->Attribute("position")) position = Parser::stringToB2Vec2(entity->Attribute("position"));
		
		// R�cup�re le layer
		entity->QueryIntAttribute("layer", &layer);

		// Cr�e l'Entity
		if (type == "ragdoll")
		{
			e = new Ragdoll(mLevel->mWorld, position, layer);
		}
		else if (type == "livingbeing")
		{
			// R�cup�re l'animation
			animation = entity->Attribute("animation");

			e = new LivingBeing(position, animation, layer);
		}

		// Cr�e l'Entity correspondante
		mLevel->mEntityManager.RegisterEntity(e);

		// On r�cup�re la prochaine entity
		entity = entity->NextSiblingElement();
	}
	//*/
	return true;
}

// Accesseurs
const std::string& AnimatedSprite::GetFilePath() const
{
	return std::string();
	//return GetFilePath();
}
const std::map<std::string, Animation>& AnimatedSprite::GetAnimationMap() const
{
	return mAnimationMap;
}
