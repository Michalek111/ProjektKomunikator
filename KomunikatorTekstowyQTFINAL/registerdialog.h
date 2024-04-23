#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "UserManager.h"


namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(UserManager *userManager, QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_registerButton_clicked();
    void registerAnswer(const QString &answer);

private:
    Ui::RegisterDialog *ui;
    UserManager *userManager;
};

#endif // REGISTERDIALOG_H
