#ifndef AUTHEGFORM_H
#define AUTHEGFORM_H

#include <QWidget>
#include "apiclient.h"

class QLineEdit;
class QPushButton;
class QLabel;

class AuthRegForm : public QWidget
{
    Q_OBJECT
public:
    explicit AuthRegForm(ApiClient *apiClient, QWidget *parent = nullptr);

signals:
    void userLoggedIn(const UserInfo &user);
    void userLoggedOut();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onLoginSuccess(const UserInfo &user);
    void onLoginError(const QString &message);

private:
    ApiClient *m_apiClient;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_emailEdit;
    QPushButton *m_loginButton;
    QPushButton *m_registerButton;
    QLabel *m_statusLabel;
};

#endif // AUTHEGFORM_H
