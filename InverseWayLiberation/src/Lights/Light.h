#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "../Physics/Bodies/Body.h"

class Body;
class Light : public sf::Drawable
{
public:
	// Ctor & Dtor
	Light(sf::Vector2f pos, float radius, bool isStatic, bool isActivated = true, Body* emitter = nullptr);
	Light(float x, float y, float radius, bool isStatic, bool isActivated = true, Body* emitter = nullptr);
	virtual ~Light();

	// Gestion des propriétés
	bool IsStatic() const;
	void SetStatic(bool isStatic);
	bool IsActivated() const;
	void Activate();
	void Deactivate();
	void SetActivated(bool activated);

	// Déplacement de la lumière
	void SetPosition(sf::Vector2f pos);
	void Move(float dx, float dy);
	void Move(const sf::Vector2f& dep);

	// Mise à jour de la position
	virtual void UpdatePosition();

	// Affichage des ombres de la texture de la lumière
	virtual void RenderLight() = 0;
	void DisplayTexture();
	void DrawOn(const sf::Drawable &drawable, sf::RenderStates states);

	// La lumière est *dans* un objet
	bool IsHidden() const;
	void IsHidden(bool hiden);
	
	// Récupérer l'AABB de la lumière
	virtual sf::FloatRect GetAABB() const = 0;
	
	// Règle la lumière
	virtual void SetRadius(float radius);
	float GetRadius() const;

	// Accesseurs
	inline Body* GetEmitter()
	{
		return mEmitter;
	}
	inline const Body* GetEmitter() const
	{
		return mEmitter;
	}
	inline const sf::Vector2f& GetPosition() const
	{
		return mPosition;
	}

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
	// Propriétés
	bool mIsActivated;
	float mRadius;
	bool mIsStatic;
	sf::Vector2f mPosition;
	Body* mEmitter;
	sf::Vector2f mPosRelEmitter;

	// Etat
	bool mIsHidden;

	// Texture de rendu
	sf::View mView;
	sf::Sprite mSprite;
	sf::RenderTexture mRenderTexture;

private:
};
