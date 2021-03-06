#include "ScriptConsoleWindow.h"
#include "../../Scripts/ScriptMachine.h"
#include "../../Scripts/OutputInterfaces.h"
#include "../../Tools/Error.h"

// Ctor
ScriptConsoleWindow::ScriptConsoleWindow()
        : Window("Console Script"), mScriptMachine(nullptr), mCurrent(-1) {
    // Rempli la fenêtre
    Fill();
    mApply = true;
}

// Actualisation
void ScriptConsoleWindow::Update() {
    // Ne fait rien
}

// Gestion de la Machine de script
void ScriptConsoleWindow::SetScriptMachine(ScriptMachine *luaMachine) {
    myAssert(luaMachine, "La ScriptMachine passée est invalide.");

    mScriptMachine = luaMachine;
}

ScriptMachine *ScriptConsoleWindow::GetScriptMachine() {
    return mScriptMachine;
}

// Construit la fenêtre et les éléments
void ScriptConsoleWindow::Fill() {
    // Widgets
    mPrompt = PromptEntry::Create("");
    mReturnBtn = sfg::Button::Create(L"Entrée");
    mCloseBtn = sfg::Button::Create(L"Fermer");
    mScrolledBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    mScrolled = sfg::ScrolledWindow::Create();
    mScrolled->AddWithViewport(mScrolledBox);
    mScrolled->SetRequisition(sf::Vector2f(420.f, 220.f));
    mScrolled->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
    mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    mHBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
    mVBox->PackEnd(mScrolled);
    mHBox->PackEnd(sfg::Label::Create(L"Commande : "), false);
    mHBox->PackEnd(mPrompt);
    mHBox->PackEnd(mReturnBtn, false);
    mHBox->PackEnd(mCloseBtn, false);

    mPath = sfg::Entry::Create();
    mExecuteFileBtn = sfg::Button::Create(L"Exécuter script");
    mPathHBox = sfg::Box::Create();
    mPathHBox->Pack(sfg::Label::Create(L"Fichier : "), false);
    mPathHBox->Pack(mPath);
    mPathHBox->Pack(mExecuteFileBtn, false);

    mVBox->PackEnd(mHBox, false);
    mVBox->PackEnd(mPathHBox, false);

    // Connecte les signaux
    mPrompt->GetSignal(PromptEntry::OnTextSent).Connect(std::bind(&ScriptConsoleWindow::OnSend, this));
    mPrompt->GetSignal(PromptEntry::OnHistoryUp).Connect(std::bind(&ScriptConsoleWindow::OnUp, this));
    mPrompt->GetSignal(PromptEntry::OnHistoryDown).Connect(std::bind(&ScriptConsoleWindow::OnDown, this));
    mReturnBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ScriptConsoleWindow::OnSend, this));
    mCloseBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ScriptConsoleWindow::OnClose, this));
    mExecuteFileBtn->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&ScriptConsoleWindow::OnExecuteFile, this));

    // Ajoute les widgets à la fenêtre
    AddToWindow(mVBox);
}

// Callbacks
void ScriptConsoleWindow::OnSend() {
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
    if (mScriptMachine) {
        mScriptMachine->DoString(commande.toAnsiString(), new ScriptConsoleInterface(*this));
    }

    // Mets à jour la fenêtre
    Update();
}

void ScriptConsoleWindow::OnExecuteFile() {
    if (mScriptMachine && !mPath->GetText().isEmpty()) {
        mScriptMachine->DoFile(mPath->GetText(), new ScriptConsoleInterface(*this));
    }
}

void ScriptConsoleWindow::OnUp() {
    if (!mApply) return;

    // Change la commande
    if (mHistory.size() != 0 && mCurrent >= 0) {
        mPrompt->SetText(mHistory.at(static_cast<unsigned long>(mCurrent--)));
    }
}

void ScriptConsoleWindow::OnDown() {
    if (!mApply) return;

    // Change la commande
    if (mCurrent + 1 < static_cast<int>(mHistory.size())) {
        mPrompt->SetText(mHistory.at(static_cast<unsigned long>(++mCurrent)));
    } else
        mPrompt->SetText(L"");
}

void ScriptConsoleWindow::autoscroll() {
    sfg::Adjustment::Ptr toAdjust(mScrolled->GetVerticalAdjustment());
    toAdjust->SetValue(toAdjust->GetUpper());
}
