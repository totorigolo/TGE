#include "ScriptConsoleWindow.h"
#include "../../Scripts/ScriptMachine.h"
#include "../../Scripts/OutputInterfaces.h"
#include "../../Tools/Error.h"

// Ctor
ScriptConsoleWindow::ScriptConsoleWindow()
	: Window("Console Script"), mScriptMachine(nullptr), mCurrent(-1)
{
	// Rempli la fenêtre
	Fill();
	mApply = true;
}

// Actualisation
void ScriptConsoleWindow::Update()
{
	// Change le texte de la console
	mTextLabel->SetText(mText);

	// Auto-Scroll
	mScrolled->SetPlacement(sfg::ScrolledWindow::Placement::BOTTOM_LEFT);
}

// Gestion de la Machine de script
void ScriptConsoleWindow::SetScriptMachine(ScriptMachine *luaMachine)
{
	myAssert(luaMachine, "La ScriptMachine passée est invalide.");

	mScriptMachine = luaMachine;
}
ScriptMachine* ScriptConsoleWindow::GetScriptMachine()
{
	return mScriptMachine;
}

// Construit la fenêtre et les éléments
void ScriptConsoleWindow::Fill()
{
	// Widgets
	mPromptLabel = sfg::Label::Create(L"Commande :");
	mPrompt = PromptEntry::Create("");
	mReturnBtn = sfg::Button::Create(L"Entrée");
	mCloseBtn = sfg::Button::Create(L"Fermer");
	mScrolledBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	mTextLabel = sfg::Label::Create(L"");
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
	mVBox->PackEnd(mHBox, false);

	// Connecte les signaux
	mPrompt->GetSignal(PromptEntry::OnTextSent).Connect(std::bind(&ScriptConsoleWindow::OnSend, this));
	mPrompt->GetSignal(PromptEntry::OnHistoryUp).Connect(std::bind(&ScriptConsoleWindow::OnUp, this));
	mPrompt->GetSignal(PromptEntry::OnHistoryDown).Connect(std::bind(&ScriptConsoleWindow::OnDown, this));
	mReturnBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ScriptConsoleWindow::OnSend, this));
	mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ScriptConsoleWindow::OnClose, this));

	// Ajoute la Table à la fenêtre
	AddToWindow(mVBox);
}

// Callbacks
void ScriptConsoleWindow::OnSend()
{
	if (!mApply) return;

	// Récupère la commande
	sf::String commande = mPrompt->GetText();

	// Ajoute la commande au texte de la console
	if (!mText.isEmpty())
		*this << "\n";
	*this << "> " << commande << "\n";

	// Vide la zone de texte
	mPrompt->SetText(L"");

	// Ajoute la commande à l'historique
	mHistory.push_back(commande);
	++mCurrent;

	// Exécution de la commande
	if (mScriptMachine)
	{
		mScriptMachine->DoString(commande.toAnsiString(), new ScriptConsoleInterface(*this));
	}

	// Mets à jour la fenêtre
	Update();
}
void ScriptConsoleWindow::OnUp()
{
	if (!mApply) return;

	// Change la commande
	if (mCurrent < static_cast<int>(mHistory.size()) && mCurrent >= 0)
	{
		mPrompt->SetText(mHistory.at(mCurrent--));
	}
}
void ScriptConsoleWindow::OnDown()
{
	if (!mApply) return;

	// Change la commande
	if (mCurrent + 1 < static_cast<int>(mHistory.size()) && mCurrent >= 0)
	{
		mPrompt->SetText(mHistory.at(++mCurrent));
	}
	else
		mPrompt->SetText(L"");
}
