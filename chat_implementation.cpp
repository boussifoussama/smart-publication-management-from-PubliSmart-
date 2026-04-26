// ================================================================
// CHAT BOX REVIEWERS - IMPLEMENTATION COMPLETE
// ================================================================
void SmartMarket::setupReviewerChat()
{
    if (!ui->revv_tab_6) return;

    // Bouton Toggle Chat
    QPushButton *chatToggleBtn = new QPushButton("💬 Assistant Chat", ui->revv_tab_6);
    chatToggleBtn->setGeometry(880, 12, 180, 34);
    chatToggleBtn->setStyleSheet("QPushButton { background-color: #2563eb; color: white; border-radius: 6px; font-weight: bold; } QPushButton:hover { background-color: #1d4ed8; }");

    // Panneau Chat Principal
    QWidget *chatPanel = new QWidget(ui->revv_tab_6);
    chatPanel->setGeometry(720, 55, 340, 620);
    chatPanel->setStyleSheet("QWidget { background-color: #f8fafc; border: 1px solid #cbd5e1; border-radius: 8px; }");
    chatPanel->hide();

    // Zone historique messages
    QTextEdit *chatHistory = new QTextEdit(chatPanel);
    chatHistory->setObjectName("chatHistory");
    chatHistory->setGeometry(10, 10, 320, 520);
    chatHistory->setReadOnly(true);
    chatHistory->setStyleSheet("QTextEdit { border: none; background-color: white; }");
    chatHistory->setFont(QFont("Segoe UI", 10));

    // Champ texte message
    QLineEdit *chatInput = new QLineEdit(chatPanel);
    chatInput->setObjectName("chatInput");
    chatInput->setGeometry(10, 540, 230, 32);
    chatInput->setPlaceholderText("Posez votre question ici...");
    chatInput->setStyleSheet("QLineEdit { border: 1px solid #cbd5e1; border-radius: 4px; padding: 4px; }");

    // Bouton Envoyer
    QPushButton *sendBtn = new QPushButton("Envoyer", chatPanel);
    sendBtn->setGeometry(250, 540, 80, 32);
    sendBtn->setStyleSheet("QPushButton { background-color: #2563eb; color: white; border-radius: 4px; }");

    // Connexions
    connect(chatToggleBtn, &QPushButton::clicked, this, &SmartMarket::toggleChatPanel);
    connect(sendBtn, &QPushButton::clicked, this, &SmartMarket::sendChatMessage);
    connect(chatInput, &QLineEdit::returnPressed, this, &SmartMarket::sendChatMessage);

    // Charger historique
    loadChatHistory();

    // Message de bienvenue
    if (chatHistory->toPlainText().isEmpty()) {
        appendChatMessage("👋 Bonjour! Je suis votre assistant Reviewer. Posez-moi n'importe quelle question sur l'ajout, modification, suppression, recherche des reviewers.", false);
    }
}

void SmartMarket::toggleChatPanel()
{
    QWidget *chatPanel = ui->revv_tab_6->findChild<QWidget*>();
    if (chatPanel) {
        chatPanel->setVisible(!chatPanel->isVisible());
    }
}

void SmartMarket::sendChatMessage()
{
    QLineEdit *chatInput = ui->revv_tab_6->findChild<QLineEdit*>("chatInput");
    QTextEdit *chatHistory = ui->revv_tab_6->findChild<QTextEdit*>("chatHistory");

    if (!chatInput || !chatHistory) return;

    QString question = chatInput->text().trimmed();
    if (question.isEmpty()) return;

    // Ajouter message utilisateur
    appendChatMessage(question, true);
    chatInput->clear();

    // Reponse IA instantanée
    QString response = localReviewerAssistantReply(question);

    // Simuler "est en train d'écrire"
    QTimer::singleShot(600, this, [this, response]() {
        appendChatMessage(response, false);
        saveChatHistory();
    });
}

void SmartMarket::appendChatMessage(const QString &text, bool isUser)
{
    QTextEdit *chatHistory = ui->revv_tab_6->findChild<QTextEdit*>("chatHistory");
    if (!chatHistory) return;

    QString style = isUser
        ? "margin-left: 60px; background-color: #dbeafe; color: #1e40af; padding: 8px 12px; border-radius: 12px 12px 2px 12px;"
        : "margin-right: 60px; background-color: #f1f5f9; color: #1e293b; padding: 8px 12px; border-radius: 12px 12px 12px 2px;";

    QString html = QString("<div style='%1'>%2</div><div style='height: 8px;'></div>").arg(style, text.toHtmlEscaped().replace("\n", "<br>"));

    chatHistory->moveCursor(QTextCursor::End);
    chatHistory->insertHtml(html);
    chatHistory->verticalScrollBar()->setValue(chatHistory->verticalScrollBar()->maximum());
}

void SmartMarket::saveChatHistory()
{
    QTextEdit *chatHistory = ui->revv_tab_6->findChild<QTextEdit*>("chatHistory");
    if (!chatHistory) return;

    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/chat_history_reviewer.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << chatHistory->toHtml();
    }
}

void SmartMarket::loadChatHistory()
{
    QTextEdit *chatHistory = ui->revv_tab_6->findChild<QTextEdit*>("chatHistory");
    if (!chatHistory) return;

    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/chat_history_reviewer.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        chatHistory->setHtml(in.readAll());
    }
