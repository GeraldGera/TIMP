#include "profilepage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QJsonObject>
#include <QShowEvent>

ProfilePage::ProfilePage(ApiClient *apiClient, QWidget *parent)
    : QWidget(parent)
    , m_apiClient(apiClient)
    , m_userId(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_statsLabel = new QLabel("Загрузка статистики...", this);
    layout->addWidget(m_statsLabel);

    connect(m_apiClient, &ApiClient::statsReceived, this, &ProfilePage::onStatsReceived);
}

void ProfilePage::onUserChanged(const UserInfo &user)
{
    m_userId = user.id;
    if (m_userId > 0) {
        m_apiClient->fetchUserStats(0);
    }
}

void ProfilePage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (m_userId > 0) {
        m_apiClient->fetchUserStats(0);
    }
}

void ProfilePage::onStatsReceived(const QJsonObject &stats)
{
    int solved = stats.value("solved").toInt();
    int total = stats.value("total").toInt();
    double rate = (total > 0) ? (100.0 * solved / total) : 0.0;
    QString login = stats.value("login").toString();
    if (!login.isEmpty()) {
        m_statsLabel->setText(QString("Пользователь: %1\nРешено задач: %2 из %3 (%4%)")
                                  .arg(login).arg(solved).arg(total).arg(rate, 0, 'f', 1));
    } else {
        m_statsLabel->setText(QString("Решено задач: %1 из %2 (%3%)")
                                  .arg(solved).arg(total).arg(rate, 0, 'f', 1));
    }
}
