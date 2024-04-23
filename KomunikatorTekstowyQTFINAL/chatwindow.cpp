#include "chatwindow.h"
#include "ui_chatwindow.h"
#include<QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPixmap>
#include <QShortcut>


ChatWindow::ChatWindow(int userId, const QString &contactNick,QWidget *parent, UserManager* userManager) :
    QDialog(parent), ui(new Ui::ChatWindow), userManager(userManager)
{
    ui->setupUi(this);
    QPixmap logo(":/logo12/texTTalk.png");
    if (!logo.isNull()) {
        ui->chatLabel->setPixmap(logo);
    } else {
        qDebug() << "Nie można załadować obrazu z zasobów.";
    }

    networkManager = new NetworkManager(this);
    chatSession = new ChatSession(this);

    setContactNick(contactNick);
    networkManager->connectToServer();

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Return), ui->textToSend);
    connect(shortcut, &QShortcut::activated, this, &ChatWindow::on_sendButton_clicked);
    connect(chatSession, &ChatSession::newMessage, this, &ChatWindow::sendMessageToServer);
    connect(networkManager, &NetworkManager::receivedData, this, &ChatWindow::displayMessageFromServer);

    if (userManager)
    {
        userNick = userManager->getLoggedInUserNick();
    }

    connect(ui->sendButton, &QPushButton::clicked, this, &ChatWindow::on_sendButton_clicked);
    ui->timeEdit->setDisplayFormat("HH:mm:ss");
    ui->timeEdit->setTime(QTime::currentTime());
    ui->timeEdit->setReadOnly(true);
    ui->timeEdit->setStyleSheet("QTimeEdit::up-button, QTimeEdit::down-button {width: 0px; height: 0px;}"
                                "QTimeEdit::up-arrow, QTimeEdit::down-arrow {width: 0px; height: 0px;}");

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setReadOnly(true);
    ui->dateEdit->setStyleSheet("QDateEdit::up-button, QDateEdit::down-button {width: 0px; height: 0px;}"
                                "QDateEdit::up-arrow, QDateEdit::down-arrow {width: 0px; height: 0px;}");

    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=]()
    {
    ui->timeEdit->setTime(QTime::currentTime());
    ui->dateEdit->setDate(QDate::currentDate());
    });
    timer->start(1000);
    ui->chatDialog->setReadOnly(true);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_sendButton_clicked() {
    QString messageText = ui->textToSend->toPlainText();
    if (!messageText.isEmpty())
    {
        Message message(userNick, messageText);
        sendMessageToServer(message); // Zmodyfikowane wywołanie funkcji


        ui->textToSend->clear();
    }
}

void ChatWindow::sendMessageToServer(const Message& message) {

    QJsonObject messageObject;
    messageObject["nick"] = message.getUser();
    messageObject["message"] = message.getText();
    QByteArray data = QJsonDocument(messageObject).toJson();


    networkManager->sendData(data);

    displayMessage(message);
}

void ChatWindow::displayMessageFromServer(const QByteArray &data) {
    // Deserializacja danych JSON otrzymanych z serwera
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject messageObject = doc.object();
    QString userNick = messageObject.value("nick").toString();
    QString messageText = messageObject.value("message").toString();
    QDateTime timestamp = QDateTime::currentDateTime();


    QString formattedMessage = QString("[%1] %2: %3")
    .arg(timestamp.toString("yyyy-MM-dd HH:mm:ss"))
    .arg(userNick)
    .arg(messageText);

    ui->chatDialog->append(formattedMessage);

}

void ChatWindow::displayMessage(const Message& message) {
    QString formattedMessage = QString("[%1] %2: %3")
    .arg(message.getTimeStamp())
    .arg(message.getUser())
    .arg(message.getText());

    ui->chatDialog->append(formattedMessage);

}
void ChatWindow::setContactNick(const QString &nick)
{
    QString contactNickText = "Rozmowa z " + nick;
    ui->ContactNick->setText(contactNickText);
}


