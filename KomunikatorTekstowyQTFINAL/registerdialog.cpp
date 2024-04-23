#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QMessageBox>
#include <QPixmap>
#include <QTimer>
#include <QLineEdit>

RegisterDialog::RegisterDialog(UserManager *userManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog),
    userManager(userManager)
{
    ui->setupUi(this);
    QPixmap logo(":/logo12/texTTalk.png");
    if (!logo.isNull()) {
        ui->registerLabel->setPixmap(logo);
    } else {
        qDebug() << "Nie można załadować obrazu z zasobów.";
    }
    ui->registerPassword->setEchoMode(QLineEdit::Password);


    connect(ui->registerNick, &QLineEdit::returnPressed, [this]()
            {
                ui->registerPassword->setFocus();
            });
    connect(ui->registerPassword, &QLineEdit::returnPressed, this, [this]() {
        ui->registerButton->setFocus();
    });
    ui->registerNick->setFocus();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::registerAnswer(const QString &answer)
{
    ui->registerAnswer->setText(answer);
}

void RegisterDialog::on_registerButton_clicked()
{
    QString nick = ui->registerNick->text();
    QString password = ui->registerPassword->text();

    if (nick.isEmpty() || password.isEmpty())
    {
        registerAnswer("Nick i hasło nie mogą być puste");
        return;
    }

    if (!userManager->registerUser(nick, password))
    {
        registerAnswer("Błąd, użytkownik o takim nicku już istnieje.");
    }
    else
    {
        registerAnswer("Rejestracja użytkownika przebiegła pomyślnie");
        QTimer::singleShot(3000, this, &RegisterDialog::accept);
    }
}





