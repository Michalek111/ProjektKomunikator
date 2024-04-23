#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include "userManager.h"
#include "contactdialog.h"


namespace Ui {
class userDialog;
}

class userDialog : public QDialog
{
    Q_OBJECT

public:
    explicit userDialog(UserManager* userManager,QWidget *parent = nullptr);
    ~userDialog();


private:
    Ui::userDialog *ui;
    UserManager* userManager;



private slots:
    void onContactListButton_clicked();
    void onEditProfilButton_clicked();
    void onSingOutButton_clicked();

};

#endif // USERDIALOG_H
