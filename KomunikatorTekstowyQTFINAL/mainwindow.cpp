#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerdialog.h"
#include "userManager.h"
#include "userdialog.h"
#include <QPixmap>
#include <QMessageBox>
#include <QTimer>
#include <QLineEdit>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,userManager(new UserManager(this))
{
    ui->setupUi(this);
    QPixmap logo(":/logo12/texTTalk.png");
    if (!logo.isNull()) {
        ui->loginLabel->setPixmap(logo);
    } else {
        qDebug() << "Nie można załadować obrazu z zasobów.";
    }
    ui->loginPassword->setEchoMode(QLineEdit::Password);

    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(ui->loginNick, &QLineEdit::returnPressed, [this]()
            {
        ui->loginPassword->setFocus();
    });
    connect(ui->loginPassword, &QLineEdit::returnPressed, this, [this]() {
        ui->loginButton->setFocus();
    });

ui->loginNick->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginClicked()
{
    qDebug() << "Przycisk logowania został naciśnięty.";

    QString nick = ui->loginNick->text();
    QString password = ui->loginPassword->text();

    qDebug() << "Próba logowania z nick:" << nick << "i hasłem:" << password;


        if (ui->loginNick->text().isEmpty() || ui->loginPassword->text().isEmpty())
    {
        qDebug() << "Pole loginu lub hasła jest puste.";
                        ui->loginAnswer->setText("Pole loginu lub hasła jest puste.");
    }
    else
    {

        if (!userManager->loginUser(nick, password))
        {
            qDebug() << "Logowanie nieudane.";
             ui->loginAnswer->setText("Podany nick użytkownika lub hasło jest nieprawidłowe");
        }
        else
        {
            qDebug() << "Zalogowano pomyślnie. Witaj" << nick;
                    LoginAnswer("Zalogowano pomyślnie. Witaj " + nick);

                QTimer::singleShot(3000, this, [this, nick]() {

                    qDebug() << "Przenoszenie do interfejsu użytkownika.";

                        userDialog *userdialog = new userDialog(userManager, this);
                    userdialog->show();
                    this->hide();
                });
        }
    }
}

void MainWindow::onRegisterClicked()
{
    this-> hide();
    RegisterDialog registerDialog(userManager, this);
    registerDialog.exec();
    this-> show();

}



void MainWindow::LoginAnswer(const QString &answer)
{
    ui->loginAnswer->setText(answer);
}
