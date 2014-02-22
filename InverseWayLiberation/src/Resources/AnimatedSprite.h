#pragma once
#include "../Tools/Loader.h"

class Animation
{
public:
	// Ctors
	Animation();
	Animation(int speed, unsigned int defaultKey, bool play);

	// Gestion de l'animation
	void Start();
	void Pause();
	void TogglePlayingState();
	void Stop();

	// Mise à jour
	void Update();

	// Accesseurs
	bool IsPlaying() const;
	int GetSpeed() const;
	unsigned int GetCurrentKeyID() const;
	const sf::IntRect& GetCurrentKey() const;
	const std::vector<sf::IntRect>& GetKeys() const;

private:
	friend class AnimatedSprite;

	// Propriétés de l'animation
	bool mIsPlaying;
	int mSpeed; // Clé par seconde
	sf::Clock mClock;
	float mLastTime;
	float mElapsedTime;

	// Liste des clés
	unsigned int mCurrentKey;
	std::vector<sf::IntRect> mKeys;
};

class AnimatedSprite : public sf::Sprite, public Loader
{
public:
	// Ctor & dtor
	AnimatedSprite(const std::string &animationPath);
	virtual ~AnimatedSprite();

	// Gestion de l'animation
	bool SetAnimation(const std::string& animation);

	// Mise à jour
	void Update();

	// Accesseurs
	const std::string& GetFilePath() const;
	const std::map<std::string, Animation>& GetAnimationMap() const;

private:
	// Chargement de l'animation
	virtual bool Process();

	std::string mCurrentAnimation;
	std::map<std::string, Animation> mAnimationMap;
};
