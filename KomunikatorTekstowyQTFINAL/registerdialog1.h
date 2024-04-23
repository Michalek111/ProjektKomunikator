#ifndef REGISTERDIALOG1_H
#define REGISTERDIALOG1_H

#include <QDialog>
#include "UserManager.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class registerDialog1;
}
QT_END_NAMESPACE
class registerDialog1 : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog1(UserManager *userManager, QWidget *parent = nullptr);
    ~registerDialog1();


private slots:
    void onRegisterButtonClicked();

private:
    Ui::registerDialog1 *ui;
    UserManager *userManager;
};

#endif // REGISTERDIALOG1_H
