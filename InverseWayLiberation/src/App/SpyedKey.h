#pragma once
#include "InputManager.h"

class SpyedKey : public NonCopyable
{
public:
	typedef std::shared_ptr<SpyedKey> Ptr;

	// Ctor statique
	static Ptr Create(sf::Keyboard::Key key)
	{
		return std::make_shared<SpyedKey>(key);
	}

public:
	// Ctor
	SpyedKey(sf::Keyboard::Key key)
		: mKey(key)
	{
		// Ajoute la touche espionnée
		InputManager::GetInstance().AddSpyedKey(mKey);
	}

	// Dtor
	~SpyedKey()
	{
		// Enlève la touche espionnée
		InputManager::GetInstance().RemoveSpyedKey(mKey);
	}

private:
	sf::Keyboard::Key mKey;
};
