#include "stdafx.h"
#include "LightEngine.h"
#include "../Entities/PolyChain.h"
#include "../Entities/PointLight.h"
#include "../Physics/DebugDraw.h"
#include "../Physics/PhysicManager.h"

// Ctor
LightEngine::LightEngine(void)
: mZero(0.f, 0.f), mOne(1.f, 1.f), mIsActive(true)
{
	// Configure l'�tat additif
	addStates.blendMode = sf::BlendAdd;

	// Charge les shaders
	LoadShaders();
}

// Dtor
LightEngine::~LightEngine(void)
{
}

// Charge les shaders
void LightEngine::LoadShaders(void)
{
	myCheckError_v(mShadowShader.loadFromFile("fx/shadow.fx", sf::Shader::Fragment),
		"Erreur pas encore g�r�e : shader introuvable"); // TODO
	mShadowShader.setParameter("texture", sf::Shader::CurrentTexture);
}

// Dessiner les obstables
void LightEngine::DrawPhysicalHull(PointLight *light, const b2Body& body)
{
	if (!mIsActive) return;

	// Parcours toutes les Fixtures du body
	for (const b2Fixture* fixture = body.GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		if (fixture->IsSensor()) continue;

		// R�cup�re la transformation du Body et le shape de la Fixture
		b2Transform t = body.GetTransform();
		const b2Shape *shape = fixture->GetShape();

		// TODO : Ne pas dupliquer la projection pour chaque type

		// Pour chaque type diff�rent de Body
		/*
			e_circle = 0,
			e_edge = 1,
		*/
		if (shape->GetType() == b2Shape::e_chain) // Chain et loop
		{
			// D�termine s'il s'agit d'un loop ou d'un chain
			bool isLoop = false;
			if (body.GetUserData())
			{
				Entity* e = (Entity*)body.GetUserData();
				if (e->GetType() == EntityType::PolyChain)
				{
					PolyChain* pc = (PolyChain*)e;
					if (pc->GetChainType() == PolyChain::Type::Loop)
						isLoop = true;
				}
			}

			// R�cup�re le shape
			b2ChainShape *chain = (b2ChainShape*)shape;

			// Cr�e les ombres
			const float ppm = PhysicManager::GetInstance().GetPPM();
			const sf::Color &c = sf::Color::Green;
			const sf::Vector2f lpos = light->GetPosition_sf();
			for (int32 i = 0; i < chain->m_count - ((!isLoop) ? 1 : 0); ++i) // Chaque pt -> chaque arr�te
			{
				// N� pt suivant (overflow)
				int j = ((i + 1) < chain->m_count) ? (i + 1) : 0;

				// Pts formant l'arr�te
				sf::Vector2f pt1 = b22sfVec(b2Mul(t, chain->m_vertices[i]), ppm);
				sf::Vector2f pt2 = b22sfVec(b2Mul(t, chain->m_vertices[j]), ppm);

				// Ajoute les pts au VertexArray
				light->mShadowsVertexArray.append(sf::Vertex(pt1, c, mZero));
				light->mShadowsVertexArray.append(sf::Vertex(pt2, c, mZero));

				// Projette les points
				// Pt1
				float dist = sqrtf((pt2.y - lpos.y)*(pt2.y - lpos.y) + (pt2.x - lpos.x)*(pt2.x - lpos.x));
				pt2 = pt2 + (((pt2 - lpos) / dist) * float(light->mLightRadius) * 10000.f);
				light->mShadowsVertexArray.append(sf::Vertex(pt2, c, mZero));
				// Pt2
				dist = sqrtf((pt1.y - lpos.y)*(pt1.y - lpos.y) + (pt1.x - lpos.x)*(pt1.x - lpos.x));
				pt1 = pt1 + (((pt1 - lpos) / dist) * float(light->mLightRadius) * 10000.f);
				light->mShadowsVertexArray.append(sf::Vertex(pt1, c, mZero));
			}
		}
		else if (shape->GetType() == b2Shape::e_polygon) // polygon
		{
			// R�cup�re le shape
			b2PolygonShape *poly = (b2PolygonShape*) shape;

			// Cr�e les ombres
			const float ppm = PhysicManager::GetInstance().GetPPM();
			const sf::Color &c = sf::Color::Green;
			const sf::Vector2f lpos = light->GetPosition_sf();
			for (int32 i = 0; i < poly->m_count; ++i) // Chaque pt -> chaque arr�te
			{
				// N� pt suivant (overflow)
				int j = ((i + 1) < poly->m_count) ? (i + 1) : 0;

				// Pts formant l'arr�te
				sf::Vector2f pt1 = b22sfVec(b2Mul(t, poly->m_vertices[i]), ppm);
				sf::Vector2f pt2 = b22sfVec(b2Mul(t, poly->m_vertices[j]), ppm);

				// Ajoute les pts au VertexArray
				light->mShadowsVertexArray.append(sf::Vertex(pt1, c, mZero));
				light->mShadowsVertexArray.append(sf::Vertex(pt2, c, mZero));

				// Projette les points
				// Pt1
				float dist = sqrtf((pt2.y - lpos.y)*(pt2.y - lpos.y) + (pt2.x - lpos.x)*(pt2.x - lpos.x));
				pt2 = pt2 + (((pt2 - lpos) / dist) * float(light->mLightRadius) * 10000.f);
				light->mShadowsVertexArray.append(sf::Vertex(pt2, c, mZero));
				// Pt2
				dist = sqrtf((pt1.y - lpos.y)*(pt1.y - lpos.y) + (pt1.x - lpos.x)*(pt1.x - lpos.x));
				pt1 = pt1 + (((pt1 - lpos) / dist) * float(light->mLightRadius) * 10000.f);
				light->mShadowsVertexArray.append(sf::Vertex(pt1, c, mZero));
			}
		}
	}
}
void LightEngine::Clear(PointLight *light)
{
	if (!mIsActive) return;

	// Efface la casterTexture
	light->mShadowsVertexArray.clear();
}

// Cr�e les ombres
void LightEngine::CreateShadows(PointLight *light)
{
	if (!mIsActive) return;

	// Change la vue de la texture
	light->mLightTex.setView(light->view);

	// Applique le shadow shader
	mShadowShader.setParameter("renderTargetSize", light->GetBoundingBox().width);//light->mLightRadius * 2.f); // Fix ?
	mShadowShader.setParameter("lightColor", light->mLightColor);
	sf::RenderStates mShaderStates;
	mShaderStates.shader = &mShadowShader;
	light->mLightTex.clear();
	light->mLightTex.draw(light->mShadowsVertexArray, mShaderStates);
	light->mLightTex.display();
}

// G�re l'�tat du LightEngine
void LightEngine::Activate()
{
	mIsActive = true;
}
void LightEngine::Deactivate()
{
	mIsActive = false;
}
void LightEngine::SetActive(bool state)
{
	mIsActive = state;
}
bool LightEngine::IsActive() const
{
	return mIsActive;
}
