#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include "apiclient.h"
#include "userinfo.h"
#include <QJsonObject>

class QLabel;

class ProfilePage : public QWidget
{
    Q_OBJECT
public:
    explicit ProfilePage(ApiClient *apiClient, QWidget *parent = nullptr);

public slots:
    void onUserChanged(const UserInfo &user);

private slots:
    void onStatsReceived(const QJsonObject &stats);

private:
    ApiClient *m_apiClient;
    int m_userId;
    QLabel *m_statsLabel;
};

#endif // PROFILEPAGE_H
