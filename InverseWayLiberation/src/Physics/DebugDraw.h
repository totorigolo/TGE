#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "../Tools/NonCopyable.h"

class PhysicManager;
class DebugDraw : public b2Draw, public NonCopyable
{
public:
	// Ctor
	DebugDraw(PhysicManager &physicMgr);

	// Dessin
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	virtual void DrawTransform(const b2Transform& xf);
	
	void DrawPoint(const b2Vec2& p, const b2Color& color);

	void DrawContacts();

	// Accesseurs
	void SetRenderTarget(sf::RenderTarget *target) { mTarget = target; }
	sf::RenderTarget* GetRenderTarget() { return mTarget; }
	const sf::RenderTarget* GetRenderTarget() const { return mTarget; }

private:
	// Convertion
	sf::Vector2f sfVertex(const b2Vec2& vertex);
	sf::Color sfColor(const b2Color& color);

private:
	PhysicManager &mPhysicMgr;
	sf::RenderTarget *mTarget;
};
