#include "InputManager.h"
#include "../Tools/utils.h"

// Ctor & dtor
InputManager::InputManager()
{
	// Etat
	mHasQuitted = false;
	mHasFocus = true;

	// View et Zoom
	mCurrentZoom = 1.f;
	mDefaultZoom = 1.f;

	// Souris
	mRMBIsDown = false;
	mLMBIsDown = false;
	mMMBIsDown = false;

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
}
void InputManager::SetZoom(float zoom)
{
	if (!mWindow) return;

	mCurrentZoom = zoom;
	mView.setSize(u2f(mWindow->getSize()) * sf::Vector2f(mView.getViewport().width, mView.getViewport().height));
	mView.setCenter(sf::Vector2f(0.f, 0.f));
	mView.zoom(mCurrentZoom);
}
void InputManager::SetDefaultZoom(float zoom)
{
	mDefaultZoom = zoom;
	SetZoom(zoom);
}
void InputManager::SetView(const sf::View &view)
{
	mView = view;
}
void InputManager::SetCenter(const sf::Vector2f &center)
{
	mView.setCenter(center);
}
void InputManager::SetDefaultCenter(const sf::Vector2f &center)
{
	mDefaultCenter = center;
	mView.setCenter(mDefaultCenter);
}

// Ajout d'évènement
void InputManager::Update()
{
	if (!mWindow) return;

	// Traite tous les évènements
	while (mWindow->pollEvent(mEvent))
	{
		// Traite l'évènement
		AddEvent(mEvent);
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

	// Gère le déplacement à la souris (clic molette)
	if (mMMBIsDown)
	{
		mView.move(- i2f(mMousePos - mLastMousePos) * mCurrentZoom);
	}
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
		//mHasFocus = false;
	}
		
	// Gestion du zoom et du resize
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Add)
	{
		mCurrentZoom *= 0.8f;
		mView.zoom(0.8f);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Subtract)
	{
		mCurrentZoom *= 1.2f;
		mView.zoom(1.2f);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad0)
	{
		SetZoom(mDefaultZoom);
		SetCenter(mDefaultCenter);
	}
	else if (event.type == sf::Event::Resized)
	{
		mView.setSize(u2f(mWindow->getSize()) * sf::Vector2f(mView.getViewport().width, mView.getViewport().height));
		mView.zoom(mCurrentZoom);
	}
	else if (event.type == sf::Event::MouseWheelMoved)
	{
		// Zoom
		if (event.mouseWheel.delta > 0)
		{
			mCurrentZoom *= 0.8f;
			mView.zoom(0.8f);
		}
			
		// Dézoom
		else
		{
			mCurrentZoom *= 1.2f;
			mView.zoom(1.2f);
		}
	}

	// Déplacement de la vue avec les flèches
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
	{
		mView.move(sf::Vector2f(0.f, -5.f) * mCurrentZoom);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
	{
		mView.move(sf::Vector2f(0.f, 5.f) * mCurrentZoom);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
	{
		mView.move(sf::Vector2f(-5.f, 0.f) * mCurrentZoom);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
	{
		mView.move(sf::Vector2f(5.f, 0.f) * mCurrentZoom);
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

// Accesseurs
float InputManager::GetDefaultZoom() const
{
	return mDefaultZoom;
}
float InputManager::GetCurrentZoom() const
{
	return mCurrentZoom;
}
const sf::View& InputManager::GetView() const
{
	return mView;
}
const sf::Vector2f& InputManager::GetCurrentCenter() const
{
	return mView.getCenter();
}
const sf::Vector2f& InputManager::GetDefaultCenter() const
{
	return mDefaultCenter;
}

sf::RenderWindow* InputManager::GetWindow()
{
	return mWindow;
}
const sf::RenderWindow* InputManager::GetWindow() const
{
	return mWindow;
}
