#pragma once
#include "../Tools/Singleton.h"

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <list>
#include <map>

enum KeyState
{
	None,
	Pressed,
	Released
};

// UNDONE: Singleton car une seule fenêtre -> modifier ça :)
//? -> window dans le ctor
class InputManager : public Singleton<InputManager>
{
protected:
	friend class Singleton<InputManager>;

	// Ctor & dtor
	InputManager();
	~InputManager();

public:
	// Réglages des propriétés
	void SetWindow(sf::RenderWindow *window);
	void SetZoom(float zoom);
	void SetDefaultZoom(float zoom);
	void SetView(const sf::View &view);
	void SetCenter(const sf::Vector2f &center);
	void SetDefaultCenter(const sf::Vector2f &center);

	// Ajout d'évènement
	void Update();
	void AddEvent(const sf::Event &event);
	
	// Gestion des touches espionnées
	void AddSpyedKey(const sf::Keyboard::Key &keyCode);
	void RemoveSpyedKey(const sf::Keyboard::Key &keyCode);
	bool KeyPressed(const sf::Keyboard::Key &keyCode); // #1
	bool KeyReleased(const sf::Keyboard::Key &keyCode); // #1
	KeyState GetSpyedKeyState(const sf::Keyboard::Key &keyCode); // #1
	// #1 => Réinitialise la touche

	// Accès aux évènements
	bool IsKeyPressed(const sf::Keyboard::Key &keyCode) const;
	sf::Vector2i GetMousePos() const;
	sf::Vector2f GetMousePosRV() const;
	sf::Vector2i GetLastMousePos() const;
	sf::Vector2f GetLastMousePosRV() const;
	bool GetLMBState() const;
	bool GetRMBState() const;
	bool GetMMBState() const;
	bool HasQuitted(); // Réinitialise après appel
	bool HasFocus() const;

	// Gestion des Desktops (GUI)
	void AddDesktop(sfg::Desktop *d);
	void RemoveDesktop(sfg::Desktop *d);

	// Accesseurs
	float GetDefaultZoom() const;
	float GetCurrentZoom() const;
	const sf::View& GetView() const;
	const sf::Vector2f& GetCurrentCenter() const;
	const sf::Vector2f& GetDefaultCenter() const;
	
	sf::RenderWindow* GetWindow();
	const sf::RenderWindow* GetWindow() const;

private:
	// Etat
	bool mHasQuitted;
	bool mHasFocus;

	// Fenêtre
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

	// Pour l'édition de texte
	bool mTipingText;
	sf::String mText;

	// Liste des Desktop à qui partager les events
	std::list<sfg::Desktop*> mDesktops;
};