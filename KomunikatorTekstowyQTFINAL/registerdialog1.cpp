#include "registerdialog1.h"
#include "ui_registerdialog1.h"
#include <QMessageBox>


registerDialog1::registerDialog1(UserManager *userManager,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerDialog1),
    userManager(userManager)
{
    ui->setupUi(this);
    connect(ui->registerButton, &QPushButton::clicked, this, &registerDialog1::onRegisterButtonClicked);
}

registerDialog1::~registerDialog1()
{
    delete ui;
}

void registerDialog1::onRegisterButtonClicked()
{
    QString nick = ui->registerNick->toPlainText();
    QString password = ui->registerPassword->text();

    if(nick.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Błąd rejestracji", "Nick i hasło nie mogą być puste.");
        return;
    }

    if(userManager->registerUser(nick, password))
    {
        QMessageBox::information(this, "Rejestracja", "Rejestracja użytkownika przebiegła pomyślnie.");
        accept();
    }
    else
    {
        QMessageBox::warning(this, "Błąd rejestracji", "Rejestracja nie powiodła się. Użytkownik o takim nicku może już istnieć.");
    }
}
