#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "../Tools/Singleton.h"

enum class KeyState
{
	None,
	Pressed,
	Released
};

// UNDONE: Singleton car une seule fen�tre -> modifier �a :)
//? -> window dans le ctor
class InputManager : public Singleton<InputManager>
{
public:
	// Ctor & dtor
	InputManager();
	~InputManager();

	// R�glages des propri�t�s
	void SetWindow(sf::RenderWindow *window);
	void SetZoom(float zoom);
	void SetDefaultZoom(float zoom);
	void SetView(const sf::View &view);
	void SetDefaultCenter(const sf::Vector2f &center);

	// Ajout d'�v�nement
	void Update();
	void AddEvent(const sf::Event &event);
	
	// Gestion des touches espionn�es
	void AddSpyedKey(const sf::Keyboard::Key &keyCode);
	void RemoveSpyedKey(const sf::Keyboard::Key &keyCode);
	bool KeyPressed(const sf::Keyboard::Key &keyCode); // #1
	bool KeyReleased(const sf::Keyboard::Key &keyCode); // #1
	KeyState GetSpyedKeyState(const sf::Keyboard::Key &keyCode); // #1
	// #1 => R�initialise la touche

	// Acc�s aux �v�nements
	bool GetKeyState(const sf::Keyboard::Key &keyCode) const;
	sf::Vector2i GetMousePos() const;
	sf::Vector2f GetMousePosRV() const;
	sf::Vector2i GetLastMousePos() const;
	sf::Vector2f GetLastMousePosRV() const;
	bool GetLMBState() const;
	bool GetRMBState() const;
	bool GetMMBState() const;
	bool HasQuitted(); // R�initialise apr�s appel

	// Accesseurs
	float GetDefaultZoom() const;
	float GetCurrentZoom() const;
	const sf::View& GetView() const;
	const sf::Vector2f& GetDefaultCenter() const;
	
	sf::RenderWindow* GetWindow();
	const sf::RenderWindow* GetWindow() const;

private:
	// Etat
	bool mHasQuitted;

	// Fen�tre
	sf::RenderWindow *mWindow;

	// Structure sf::Event
	sf::Event mEvent;

	// Espionnage des touches
	std::map<sf::Keyboard::Key, KeyState> mSpyedKeyMap;

	// View et Zoom
	float mCurrentZoom;
	sf::View mView;
	float mDefaultZoom;
	sf::Vector2f mDefaultCenter;

	// Souris
	sf::Vector2i mMousePos;
	sf::Vector2f mMousePosRV;
	sf::Vector2i mLastMousePos;
	sf::Vector2f mLastMousePosRV;
	bool mRMBIsDown;
	bool mLMBIsDown;
	bool mMMBIsDown;

	// Pour l'�dition de texte
	bool mTipingText;
	sf::String mText;
};