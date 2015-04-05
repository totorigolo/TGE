#pragma once
#include "../Tools/Singleton.h"

class PointLight;
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
	void DrawPhysicalHull(PointLight *light, const b2Body& body);
	void Clear(PointLight *light);

	// Cr�e les ombres
	void CreateShadows(PointLight *light);

	// G�re l'�tat du LightEngine
	void Activate();
	void Deactivate();
	void SetActive(bool state);
	bool IsActive() const;

private:
	// Acc�s de PointLight
	friend class PointLight;

	// State of the LightEngine
	bool mIsActive;

	// States
	sf::RenderStates addStates;

	// Shader
	sf::Shader mShadowShader;

	// sf::Vector particuliers pour le ShadowVertexArray
	/* L'id�e est que quand on dessine les ombres, on mets les coordonn�es de texture � 0, alors que
	 * le fond dessin� sous les ombres sur toute la surface de la lumi�re a ses coordonn�es � 1.
	 * De cette fa�on on sait tr�s facilement si on est dans une ombre ou non.
	 */
	sf::Vector2f mZero;
	sf::Vector2f mOne;
};

