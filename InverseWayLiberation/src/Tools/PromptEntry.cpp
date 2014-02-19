#include "PromptEntry.h"

sfg::Signal::SignalID PromptEntry::OnTextSent = 0;
sfg::Signal::SignalID PromptEntry::OnHistoryUp = 0;
sfg::Signal::SignalID PromptEntry::OnHistoryDown = 0;

PromptEntry::Ptr PromptEntry::Create(const sf::String& text)
{
	PromptEntry::Ptr ptr(new PromptEntry);
	ptr->SetText(text);
	return ptr;
}

void PromptEntry::HandleKeyEvent(sf::Keyboard::Key key, bool press)
{
	sfg::Entry::HandleKeyEvent(key, press);

	if (!HasFocus() || !press) return;

	switch (key)
	{
	case sf::Keyboard::Return:
		GetSignals().Emit(OnTextSent);
		break;
	case sf::Keyboard::Up:
		GetSignals().Emit(OnHistoryUp);
		break;
	case sf::Keyboard::Down:
		GetSignals().Emit(OnHistoryDown);
		break;
	}
}
