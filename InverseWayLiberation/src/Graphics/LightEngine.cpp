#include "stdafx.h"
#include "LightEngine.h"
#include "../Entities/PolyChain.h"
#include "../Entities/PointLight.h"
#include "../Physics/DebugDraw.h"
#include "../Physics/PhysicManager.h"

// Ctor
TextureHolder::TextureHolder(std::pair<int, int> size)
	: size(size)
{
	// CasterTexture
	casterTex.create(size.first, size.second);
	casterSprite.setTexture(casterTex.getTexture(), true);
	
	// DistortTexture
	distortTex.create(size.first, size.second);
	distortSprite.setTexture(distortTex.getTexture(), true);
	
	// ReduceTexture
	reduceTex.create(2, size.second);

	// BlurVTexture
	blurTex.create(size.first, size.second);
	blurSprite.setTexture(blurTex.getTexture(), true);
}

// Ctor
LightEngine::LightEngine(void)
{
	// Charge les shaders
	LoadShaders();

	// Configure l'état additif
	addStates.blendMode = sf::BlendAdd;

#if LIGHTENGINE_DEBUGDRAW
	mRenderWindows["physicalHull"] = new sf::RenderWindow(sf::VideoMode(800U, 600U), "Light physical hull debug");
	mRenderWindows["casterTex"] = new sf::RenderWindow(sf::VideoMode(800U, 600U), "Light debug - caster");
	mRenderWindows["distortShader"] = new sf::RenderWindow(sf::VideoMode(800U, 600U), "Light debug - distort");
	mRenderWindows["reduceShader"] = new sf::RenderWindow(sf::VideoMode(800U, 600U), "Light debug - reduce");
	mRenderWindows["shadowShader"] = new sf::RenderWindow(sf::VideoMode(800U, 600U), "Light debug - shadow");
	mRenderWindows["blurShader"] = new sf::RenderWindow(sf::VideoMode(800U, 600U), "Light debug - blur");
#endif
}

// Dtor
LightEngine::~LightEngine(void)
{
	// Charge les shaders
	LoadShaders();
}

// Charge les shaders
void LightEngine::LoadShaders(void)
{
	// Distort shader
	distortShader.loadFromFile("fx/distort.fx", sf::Shader::Fragment);
	distortShader.setParameter("texture", sf::Shader::CurrentTexture);

	// Reduce shader
	reduceShader.loadFromFile("fx/reduce.fx", sf::Shader::Fragment);
	reduceShader.setParameter("texture", sf::Shader::CurrentTexture);

	// Shadow shader
	shadowShader.loadFromFile("fx/shadow.fx", sf::Shader::Fragment);

	// BlurV shader
	blurVShader.loadFromFile("fx/blurV.fx", sf::Shader::Fragment);
	blurVShader.setParameter("texture", sf::Shader::CurrentTexture);

	// BlurH shader
	blurHShader.loadFromFile("fx/blurH.fx", sf::Shader::Fragment);
	blurHShader.setParameter("texture", sf::Shader::CurrentTexture);
}

// Dessiner les obstables
void LightEngine::DrawHull(PointLight *light, const sf::Drawable& hull)
{
	light->textures->casterTex.setView(light->view);
	light->textures->casterTex.draw(hull);
}
void LightEngine::DrawPhysicalHull(PointLight *light, const b2Body& body)
{
	// Debug draw
#if LIGHTENGINE_DEBUGDRAW
	mRenderWindows["physicalHull"]->clear();
	mRenderWindows["physicalHull"]->setView(light->view);
#endif

	// Change la vue de la texture
	light->textures->casterTex.setView(light->view);

	// Parcours toutes les Fixtures du body
	for (const b2Fixture* fixture = body.GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		// Récupère la transformation du Body et le shape de la Fixture
		b2Transform t = body.GetTransform();
		const b2Shape *shape = fixture->GetShape();

		// Pour chaque type différent de Body
		if (shape->GetType() == b2Shape::e_chain)
		{
			// Détermine s'il s'agit d'un loop ou d'un chain
			bool isLoop = false;
			if (body.GetUserData())
			{
				Entity* e = (Entity*) body.GetUserData();
				if (e->GetType() == EntityType::PolyChain)
				{
					PolyChain* pc = (PolyChain*) e;
					if (pc->GetChainType() == PolyChain::Type::Loop)
						isLoop = true;
				}
			}

			// Récupère le shape
			b2ChainShape *chain = (b2ChainShape*) shape;

			// Crée le polygone
			sf::VertexArray va;
			va.setPrimitiveType(sf::LinesStrip);
			va.resize(chain->m_count + ((isLoop) ? 1U : 0U));

			// Remplit le polygone
			for (int32 i = 0; i < chain->m_count; ++i)
			{
				va[i].position = b22sfVec(b2Mul(t, chain->m_vertices[i]), PhysicManager::GetInstance().GetPPM());
			}
			if (isLoop)
				va[chain->m_count].position = b22sfVec(b2Mul(t, chain->m_vertices[0]), PhysicManager::GetInstance().GetPPM());

			// Dessine le polygone
			light->textures->casterTex.resetGLStates();
			glLineWidth(3.5f);
			light->textures->casterTex.draw(va);
			glLineWidth(1.f);
#if LIGHTENGINE_DEBUGDRAW
			mRenderWindows["physicalHull"]->resetGLStates();
			glLineWidth(3.5f);
			mRenderWindows["physicalHull"]->draw(va);
			glLineWidth(1.f);
#endif
		}
		else if (shape->GetType() == b2Shape::e_polygon)
		{
			// Récupère le shape
			b2PolygonShape *poly = (b2PolygonShape*) shape;

			// Crée le polygone
			sf::ConvexShape sfpoly;
			sfpoly.setPointCount(poly->m_count + 1U);

			// Remplit le polygone
			for (int32 i = 0; i < poly->m_count; ++i)
			{
				sfpoly.setPoint(i, b22sfVec(b2Mul(t, poly->m_vertices[i]), PhysicManager::GetInstance().GetPPM()));
			}
			sfpoly.setPoint(poly->m_count, b22sfVec(b2Mul(t, poly->m_vertices[0]), PhysicManager::GetInstance().GetPPM()));

			// Dessine le polygone
			light->textures->casterTex.draw(sfpoly);
#if LIGHTENGINE_DEBUGDRAW
			mRenderWindows["physicalHull"]->draw(sfpoly);
#endif
		}
		/*if (shape->GetType() == b2Shape::e_chain)
		{
			// Récupère le shape
			b2ChainShape *chain = (b2ChainShape*) shape;

			// Crée le polygone
			sf::ConvexShape sfpoly;
			sfpoly.setPointCount(chain->m_count + 1U);

			// Remplit le polygone
			for (int32 i = 0; i < chain->m_count; ++i)
			{
				sfpoly.setPoint(i, b22sfVec(b2Mul(t, chain->m_vertices[i]), PhysicManager::GetInstance().GetPPM()));
			}
			sfpoly.setPoint(chain->m_count, b22sfVec(b2Mul(t, chain->m_vertices[0]), PhysicManager::GetInstance().GetPPM()));

			// Dessine le polygone
			light->textures->casterTex.draw(sfpoly);
			mRenderWindow.draw(sfpoly);
		}
		else if (shape->GetType() == b2Shape::e_polygon)
		{
			// Récupère le shape
			b2PolygonShape *poly = (b2PolygonShape*) shape;

			// Crée le polygone
			sf::ConvexShape sfpoly;
			sfpoly.setPointCount(poly->m_count + 1U);

			// Remplit le polygone
			for (int32 i = 0; i < poly->m_count; ++i)
			{
				sfpoly.setPoint(i, b22sfVec(b2Mul(t, poly->m_vertices[i]), PhysicManager::GetInstance().GetPPM()));
			}
			sfpoly.setPoint(poly->m_count, b22sfVec(b2Mul(t, poly->m_vertices[0]), PhysicManager::GetInstance().GetPPM()));

			// Dessine le polygone
			light->textures->casterTex.draw(sfpoly);
			mRenderWindow.draw(sfpoly);
		}*/
	}

	// Debug Draw
#if LIGHTENGINE_DEBUGDRAW
	mRenderWindows["physicalHull"]->display();
#endif
}

// Crée les ombres
void LightEngine::CreateShadows(PointLight *light)
{
	// Fini le rendu de la casterTexture
	light->textures->casterTex.display();

	// Applique le distort shader
	mShaderStates.shader = &distortShader;
	light->textures->distortTex.draw(light->textures->casterSprite, mShaderStates);
	light->textures->distortTex.display();
	
	// Applique le reduce shader
	reduceShader.setParameter("renderTargetWidth", light->GetBoundingBox().width);
	mShaderStates.shader = &reduceShader;
	light->textures->reduceTex.draw(light->textures->distortSprite, mShaderStates);

	// Applique le shadow shader
	shadowShader.setParameter("reduce", light->textures->reduceTex.getTexture());
	shadowShader.setParameter("renderTargetSize", light->GetBoundingBox().width);//light->mLightRadius * 2.f); // Fix ?
	shadowShader.setParameter("lightColor", light->mLightColor);
	mShaderStates.shader = &shadowShader;
	light->shadowTex.clear();
	light->shadowTex.draw(light->textures->casterSprite, mShaderStates);
	light->shadowTex.display();
	
	// Applique le blur shader
	mShaderStates.shader = &blurHShader;
	light->textures->blurTex.draw(light->shadowSprite, mShaderStates);
	light->textures->blurTex.display();
	mShaderStates.shader = &blurVShader;
	light->shadowTex.draw(light->textures->blurSprite, mShaderStates);
	light->shadowTex.display();

	// Efface la casterTexture
	light->textures->casterTex.clear(sf::Color::Transparent);


#if LIGHTENGINE_DEBUGDRAW
	mRenderWindows["casterTex"]->clear();
	mRenderWindows["distortShader"]->clear();
	mRenderWindows["reduceShader"]->clear();
	mRenderWindows["shadowShader"]->clear();
	mRenderWindows["blurShader"]->clear();

	mRenderWindows["casterTex"]->draw(light->textures->casterSprite);
	mRenderWindows["distortShader"]->draw(light->textures->distortSprite);
	mRenderWindows["reduceShader"]->draw(sf::Sprite(light->textures->reduceTex.getTexture()));
	mRenderWindows["shadowShader"]->draw(light->shadowSprite);
	mRenderWindows["blurShader"]->draw(light->textures->blurSprite);

	mRenderWindows["casterTex"]->display();
	mRenderWindows["distortShader"]->display();
	mRenderWindows["reduceShader"]->display();
	mRenderWindows["shadowShader"]->display();
	mRenderWindows["blurShader"]->display();
#endif
}

// Nettoie la mémoire
void LightEngine::Clean(void)
{
	// Parcours toutes les TexturesHolders
	for (auto it = mTextures.begin(); it != mTextures.end(); )
	{
		// Si il n'y a aucune référence à cette taille
		if (it->second.use_count() == 1)
			it = mTextures.erase(it);
		else
			++it;
	}
}

// Récupérer les textures
std::shared_ptr<TextureHolder>& LightEngine::GetTextures(std::pair<int, int> size)
{
	// Cherche si on a les textures correspondantes
	auto textures = mTextures.find(size);

	// Si on n'a pas, on crée
	if (textures == mTextures.end())
	{
		std::cout << "Cr\x82""ation d'une texture " << size.first << "x" << size.second << "..."; // TODO: Log
		mTextures[size] = std::shared_ptr<TextureHolder>(new TextureHolder(size));
		textures = mTextures.find(size);
		std::cout << " Fait." << std::endl;
	}

	// Retourne les textures
	return textures->second;
}
