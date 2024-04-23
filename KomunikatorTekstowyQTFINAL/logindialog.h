#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>


using namespace std;




class loginDialog : public QDialog
{
    Q_OBJECT
public:
   explicit loginDialog(QWidget *parent = nullptr);

 private slots:
     void onLoginClicked();
 private:
     QTextEdit* loginNick;
     QLineEdit* loginPassword;
     QPushButton* loginButton;
     QPushButton* registerButton;
};

#endif // LOGINDIALOG_H
