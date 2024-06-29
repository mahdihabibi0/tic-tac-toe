#ifndef SIGNUPPAGE_H
#define SIGNUPPAGE_H

#include <QDialog>

namespace Ui {
class SignupPage;
}

class SignupPage : public QDialog
{
    Q_OBJECT

public:
    explicit SignupPage(QWidget *parent = nullptr);
    ~SignupPage();

private:
    Ui::SignupPage *ui;
};

#endif // SIGNUPPAGE_H
