#pragma once
#include "../Tools/Singleton.h"

#define LIGHTENGINE_DEBUGDRAW 0

class PointLight;
struct TextureHolder
{
public:
	// Ctor
	TextureHolder(std::pair<int, int> size);

public:
	// Taille
	std::pair<int, int> size;

	// RenderTexture
	sf::RenderTexture casterTex;
	sf::RenderTexture distortTex;
	sf::RenderTexture reduceTex;
	sf::RenderTexture blurTex;
	sf::Sprite casterSprite;
	sf::Sprite distortSprite;
	sf::Sprite blurSprite;
};

class Light;
class LightEngine : public Singleton<LightEngine>
{
protected:
	friend class Singleton<LightEngine>;

	// Ctor & dtor
	LightEngine(void);
	virtual ~LightEngine(void);

public:
	// Charge les shaders
	void LoadShaders(void);

	// Dessiner les obstables
	void DrawHull(PointLight *light, const sf::Drawable& hull);
	void DrawPhysicalHull(PointLight *light, const b2Body& body);

	// Cr�e les ombres
	void CreateShadows(PointLight *light);

	// Nettoie la m�moire
	void Clean();

protected:
	// Acc�s de PointLight
	friend class PointLight;

	// R�cup�rer les textures
	std::shared_ptr<TextureHolder>& GetTextures(std::pair<int, int> size);
	
	// Cr�e les ombres
	void CreateShadows(void);

	// Shaders
	sf::Shader distortShader;
	sf::Shader reduceShader;
	sf::Shader shadowShader;
	sf::Shader blurVShader;
	sf::Shader blurHShader;
	
	// States
	sf::RenderStates addStates;

private:
	// States
	sf::RenderStates mShaderStates;

	// RenderTextures
	std::map<std::pair<int, int>, std::shared_ptr<TextureHolder>> mTextures;

#if LIGHTENGINE_DEBUGDRAW
	// Fen�tre de rendu debug
	std::map<std::string, sf::RenderWindow*> mRenderWindows;
#endif
};

