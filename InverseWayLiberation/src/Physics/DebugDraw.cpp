#include "DebugDraw.h"
#include "PhysicManager.h"
#include "../Tools/utils.h"
#include "../Tools/Error.h"

#include <iostream>

// Ctor
DebugDraw::DebugDraw(PhysicManager &physicMgr)
	: mPhysicMgr(physicMgr)
{
	//AppendFlags(b2Draw::e_aabbBit);
	//AppendFlags(b2Draw::e_centerOfMassBit);
	AppendFlags(b2Draw::e_jointBit);
	//AppendFlags(b2Draw::e_pairBit);
	AppendFlags(b2Draw::e_shapeBit);
}

// Dessin
void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	if (!mTarget) return;

	sf::Color c = sfColor(color);
	sf::VertexArray va(sf::LinesStrip, vertexCount + 1U);

	for (int32 i = 0; i < vertexCount; ++i)
	{
		va[i].position = sfVertex(vertices[i]);
		va[i].color = c;
	}
	va[vertexCount].position = sfVertex(*vertices);
	va[vertexCount].color = c;

	mTarget->draw(va);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	if (!mTarget) return;
	
	DrawPolygon(vertices, vertexCount, color);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	if (!mTarget) return;

	float r = radius * mPhysicMgr.GetPPM() - 1.f; // 1.f = bordure
	sf::CircleShape cs;
	cs.setRadius(r);
	cs.setOutlineThickness(1.f);
	cs.setOrigin(sf::Vector2f(r, r));
	cs.setOutlineColor(sfColor(color));
	cs.setFillColor(sf::Color::Transparent);
	cs.setPosition(b22sfVec(center, mPhysicMgr.GetPPM()));

	mTarget->draw(cs);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	if (!mTarget) return;

	DrawCircle(center, radius, color);
	DrawSegment(center, center + radius * axis, color);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	mPhysicMgr.GetWorld()->GetContactList();

	if (!mTarget) return;

	sf::Color c = sfColor(color);
	sf::VertexArray va(sf::Lines, 2U);
	
	va[0].position = sfVertex(p1);
	va[0].color = c;
	va[1].position = sfVertex(p2);
	va[1].color = c;

	mTarget->draw(va);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	if (!mTarget) return;

	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1, p2, b2Color(1.f, 0.f, 0.f));

	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1, p2, b2Color(0.f, 1.f, 0.f));
}

void DebugDraw::DrawPoint(const b2Vec2& p, const b2Color& color)
{
	if (!mTarget) return;

	// Affichage d'un disque
	sf::CircleShape cs;
	cs.setRadius(2.f);
	cs.setFillColor(sfColor(color));
	cs.setOrigin(sf::Vector2f(2.f, 2.f));
	cs.setPosition(b22sfVec(p, mPhysicMgr.GetPPM()));
	mTarget->draw(cs);
}

void DebugDraw::DrawContacts()
{
	// Récupère les contacts
	b2Contact *contact = mPhysicMgr.GetWorld()->GetContactList();

	// Affiche les contacts
	for (; contact; contact = contact->GetNext())
	{
		// Affiche les Sensor
		if (contact->IsTouching())
		{
			if (contact->GetFixtureA()->IsSensor())
			{
				DrawPoint(contact->GetFixtureA()->GetBody()->GetWorldCenter() - b2Vec2(0.05f, 0.f), b2Color(1.f, 0.4f, 0.f));
			}
			if (contact->GetFixtureB()->IsSensor())
			{
				DrawPoint(contact->GetFixtureB()->GetBody()->GetWorldCenter() + b2Vec2(0.05f, 0.f), b2Color(1.f, 1.f, 0.f));
			}
		}

		// Affiche les points
		b2Manifold *m = contact->GetManifold();
		if (m->pointCount == 0)
			continue;

		b2WorldManifold wm;
		contact->GetWorldManifold(&wm);

		// Pour chaque point de contact
		for (int j = 0; j < m->pointCount && j < 2; ++j)
		{
			b2Vec2 p1 = wm.points[j];

			// Affiche les points
			DrawPoint(p1, b2Color(0.3f, 0.95f, 0.3f));

			// Affiche les normales
			b2Vec2 p2 = p1 + 0.3f * wm.normal;
			DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.9f));
		}
	}
}

// Convertion
sf::Vector2f DebugDraw::sfVertex(const b2Vec2& vertex)
{
	return b22sfVec(vertex, mPhysicMgr.GetPPM());
}
sf::Color DebugDraw::sfColor(const b2Color& color)
{
	return sf::Color(static_cast<sf::Uint8>(color.r * 255.f),
					 static_cast<sf::Uint8>(color.g * 255.f),
					 static_cast<sf::Uint8>(color.b * 255.f));
}
