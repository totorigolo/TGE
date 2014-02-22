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

// Mise à jour
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
		// Si l'animation demandée existe
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

// Mise à jour
void AnimatedSprite::Update()
{
	// Si l'animation est valide
	if (mIsValid)
	{
		// Met à jour l'animation en cours
		mAnimationMap[mCurrentAnimation].Update();

		// Met à jour le Subrect du sf::Sprite
		this->setTextureRect(mAnimationMap[mCurrentAnimation].GetCurrentKey());
	}
}

// Chargement de l'animation
bool AnimatedSprite::Process()
{
	/*
	// Récupère <animatedsprite> et <animationmap>
	tinyxml2::XMLHandle hdl(mFile);
	tinyxml2::XMLHandle animatedsprite = hdl.FirstChildElement("animatedsprite");
	tinyxml2::XMLHandle animationMap = animatedsprite.FirstChildElement("animationmap");

	// Vérifie que <animatedsprite> et <animationmap> existent
	if (!animatedsprite.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement de l'animation.\n<animatedsprite> non trouvé (" + mPath + ").", true);
		return false;
	}
	if (!animationMap.ToElement())
	{
		Dialog::Error("Une erreur fatale est survenue lors du chargement de l'animation.\n<animationmap> non trouvé (" + mPath + ").", true);
		return false;
	}
	
	// On crée les attributs
	std::string defaultAnimation;
	
	// Pour toutes les animations
	tinyxml2::XMLElement *entity = entities.FirstChildElement().ToElement();
	while (entity)
	{
		// Réinitialise les attributs
		e = nullptr;
		layer = 1;
		type = "";
		animation = "";
		position = b2Vec2_zero;

		// Récupère le type
		type = entity->Name();

		// Récupère la position et la rotation
		if (entity->Attribute("position")) position = Parser::stringToB2Vec2(entity->Attribute("position"));
		
		// Récupère le layer
		entity->QueryIntAttribute("layer", &layer);

		// Crée l'Entity
		if (type == "ragdoll")
		{
			e = new Ragdoll(mLevel->mWorld, position, layer);
		}
		else if (type == "livingbeing")
		{
			// Récupère l'animation
			animation = entity->Attribute("animation");

			e = new LivingBeing(position, animation, layer);
		}

		// Crée l'Entity correspondante
		mLevel->mEntityManager.RegisterEntity(e);

		// On récupère la prochaine entity
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
