#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QMessageBox>

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
   connect(ui->ConnectionButton, &QPushButton::clicked, this, &ConnectionDialog::onConnectButtonClicked);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

QString ConnectionDialog::getServerIp() const
{
    return ui->IpEdit->text();
}

quint16 ConnectionDialog::getServerPort() const
{
    return ui->serverPortEdit->text().toUInt();
}

void ConnectionDialog::onConnectButtonClicked()
{
    // Sprawdzenie, czy pola nie są puste
    if (!ui->IpEdit->text().isEmpty() && !ui->serverPortEdit->text().isEmpty())
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, tr("Błąd"), tr("Musisz podać adres IP i port serwera."));
    }
}
