#pragma once

#include <SFGUI/Box.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "Window.h"
#include "../../Tools/PromptEntry.h"

class LuaMachine;
class LuaConsoleWindow : public Window
{
public:
	// Ctor
	LuaConsoleWindow();

	// Actualisation
	virtual void Update();

	// Gestion de la Machine Lua
	void SetLuaMachine(LuaMachine *luaMachine);
	LuaMachine* GetLuaMachine();

public:
	// Ajoute du texte dans la console
	template<typename T>
	LuaConsoleWindow& operator<<(const T &text)
	{
		mText += text;
		Update();
		return *this;
	}
	LuaConsoleWindow& operator<<(std::ostream& (*f)(std::ostream&))
	{
		f;
		*this << "\n";
		return *this;
	}

protected:
	// Construit la fenêtre et les éléments
	virtual void Fill();

	// Callbacks
	void OnSend();
	void OnUp();
	void OnDown();

private:
	// Machine Lua
	LuaMachine *mLuaMachine;

	// Texte de la console
	sf::String mText;
	
	// Historique
	int mCurrent;
	std::vector<sf::String> mHistory;

	/* Eléments */
	sfg::Box::Ptr mVBox;
	sfg::Box::Ptr mHBox;
	sfg::Label::Ptr mPromptLabel;
	PromptEntry::Ptr mPrompt;
	sfg::Button::Ptr mReturnBtn;
	sfg::Button::Ptr mCloseBtn;
	sfg::ScrolledWindow::Ptr mScrolled;
	sfg::Box::Ptr mScrolledBox;
	sfg::Label::Ptr mTextLabel;
};
