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

	// Gestion des propri�t�s
	bool IsStatic() const;
	void SetStatic(bool isStatic);
	bool IsActivated() const;
	void Activate();
	void Deactivate();
	void SetActivated(bool activated);

	// D�placement de la lumi�re
	void SetPosition(sf::Vector2f pos);
	void Move(float dx, float dy);
	void Move(const sf::Vector2f& dep);

	// Mise � jour de la position
	virtual void UpdatePosition();

	// Affichage des ombres de la texture de la lumi�re
	virtual void RenderLight() = 0;
	void DisplayTexture();
	void DrawOn(const sf::Drawable &drawable, sf::RenderStates states);

	// La lumi�re est *dans* un objet
	bool IsHidden() const;
	void IsHidden(bool hiden);
	
	// R�cup�rer l'AABB de la lumi�re
	virtual sf::FloatRect GetAABB() const = 0;
	
	// R�gle la lumi�re
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
	
	// Propri�t�s
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
