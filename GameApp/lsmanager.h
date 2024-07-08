#ifndef LSMANAGER_H
#define LSMANAGER_H
#include "signuppage.h"
#include "loginpage.h"
#include <QObject>

class LSManager : public QObject
{
    Q_OBJECT
public:
    LSManager();

    SignupPage s;

    LoginPage l;

signals:
    bool  defult_login_signal(QJsonObject user);

public slots:
    void manage();

private slots:
    void open_signup_page();

    void open_login_page();
private:
    bool try_to_login_by_default();
};

#endif // LSMANAGER_H
