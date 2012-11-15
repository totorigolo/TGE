#include "Level.h"
#include "utils.h"

// Ctor
Level::Level(World *world)
	: mIsValid(false), mIsCharged(false), mWorld(world),  mBckgC(255, 255, 255),
	mResourceManager(ResourceManager::GetInstance()), mTextureMap(mResourceManager.GetTextureMap())
{
	if (mWorld)
	{
		mIsValid = true;
	}
}

// Dtor
Level::~Level(void)
{
	mIsValid = false;
	mIsCharged = false;
}

// Vide tout le niveau
void Level::Clear()
{
	// Vide TOUS les objets du jeu // TODO: Vider les textures
	DestroyAllDeco();
	mWorld->DestroyAllJoints();
	mWorld->DestroyAllBody();

	mDecoMap; // TODO: vérifier si c'est vide
}

// Gestion de la déco
void Level::DestroyDecoLevel(int zindex)
{
	mDecoMap.erase(mDecoMap.find(zindex));
}
void Level::AddDeco(int level_zindex, sf::Sprite *sprite, int zindex)
{
	mDecoMap[level_zindex].push_back(std::make_pair(zindex, sprite));
	SortDecoByzIndex();
}
void Level::DestroyDeco(int level_zindex, sf::Sprite *sprite, int zindex)
{
	mDecoMap[level_zindex].remove(std::make_pair(zindex, sprite));
}
void Level::AddDeco(int level_zindex, b2Vec3 posRot, std::string const& texture, int zindex)
{
	sf::Sprite *s = new sf::Sprite(*mTextureMap[texture]);
	s->setPosition(b22sfVec(getVec2(posRot), mWorld->GetPPM()));
	s->setRotation(posRot.z);
	s->setOrigin(u2f(s->getTexture()->getSize()) / 2.f);
	mDecoMap[level_zindex].push_back(std::make_pair(zindex, s));
	SortDecoByzIndex();
}
void Level::DestroyDeco(int level_zindex, b2Vec3 posRot, std::string const& texture, int zindex)
{
	sf::Sprite *s = new sf::Sprite(*mTextureMap[texture]);
	s->setPosition(b22sfVec(getVec2(posRot), mWorld->GetPPM()));
	s->setRotation(posRot.z);
	mDecoMap[level_zindex].remove(std::make_pair(zindex, s));
}
void Level::DestroyAllDeco()
{
	mDecoMap.clear();
}

void Level::SortDecoByzIndex()
{
	// Tri les niveaux de deco par leur zindex
	for (auto it = mDecoMap.begin(); it != mDecoMap.end(); ++it)
	{
		it->second.sort();
	}
}
