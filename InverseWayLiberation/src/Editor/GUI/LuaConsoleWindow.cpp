#include "LuaConsoleWindow.h"
#include "../../Lua/LuaMachine.h"
#include "../../Tools/Error.h"
#include "../../Lua/OutputInterfaces.h"

// Ctor
LuaConsoleWindow::LuaConsoleWindow()
	: Window("Console Lua"), mLuaMachine(nullptr), mCurrent(-1)
{
	// Rempli la fen�tre
	Fill();
	mApply = true;
}

// Actualisation
void LuaConsoleWindow::Update()
{
	// Change le texte de la console
	mTextLabel->SetText(mText);

	// Auto-Scroll
	mScrolled->SetPlacement(sfg::ScrolledWindow::Placement::BOTTOM_LEFT);
}

// Gestion de la Machine Lua
void LuaConsoleWindow::SetLuaMachine(LuaMachine *luaMachine)
{
	myAssert(luaMachine, "La LuaMachine pass�e est invalide.");

	mLuaMachine = luaMachine;
}
LuaMachine* LuaConsoleWindow::GetLuaMachine()
{
	return mLuaMachine;
}

// Construit la fen�tre et les �l�ments
void LuaConsoleWindow::Fill()
{
	// Widgets
	mPromptLabel = sfg::Label::Create("Commande :");
	mPrompt = PromptEntry::Create("");
	mReturnBtn = sfg::Button::Create("Entr�e");
	mCloseBtn = sfg::Button::Create("Fermer");
	mScrolledBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	mTextLabel = sfg::Label::Create("");
	mScrolledBox->Pack(mTextLabel);
	mScrolled = sfg::ScrolledWindow::Create();
	mScrolled->AddWithViewport(mScrolledBox);
	mScrolled->SetRequisition(sf::Vector2f(420.f, 220.f));
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	mHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	mVBox->PackEnd(mScrolled);
	mHBox->PackEnd(mPromptLabel, false);
	mHBox->PackEnd(mPrompt);
	mHBox->PackEnd(mReturnBtn, false);
	mHBox->PackEnd(mCloseBtn, false);
	mVBox->PackEnd(mHBox);

	// Connecte les signaux
	mPrompt->GetSignal(PromptEntry::OnTextSent).Connect(std::bind(&LuaConsoleWindow::OnSend, this));
	mPrompt->GetSignal(PromptEntry::OnHistoryUp).Connect(std::bind(&LuaConsoleWindow::OnUp, this));
	mPrompt->GetSignal(PromptEntry::OnHistoryDown).Connect(std::bind(&LuaConsoleWindow::OnDown, this));
	mReturnBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&LuaConsoleWindow::OnSend, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&LuaConsoleWindow::OnClose, this));

	// Ajoute la Table � la fen�tre
	AddToWindow(mVBox);
}

// Callbacks
void LuaConsoleWindow::OnSend()
{
	if (!mApply) return;

	// R�cup�re la commande
	sf::String commande = mPrompt->GetText();

	// Ajoute la commande au texte de la console
	if (!mText.isEmpty())
		*this << "\n";
	*this << "> " << commande << std::endl;

	// Vide la zone de texte
	mPrompt->SetText("");

	// Ajoute la commande � l'historique
	mHistory.push_back(commande);
	++mCurrent;

	// Ex�cution de la commande
	if (mLuaMachine)
	{
		mLuaMachine->DoString(commande.toAnsiString(), new LuaConsoleInterface(*this));
	}

	// Mets � jour la fen�tre
	Update();
}
void LuaConsoleWindow::OnUp()
{
	if (!mApply) return;

	// Change la commande
	if (mCurrent < static_cast<int>(mHistory.size()))
	{
		mPrompt->SetText(mHistory.at(mCurrent--));
	}
}
void LuaConsoleWindow::OnDown()
{
	if (!mApply) return;

	// Change la commande
	if (mCurrent + 1 < static_cast<int>(mHistory.size()))
	{
		mPrompt->SetText(mHistory.at(++mCurrent));
	}
	else
		mPrompt->SetText("");
}