#include "authregform.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

AuthRegForm::AuthRegForm(ApiClient *apiClient, QWidget *parent)
    : QWidget(parent)
    , m_apiClient(apiClient)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("Имя пользователя");
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Пароль");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_emailEdit = new QLineEdit(this);
    m_emailEdit->setPlaceholderText("Email");

    m_loginButton = new QPushButton("Войти", this);
    m_registerButton = new QPushButton("Зарегистрироваться", this);
    m_statusLabel = new QLabel(this);

    layout->addWidget(m_usernameEdit);
    layout->addWidget(m_passwordEdit);
    layout->addWidget(m_emailEdit);
    layout->addWidget(m_loginButton);
    layout->addWidget(m_registerButton);
    layout->addWidget(m_statusLabel);

    connect(m_loginButton, &QPushButton::clicked, this, &AuthRegForm::onLoginClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &AuthRegForm::onRegisterClicked);
    connect(m_apiClient, &ApiClient::loginSuccess, this, &AuthRegForm::onLoginSuccess);
    connect(m_apiClient, &ApiClient::loginError, this, &AuthRegForm::onLoginError);
}

void AuthRegForm::onLoginClicked()
{
    m_apiClient->login(m_usernameEdit->text(), m_passwordEdit->text());
}

void AuthRegForm::onRegisterClicked()
{
    m_apiClient->registerUser(m_usernameEdit->text(), m_emailEdit->text(), m_passwordEdit->text());
}

void AuthRegForm::onLoginSuccess(const UserInfo &user)
{
    m_statusLabel->setText("Успешный вход!");
    emit userLoggedIn(user);
}

void AuthRegForm::onLoginError(const QString &message)
{
    m_statusLabel->setText("Ошибка: " + message);
}
