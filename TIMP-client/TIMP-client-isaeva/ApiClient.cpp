#include "apiclient.h"
#include "SingletonClient.h"   // обязательно
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

ApiClient::ApiClient(QObject *parent)
    : QObject(parent)
{
    connect(SingletonClient::getInstance(), &SingletonClient::messageFromServer,
            this, &ApiClient::onMessageFromServer);
}

void ApiClient::sendCommand(const QString &cmd)
{
    SingletonClient::getInstance()->sendMessageToServer(cmd);
}

void ApiClient::login(const QString &username, const QString &password)
{
    m_lastUsername = username;
    m_lastEmail.clear();
    sendCommand(QString("AUTH&%1&%2").arg(username, password));
}

void ApiClient::registerUser(const QString &username, const QString &email, const QString &password)
{
    m_lastUsername = username;
    m_lastEmail = email;
    sendCommand(QString("REG&%1&%2&%3").arg(username, password, email));
}

void ApiClient::logout(int /*userId*/)
{
    // Сервер не поддерживает logout
}

void ApiClient::fetchTask(int taskNumber)
{
    if (taskNumber == -1)
        sendCommand("GET_TASK");
    else
        sendCommand(QString("GET_TASK&%1").arg(taskNumber));
}

void ApiClient::submitAnswer(int /*userId*/, int taskId, const QString &answer)
{
    sendCommand(QString("SUBMIT&%1&%2").arg(taskId).arg(answer));
}

void ApiClient::fetchUserStats(int /*userId*/)
{
    sendCommand("STAT");
}

void ApiClient::onMessageFromServer(const QString &message)
{
    QString trimmed = message.trimmed();
    qDebug() << "ApiClient received:" << trimmed;

    if (trimmed.startsWith("Hello!"))
        return;

    if (trimmed.startsWith("Successful authentication")) {
        UserInfo user;
        user.id = 1;
        user.username = m_lastUsername;
        user.email = m_lastEmail;
        emit loginSuccess(user);
    }
    else if (trimmed.startsWith("Authentication failed")) {
        emit loginError(trimmed);
    }
    else if (trimmed.startsWith("Registration successful")) {
        UserInfo user;
        user.id = 1;
        user.username = m_lastUsername;
        user.email = m_lastEmail;
        emit loginSuccess(user);
    }
    else if (trimmed.startsWith("Registration failed")) {
        emit loginError(trimmed);
    }
    else if (trimmed.contains("||")) {
        int pos = trimmed.indexOf("||");
        if (pos > 0) {
            QString idStr = trimmed.left(pos);
            bool ok;
            int taskId = idStr.toInt(&ok);
            if (ok) {
                QJsonObject task;
                task["id"] = taskId;
                task["question"] = trimmed.mid(pos + 2);
                emit taskReceived(task);
                return;
            }
        }
        QStringList parts = trimmed.split("||");
        if (parts.size() >= 3) {
            QJsonObject stats;
            stats["solved"] = parts[1].toInt();
            stats["total"] = parts[2].toInt();
            emit statsReceived(stats);
        }
    }
    else if (trimmed.startsWith("Correct")) {
        emit answerResult(true, trimmed);
    }
    else if (trimmed.startsWith("Incorrect")) {
        emit answerResult(false, trimmed);
    }
    else if (trimmed.startsWith("ERROR")) {
        emit loginError(trimmed);
    }
    else {
        qDebug() << "Unhandled response:" << trimmed;
    }
}
