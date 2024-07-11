#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class LoginPage;
}

class LoginPage : public QDialog
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private slots:
    void on_submitBtn_clicked();

    void on_signupBtn_clicked();

    void on_hidePassword_clicked();

signals:
    void signupButton_clicked();

    bool try_to_login(QJsonObject newUser);

    void login_successfully();

    QJsonObject get_user_information(QString username);
private:
    bool check_inputs();

    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H
