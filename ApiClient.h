#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "userinfo.h"
#include <QJsonObject>

class ApiClient : public QObject
{
    Q_OBJECT
public:
    explicit ApiClient(QObject *parent = nullptr);

    void login(const QString &username, const QString &password);
    void registerUser(const QString &username, const QString &email, const QString &password);
    void logout(int userId);
    void fetchRandomTask(int userId);
    void submitAnswer(int userId, int taskId, const QString &answer);
    void fetchUserStats(int userId);

signals:
    void loginSuccess(const UserInfo &user);
    void loginError(const QString &message);
    void taskReceived(const QJsonObject &task);
    void statsReceived(const QJsonObject &stats);
    void answerResult(bool correct, const QString &message);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;
};

#endif // APICLIENT_H
