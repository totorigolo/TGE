#pragma once
#include <SFML/System.hpp>
#include <Box2D/Box2D.h>

// Constantes de convertion Radians -> Degrés
const float DEGREES_PER_RADIANS = 180.f / b2_pi;
const float DPR = DEGREES_PER_RADIANS;

// Constantes de convertion Degrés -> Radians
const float RADIANS_PER_DEGREES = 1.f / DPR;
const float RPD = RADIANS_PER_DEGREES;

// [min ; max]
inline int randMinMax(int min, int max)
{
	if ((max - min + 1) + min == 0) return 0;
	return abs(rand()) % (max - min + 1) + min;
}

// Retourne un element de la liste au hasard
template<typename T>
inline std::string randomElement(T list[], int count)
{
	return list[randMinMax(0, count - 1)];
}

// Renvoi le premier élément non égal à la valeur passée, sinon retourne la valeur
template<typename T>
inline T first_not_of(T list[], int count, T value)
{
	for (int i = 0; i < count; ++i)
		if (list[i] != value)
			return list[i];
	return value;
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

// Retourne le signe
template<typename T>
inline T sign(T a)
{
	return (a == T(0)) ? T(1) : a / abs(a);
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

// Convertion sf::Vector <> b2Vec2 (avec inversion y)
inline b2Vec2 sf2b2Vec(sf::Vector2f const& vector, float mpp)
{
	return b2Vec2(vector.x * mpp, - vector.y * mpp);
}
inline sf::Vector2f b22sfVec(b2Vec2 const& vector, float ppm)
{
	return sf::Vector2f(vector.x, - vector.y) * ppm;
}
inline b2Vec3 sf3b2Vec(sf::Vector3f const& vector, float mpp)
{
	return b2Vec3(vector.x * mpp, - vector.y * mpp, vector.z);
}
inline sf::Vector3f b32sfVec(b2Vec3 const& vector, float ppm)
{
	return sf::Vector3f(vector.x * ppm, - vector.y * ppm, vector.z);
}

// Convertion b2Vec2 <> b2Vec3
inline b2Vec2 getVec2(b2Vec3 const& vec3)
{
	return b2Vec2(vec3.x, vec3.y);
}
inline b2Vec3 getVec3(b2Vec2 const& vec2, float z = 0.f)
{
	return b2Vec3(vec2.x, vec2.y, z);
}
inline b2Vec3 operator+(b2Vec3 a, b2Vec2 b)
{
	return b2Vec3(a.x + b.x, a.y + b.y, a.y);
}

// Convertion sf::Vec2 <> sf::Vec3
template<typename T>
inline sf::Vector2<T> getVec2(sf::Vector3<T> const& vec3)
{
	return sf::Vector2<T>(vec3.x, vec3.y);
}
template<typename T>
inline sf::Vector3<T> getVec3(sf::Vector2<T> const& vec2, float z = 0.f)
{
	return sf::Vector3<T>(vec2.x, vec2.y, z);
}
