#include "apiclient.h"
#include <QJsonDocument>
#include <QJsonObject>

ApiClient::ApiClient(QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &ApiClient::onReplyFinished);
}

void ApiClient::login(const QString &username, const QString &password)
{
    // Эмуляция
    if (username == "user" && password == "pass") {
        UserInfo user;
        user.id = 1;
        user.username = username;
        user.email = "user@example.com";
        emit loginSuccess(user);
    } else {
        emit loginError("Неверное имя или пароль");
    }
}

void ApiClient::registerUser(const QString &username, const QString &email, const QString &password)
{
    // Эмуляция
    UserInfo user;
    user.id = 2;
    user.username = username;
    user.email = email;
    emit loginSuccess(user);
}

void ApiClient::logout(int /*userId*/)
{
    // Ничего не делаем
}

void ApiClient::fetchRandomTask(int /*userId*/)
{
    QJsonObject task;
    task["id"] = 123;
    task["question"] = "2 + 2 = ?";
    emit taskReceived(task);
}

void ApiClient::submitAnswer(int /*userId*/, int taskId, const QString &answer)
{
    bool correct = (taskId == 123 && answer == "4");
    emit answerResult(correct, correct ? "Верно!" : "Ошибка");
}

void ApiClient::fetchUserStats(int /*userId*/)
{
    QJsonObject stats;
    stats["solved"] = 10;
    stats["total"] = 15;
    emit statsReceived(stats);
}

void ApiClient::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    // Здесь реальная обработка ответа от сервера
}
