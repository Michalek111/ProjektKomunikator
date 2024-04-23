#include "contactdialog.h"
#include "ui_contactdialog.h"
#include <QLayoutItem>
#include <QLayout>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QPixmap>
#include <QScrollArea>
#include <QLineEdit>


contactDialog::contactDialog(UserManager* userManager, QWidget *parent) :
    QDialog(parent), userManager(userManager),
    ui(new Ui::contactDialog)
{
    ui->setupUi(this);
    QPixmap logo(":/logo12/texTTalk.png");
    if (!logo.isNull()) {
        ui->ContactLabel->setPixmap(logo);
    } else {
        qDebug() << "Nie można załadować obrazu z zasobów.";
    }

    connect(ui->addContact, &QPushButton::clicked, this, &contactDialog::AddContactToList);
    connect(ui->addContact, &QPushButton::clicked, this, &contactDialog::updateContactList);
    connect(ui->removeContact, &QPushButton::clicked, this, [=]() {
        int contactId = ui->removeIpContact->text().toInt();
        removeContactFromList(contactId);
    });

    connect(ui->addIpContact, &QLineEdit::returnPressed, [this]() {
        ui->addContact->setFocus();
    });

    connect(ui->removeIpContact, &QLineEdit::returnPressed, [this]() {
        ui->removeContact->setFocus();
    });

    updateContactList();
}

void contactDialog::AddContactToList()
{
    bool ok;
    int contactId = ui->addIpContact->text().toInt(&ok);

    if (!ok || contactId <= 0) {
        ui->contactAnswer->setText("Wprowadzono niepoprawne ID kontaktu.");
        return;
    }
    QString loggedInUserNick1 = userManager->getLoggedInUserNick(); // Pobierz nick zalogowanego użytkownika
    int loggedInUserId = userManager->getLoggedInUserID(loggedInUserNick1);
    qDebug() << loggedInUserId;
    if (loggedInUserId == contactId) {
        ui->contactAnswer->setText("Nie możesz dodać samego siebie.");
        return;
    }

    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isOpen()) {
        ui->contactAnswer->setText("Błąd połączenia z bazą danych.");
        return;
    }

    QSqlQuery query(database);
    query.prepare("SELECT nick FROM Uzytkownicy1 WHERE id = :id");
    query.bindValue(":id", contactId);

    if (!query.exec()) {
        ui->contactAnswer->setText("Błąd odczytu z bazy danych.");
        return;
    }

    if (!query.next()) {
        ui->contactAnswer->setText("Nie ma takiego użytkownika.");
        return;
    }

    QString loggedInUserNick = userManager->getLoggedInUserNick();
    if (userManager->isContactExists(loggedInUserNick, contactId)) {
        ui->contactAnswer->setText("Ten użytkownik jest już na Twojej liście kontaktów.");
        return;
    }

    userManager->addContact(loggedInUserNick, contactId);
    ui->contactAnswer->clear();
}
contactDialog::~contactDialog()
{
    delete ui;
}



void contactDialog::openChatWindowWithUser(int contactId)
{
    QString contactNick = contactNicks.value(contactId);
    if (contactNick.isEmpty())
    {
        qDebug() << "Błąd: Nick użytkownika nie znaleziony dla ID:" << contactId;
            return;
    }

    ChatWindow *chatWindow = new ChatWindow(contactId, contactNick, this, userManager);
    chatWindow->setModal(true);
    chatWindow->show();
}

void contactDialog::updateContactList()
{
    QVector<int> contacts = userManager->getContactsForUser(userManager->getLoggedInUserNick());

    // Usuń poprzednie widgety z ui->verticalLayout
    QLayoutItem* item;
    while ((item = ui->verticalLayout->takeAt(0)) != nullptr)
    {
            delete item->widget(); // usunięcie widgetu
            delete item; // usunięcie pozycji layoutu
    }

    // Stwórz kontener dla przycisków kontaktów
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);

    QSqlDatabase database = QSqlDatabase::database();
    for (int contactId : contacts)
    {
            QSqlQuery query(database);
            query.prepare("SELECT nick FROM Uzytkownicy1 WHERE id = :id");
            query.bindValue(":id", contactId);

            QString contactName;
            if (query.exec() && query.next())
            {
                contactName = query.value(0).toString();
                contactNicks[contactId] = contactName;
            }
            else
            {
                qDebug() << "Błąd odczytu z bazy danych dla ID:" << contactId;
                    continue;
            }

            QPushButton* contactButton = new QPushButton(contactName, container);
            layout->addWidget(contactButton);

            connect(contactButton, &QPushButton::clicked, this, [this, contactId]()
                    {
                        openChatWindowWithUser(contactId);
                    });
    }

    // Stwórz QScrollArea i umieść w nim kontener z przyciskami
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Dodaj QScrollArea do ui->verticalLayout
    ui->verticalLayout->addWidget(scrollArea);
}


void contactDialog::removeContactFromList(int contactId) {
    QString loggedInUserNick = userManager->getLoggedInUserNick();
    if (!userManager->isContactExists(loggedInUserNick, contactId))
    {
        qDebug() << "Kontakt o ID:" << contactId << " nie istnieje w Twojej liście kontaktów.";
            ui->contactAnswer->setText("Wprowadzono niepoprawne ID kontaktu.");
        return;
    }
    userManager->removeContact(loggedInUserNick, contactId);
    ui->contactAnswer->clear();

    updateContactList();
}


