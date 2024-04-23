#ifndef CONTACTDIALOG_H
#define CONTACTDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QMap>
#include "userManager.h"
#include "chatwindow.h"


namespace Ui {
class contactDialog;
}

class contactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit contactDialog(UserManager* userManager,QWidget *parent = nullptr);
    ~contactDialog();

private:
    Ui::contactDialog *ui;
    UserManager* userManager ;
    QListWidget *contactsListWidget;
    QMap<int, QString> contactNicks;

public slots:
    void AddContactToList();
    void updateContactList();
    void openChatWindowWithUser(int userId);
    void removeContactFromList(int contactId);

};

#endif // CONTACTDIALOG_H
