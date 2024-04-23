#include "userdialog.h"
#include "ui_userdialog.h"
#include "editprofiledialog.h"
#include <QMessageBox>
#include <QPixmap>

userDialog::userDialog(UserManager* userManager,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userDialog),
    userManager(userManager)
{
    ui->setupUi(this);
    QPixmap logo(":/logo12/texTTalk.png");
    if (!logo.isNull()) {
        ui->userLabel->setPixmap(logo);
    } else {
        qDebug() << "Nie można załadować obrazu z zasobów.";
    }


    connect(ui->contactListButton,&QPushButton::clicked,this,&userDialog::onContactListButton_clicked);
    connect(ui->editProfilButton,&QPushButton::clicked,this,&userDialog::onEditProfilButton_clicked);
    connect(ui->SingOutButton,&QPushButton::clicked,this,&userDialog::onSingOutButton_clicked);


}

userDialog::~userDialog()
{
    delete ui;
}

void userDialog::onContactListButton_clicked()
{
    this->hide();
    contactDialog contactdialog(userManager,this);
    contactdialog.updateContactList();
    contactdialog.exec();
    this->show();
}

void userDialog::onEditProfilButton_clicked()
{
    editProfileDialog editProfileDialog(userManager, this);
    editProfileDialog.exec();
}

void userDialog::onSingOutButton_clicked()
{

    this->reject();

}


