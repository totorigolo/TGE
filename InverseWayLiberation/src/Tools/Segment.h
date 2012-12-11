#pragma once
#include <SFML/Graphics.hpp>

inline bool intersect(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C, const sf::Vector2f& D)
{
	// Soit le segment [AB], et le segment [CD].
	// Je note I le vecteur AB, et J le vecteur CD
	sf::Vector2f I = B - A;
	sf::Vector2f J = D - C;

	// Si les vecteurs sont colinéaires, alors les droites sont parallèles, pas d'intersection.
	float col = I.x * J.y - I.y * J.x;
	if (col > -0.01f && col < 0.01f)
		return false;

	// Soit k le parametre du point d'intersection du segment CD sur la droite AB. on sera sur le segment si 0 < k < 1
	// Soit m le parametre du point d'intersection du segment AB sur la droite CD, on sera sur le segment si 0 < m < 1
	//
	// Soit P le point d'intersection
	//   P = A + k * I; // equation (1)
	//   P = C + m * J;
	//
	// D'où :
	//   A + k*I = C + m*J
	//
	// On décompose les points et vecteurs, on a :
	//   Ax + k * Ix = Cx + m * Jx
	//   Ay + k * Iy = Cy + m * Jy
	//
	// 2 équations, 2 inconnues, en résolvant, on trouve :

	float m = - (- I.x * A.y + I.x * C.y + I.y * A.x - I.y * C.x) / (I.x * J.y - I.y * J.x);
	float k = - (A.x * J.y - C.x * J.y - J.x * A.y + J.x * C.y) / (I.x * J.y - I.y * J.x);

	// (Notez que les dénominateurs sont les mêmes)

	// On vérifie que 0 < m < 1 et 0 < k < 1 --> sinon, cela veut dire que les droites s'intersectent, mais pas au niveau du segment.
	if (0.f < m && m < 1.f && 0.f < k && k < 1.f)
		return true;
	return false;
}

struct Segment;
inline bool intersect(const Segment& AB, const Segment& CD);
struct Segment
{
	Segment(sf::Vector2f a, sf::Vector2f b)
	{
		pt1 = a;
		pt2 = b;
	}

	sf::Vector2f getVector() const
	{
		return pt2 - pt1;
	}

	bool intersect(const Segment& segment)
	{
		return ::intersect(*this, segment);
	}

	sf::Vector2f pt1;
	sf::Vector2f pt2;
};

inline bool intersect(const Segment& AB, const Segment& CD)
{
	// Soit le segment [AB], et le segment [CD].
	// Je note I le vecteur AB, et J le vecteur CD
	sf::Vector2f I = AB.getVector();
	sf::Vector2f J = CD.getVector();

	// Si les vecteurs sont colinéaires, alors les droites sont parallèles, pas d'intersection.
	float col = I.x * J.y - I.y * J.x;
	if (col > -0.01f && col < 0.01f)
		return false;

	// Soit k le parametre du point d'intersection du segment CD sur la droite AB. on sera sur le segment si 0 < k < 1
	// Soit m le parametre du point d'intersection du segment AB sur la droite CD, on sera sur le segment si 0 < m < 1
	//
	// Soit P le point d'intersection
	//   P = A + k * I; // equation (1)
	//   P = C + m * J;
	//
	// D'où :
	//   A + k*I = C + m*J
	//
	// On décompose les points et vecteurs, on a :
	//   Ax + k * Ix = Cx + m * Jx
	//   Ay + k * Iy = Cy + m * Jy
	//
	// 2 équations, 2 inconnues, en résolvant, on trouve :

	float m = - (- I.x * AB.pt1.y + I.x * CD.pt1.y + I.y * AB.pt1.x - I.y * CD.pt1.x) / (I.x * J.y - I.y * J.x);
	float k = - (AB.pt1.x * J.y - CD.pt1.x * J.y - J.x * AB.pt1.y + J.x * CD.pt1.y) / (I.x * J.y - I.y * J.x);

	// (Notez que les dénominateurs sont les mêmes)

	// On vérifie que 0 < m < 1 et 0 < k < 1 --> sinon, cela veut dire que les droites s'intersectent, mais pas au niveau du segment.
	if (0.f < m && m < 1.f && 0.f < k && k < 1.f)
		return true;
	return false;
}
