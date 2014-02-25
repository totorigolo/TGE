#include "stdafx.h"
#include "InputManager.h"
#include "../Tools/utils.h"
#include "../Tools/Error.h"

// Ctor & dtor
InputManager::InputManager()
{
	// Etat
	mHasQuitted = false;
	mHasFocus = true;
	mHasResized = false;

	// View et Zoom
	mCurrentZoom = 1.f;

	// Souris
	mRMBIsDown = false;
	mLMBIsDown = false;
	mMMBIsDown = false;
	mLMBClicked = false;
	mRMBClicked = false;
	mMMBClicked = false;
	mLMBReleased = false;
	mRMBReleased = false;
	mMMBReleased = false;
	mMouseWheelMoved = false;
	mMouseWheelDelta = 0;

	// Pour l'édition de texte
	mTipingText = false;
}
InputManager::~InputManager()
{
}

// Réglages des propriétés
void InputManager::SetWindow(sf::RenderWindow *window)
{
	mWindow = window;
	mWindowView = mWindow->getDefaultView();
}
void InputManager::SetZoom(float zoom)
{
	if (!mWindow) return;

	mCurrentZoom = zoom;
	mView.setSize(u2f(mWindow->getSize()) * sf::Vector2f(mView.getViewport().width, mView.getViewport().height));
	mView.setCenter(mView.getCenter());
	mView.zoom(mCurrentZoom);
}
void InputManager::Zoom(float zoom)
{
	if (!mWindow) return;

	SetZoom(mCurrentZoom * zoom);
}
void InputManager::SetView(const sf::View &view)
{
	mView = view;
	SetZoom(1.f);
}
void InputManager::MoveCenter(const sf::Vector2f &dep)
{
	mView.move(dep);
}
void InputManager::SetCenter(const sf::Vector2f &center)
{
	mView.setCenter(center);
}

// Ajout d'évènement
void InputManager::Update()
{
	if (!mWindow) return;

	// Réinitialise les infos de la souris
	mLMBClicked = false;
	mRMBClicked = false;
	mMMBClicked = false;
	mLMBReleased = false;
	mRMBReleased = false;
	mMMBReleased = false;

	// Réinitialise les infos de la molette
	mMouseWheelMoved = false;
	mMouseWheelDelta = 0;

	// Traite tous les évènements
	while (mWindow->pollEvent(mEvent))
	{
		// Traite l'évènement
		AddEvent(mEvent);

		// Passe l'évènement à chaque Desktop
		for each (auto desktop in mDesktops)
		{
			desktop->HandleEvent(mEvent);
		}
	}

	if (!mHasFocus)
		return;
	
	// Retient les infos sur la souris
	mLastMousePos = mMousePos;
	mLastMousePosRV = mMousePosRV;
	mMousePos = sf::Mouse::getPosition(*mWindow);
	mMousePosRV = mWindow->mapPixelToCoords(mMousePos, mView);
	mLMBIsDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	mRMBIsDown = sf::Mouse::isButtonPressed(sf::Mouse::Right);
	mMMBIsDown = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
}
void InputManager::AddEvent(const sf::Event &event)
{
	if (!mWindow) return;

	// Fermeture de la fenêtre ou touche Echap
	if (event.type == sf::Event::Closed ||
		(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
	{
		mHasQuitted = true;
	}
	else if (event.type == sf::Event::GainedFocus)
	{
		mHasFocus = true;
	}
	else if (event.type == sf::Event::LostFocus)
	{
		mHasFocus = false;
	}

	// Gestion des clics
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
			mLMBClicked = true;
		if (event.mouseButton.button == sf::Mouse::Right)
			mRMBClicked = true;
		if (event.mouseButton.button == sf::Mouse::Middle)
			mMMBClicked = true;
	}
	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
			mLMBReleased = true;
		if (event.mouseButton.button == sf::Mouse::Right)
			mRMBReleased = true;
		if (event.mouseButton.button == sf::Mouse::Middle)
			mMMBReleased = true;
	}

	// Gestion de la molette
	if (event.type == sf::Event::MouseWheelMoved)
	{
		mMouseWheelMoved = true;
		mMouseWheelDelta = event.mouseWheel.delta;
	}
	
	// Gestion du resize
	else if (event.type == sf::Event::Resized)
	{
		mView.setSize(u2f(mWindow->getSize()) * sf::Vector2f(mView.getViewport().width, mView.getViewport().height));
		mView.zoom(mCurrentZoom);

		mWindowView.setSize(u2f(mWindow->getSize()) * sf::Vector2f(mWindowView.getViewport().width, mWindowView.getViewport().height));

		mHasResized = true;
	}

	// Espionnage des touches (doit être à la fin)
	else if ((event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && mSpyedKeyMap.size() > 0)
	{
		auto it = mSpyedKeyMap.find(event.key.code);
		if (it != mSpyedKeyMap.end())
		{
			if (event.type == sf::Event::KeyPressed)
				it->second = Pressed;

			else if (event.type == sf::Event::KeyReleased)
				it->second = Released;
		}
	}
}
	
// Gestion des touches espionnées
void InputManager::AddSpyedKey(const sf::Keyboard::Key &keyCode)
{
	mSpyedKeyMap[keyCode] = None;
}
void InputManager::RemoveSpyedKey(const sf::Keyboard::Key &keyCode)
{
	mSpyedKeyMap.erase(mSpyedKeyMap.find(keyCode));
}
bool InputManager::KeyPressed(const sf::Keyboard::Key &keyCode)
{
	if (GetSpyedKeyState(keyCode) == Pressed)
		return true;
	return false;
}
bool InputManager::KeyReleased(const sf::Keyboard::Key &keyCode)
{
	if (GetSpyedKeyState(keyCode) == Released)
		return true;
	return false;
}
KeyState InputManager::GetSpyedKeyState(const sf::Keyboard::Key &keyCode)
{
	auto it = mSpyedKeyMap.find(keyCode);

	if (it != mSpyedKeyMap.end())
	{
		auto r = it->second;
		it->second = None;
		return r;
	}

	return None;
}

// Accès aux évènements
bool InputManager::IsKeyPressed(const sf::Keyboard::Key &keyCode) const
{
	return sf::Keyboard::isKeyPressed(keyCode);
}
sf::Vector2i InputManager::GetMousePos() const
{
	return mMousePos;
}
sf::Vector2f InputManager::GetMousePosRV() const
{
	return mMousePosRV;
}
sf::Vector2i InputManager::GetLastMousePos() const
{
	return mLastMousePos;
}
sf::Vector2f InputManager::GetLastMousePosRV() const
{
	return mLastMousePosRV;
}
bool InputManager::GetLMBState() const
{
	return mLMBIsDown;
}
bool InputManager::GetRMBState() const
{
	return mRMBIsDown;
}
bool InputManager::GetMMBState() const
{
	return mMMBIsDown;
}
bool InputManager::IsLMBClicked() const
{
	return mLMBClicked;
}
bool InputManager::IsRMBClicked() const
{
	return mRMBClicked;
}
bool InputManager::IsMMBClicked() const
{
	return mMMBClicked;
}
bool InputManager::IsLMBReleased() const
{
	return mLMBReleased;
}
bool InputManager::IsRMBReleased() const
{
	return mRMBReleased;
}
bool InputManager::IsMMBReleased() const
{
	return mMMBReleased;
}
bool InputManager::GetMouseWheelState() const
{
	return mMouseWheelMoved;
}
int InputManager::GetMouseWheelDelta() const
{
	return mMouseWheelDelta;
}
bool InputManager::HasQuitted()
{
	if (mHasQuitted)
	{
		mHasQuitted = false;
		return true;
	}
	return false;
}
bool InputManager::HasFocus() const
{
	return mHasFocus;
}
bool InputManager::HasResized()
{
	if (mHasResized)
	{
		mHasResized = false;
		return true;
	}
	return false;
}

// Gestion des Desktops (GUI)
void InputManager::AddDesktop(sfg::Desktop *d)
{
	myAssert(d, "Le Desktop passé est invalide.");

	mDesktops.push_back(d);
}
void InputManager::RemoveDesktop(sfg::Desktop *d)
{
	myAssert(d, "Le Desktop passé est invalide.");

	mDesktops.remove(d);
}

// Accès relatifs à la vue
float InputManager::GetCurrentZoom() const
{
	return mCurrentZoom;
}
const sf::View& InputManager::GetView() const
{
	return mView;
}
const sf::View& InputManager::GetWindowView() const
{
	return mWindowView;
}
const sf::Vector2f& InputManager::GetCurrentCenter() const
{
	return mView.getCenter();
}

// Accès à la fenêtre
sf::RenderWindow* InputManager::GetWindow()
{
	return mWindow;
}
const sf::RenderWindow* InputManager::GetWindow() const
{
	return mWindow;
}
