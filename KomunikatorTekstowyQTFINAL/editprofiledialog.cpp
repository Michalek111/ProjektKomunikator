#include "editprofiledialog.h"
#include "ui_editprofiledialog.h"
#include "userManager.h"
#include <QMessageBox>
#include <QString>
#include <QPixmap>
#include <QLineEdit>


editProfileDialog::editProfileDialog(UserManager *userManager,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editProfileDialog),
    userManager(userManager)
{
    ui->setupUi(this);
    QPixmap logo(":/logo12/texTTalk.png");
    if (!logo.isNull()) {
        ui->editProfileLabel->setPixmap(logo);
    } else {
        qDebug() << "Nie można załadować obrazu z zasobów.";
    }

    ui->oldPassword->setEchoMode(QLineEdit::Password);
    ui->newPassword->setEchoMode(QLineEdit::Password);
    connect(ui->oldPassword, &QLineEdit::returnPressed, [this]() {
        ui->newPassword->setFocus();
    });

    connect(ui->newPassword, &QLineEdit::returnPressed, [this]() {
        ui->changePasswordButton->setFocus();
    });

    ui->oldPassword->setFocus();

}

editProfileDialog::~editProfileDialog()
{
    delete ui;
}

void editProfileDialog::on_changePasswordButton_clicked()
{
    QString oldPassword = ui->oldPassword->text();
    QString newPassword = ui->newPassword->text();

    if(userManager->changePassword(oldPassword,newPassword))
    {
        QMessageBox::information(this,"Sukces", "Hasło zostało zmienione");
        this->close();
    }
    else
    {
        QMessageBox::critical(this,"Błąd", "Zmiana hasła zakończona niepowodzeniem");
    }

}


