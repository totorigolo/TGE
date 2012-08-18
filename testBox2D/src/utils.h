#pragma once
#include <SFML/System.hpp>
#include <Box2D/Box2D.h>

// [min ; max]
inline int randMinMax(int min, int max)
{
	if ((max - min + 1) + min == 0) return 0;
	return abs(rand()) % (max - min + 1) + min;
}

// Retourne le minimum
template<typename T>
inline T min(T a, T b)
{
	return (a < b) ? a : b;
}

// Retourne le maximum
template<typename T>
inline T max(T a, T b)
{
	return (a > b) ? a : b;
}

// Convertion sf::Vector <> b2Vec2 (sans convertion)
inline b2Vec2 sf2b2Vec(sf::Vector2f const& vector)
{
	return b2Vec2(vector.x, vector.y);
}
inline sf::Vector2f b22sfVec(b2Vec2 const& vector)
{
	return sf::Vector2f(vector.x, vector.y);
}

// Converti les sf::Vector2 entre eux
inline sf::Vector2f u2f(sf::Vector2u vec)
{
	return sf::Vector2f(static_cast<float>(vec.x), static_cast<float>(vec.y));
}
inline sf::Vector2i u2i(sf::Vector2u vec)
{
	return sf::Vector2i(static_cast<int>(vec.x), static_cast<int>(vec.y));
}
inline sf::Vector2u f2u(sf::Vector2f vec)
{
	return sf::Vector2u(static_cast<unsigned int>(vec.x), static_cast<unsigned int>(vec.y));
}
inline sf::Vector2i f2i(sf::Vector2f vec)
{
	return sf::Vector2i(static_cast<int>(vec.x), static_cast<int>(vec.y));
}
inline sf::Vector2f i2f(sf::Vector2i vec)
{
	return sf::Vector2f(static_cast<float>(vec.x), static_cast<float>(vec.y));
}
inline sf::Vector2u i2u(sf::Vector2i vec)
{
	return sf::Vector2u(static_cast<unsigned int>(vec.x), static_cast<unsigned int>(vec.y));
}

// Surcharges d'opérateurs pour les sf::Vector
template<typename T, typename N>
sf::Vector2<T> operator+(sf::Vector2<T> a, N b)
{
	return sf::Vector2<T>(a.x + b, a.y + b);
}
template<typename T, typename N>
sf::Vector2<T> operator-(sf::Vector2<T> a, N b)
{
	return sf::Vector2<T>(a.x - b, a.y - b);
}
template<typename T, typename N>
sf::Vector2<T> operator*(sf::Vector2<T> a, N b)
{
	return sf::Vector2<T>(a.x * b, a.y * b);
}
template<typename T, typename N>
sf::Vector2<T> operator/(sf::Vector2<T> a, N b)
{
	return sf::Vector2<T>(a.x / b, a.y / b);
}

template<typename T>
sf::Vector2<T> operator+(sf::Vector2<T> a, sf::Vector2<T> b)
{
	return sf::Vector2<T>(a.x + b.x, a.y + b.y);
}
template<typename T>
sf::Vector2<T> operator-(sf::Vector2<T> a, sf::Vector2<T> b)
{
	return sf::Vector2<T>(a.x - b.x, a.y - b.y);
}
template<typename T>
sf::Vector2<T> operator*(sf::Vector2<T> a, sf::Vector2<T> b)
{
	return sf::Vector2<T>(a.x * b.x, a.y * b.y);
}
template<typename T>
sf::Vector2<T> operator/(sf::Vector2<T> a, sf::Vector2<T> b)
{
	return sf::Vector2<T>(a.x / b.x, a.y / b.y);
}
