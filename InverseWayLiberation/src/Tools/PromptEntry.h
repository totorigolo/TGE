#pragma once
#include <memory>
#include <SFGUI/Entry.hpp>

class PromptEntry : public sfg::Entry
{
public:
	typedef std::shared_ptr<PromptEntry> Ptr;
	typedef std::shared_ptr<const PromptEntry> PtrConst;

	static Ptr Create(const sf::String& text = L"");

	virtual void HandleKeyEvent(sf::Keyboard::Key key, bool press);

public:
	static sfg::Signal::SignalID OnTextSent;
	static sfg::Signal::SignalID OnHistoryUp;
	static sfg::Signal::SignalID OnHistoryDown;

protected:
	PromptEntry(){}
};
