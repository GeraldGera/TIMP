#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QJsonObject>
#include "userinfo.h"

class ApiClient : public QObject
{
    Q_OBJECT
public:
    explicit ApiClient(QObject *parent = nullptr);

    void login(const QString &username, const QString &password);
    void registerUser(const QString &username, const QString &email, const QString &password);
    void logout(int userId);
    void fetchTask(int taskNumber);   // -1 = случайное, иначе номер задания (1..12)
    void submitAnswer(int userId, int taskId, const QString &answer);
    void fetchUserStats(int userId);

signals:
    void loginSuccess(const UserInfo &user);
    void loginError(const QString &message);
    void taskReceived(const QJsonObject &task);
    void statsReceived(const QJsonObject &stats);
    void answerResult(bool correct, const QString &message);

private slots:
    void onMessageFromServer(const QString &message);

private:
    QString m_lastUsername;
    QString m_lastEmail;
    void sendCommand(const QString &cmd);
};

#endif // APICLIENT_H
