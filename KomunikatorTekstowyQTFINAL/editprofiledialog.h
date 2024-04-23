#ifndef EDITPROFILEDIALOG_H
#define EDITPROFILEDIALOG_H

#include <QDialog>
#include "userManager.h"

namespace Ui
{
class editProfileDialog;
}

class editProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editProfileDialog(UserManager *userManager,QWidget *parent = nullptr);
    ~editProfileDialog();

private:
    Ui::editProfileDialog *ui;
    UserManager *userManager;

private slots:
    void on_changePasswordButton_clicked();

};

#endif // EDITPROFILEDIALOG_H
