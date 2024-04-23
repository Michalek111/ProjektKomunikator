#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();
    QString getServerIp()const;
    quint16 getServerPort() const;
    void onConnectButtonClicked();

private:
    Ui::ConnectionDialog *ui;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *connectButton;
    QLabel *portLabel;
    QLabel *ipLabel;
};

#endif // CONNECTIONDIALOG_H
