#ifndef SIGNUPPAGE_H
#define SIGNUPPAGE_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class SignupPage;
}

class SignupPage : public QDialog
{
    Q_OBJECT

public:
    explicit SignupPage(QWidget *parent = nullptr);
    ~SignupPage();

private slots:
    void on_submitBtn_clicked();

    void on_hidePassword_clicked();

    void on_loginBtn_clicked();
signals:
    void loginButton_clicked();

    bool try_to_signup(QJsonObject newUser);
private:
    bool check_inputs();

    Ui::SignupPage *ui;
};

#endif // SIGNUPPAGE_H
