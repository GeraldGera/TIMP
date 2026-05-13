#ifndef TASKSPAGE_H
#define TASKSPAGE_H

#include <QWidget>
#include "apiclient.h"
#include "userinfo.h"
#include <QJsonObject>

class QTextEdit;
class QPushButton;
class QSpinBox;
class QLabel;

class TasksPage : public QWidget
{
    Q_OBJECT
public:
    explicit TasksPage(ApiClient *apiClient, QWidget *parent = nullptr);

public slots:
    void onUserChanged(const UserInfo &user);

private slots:
    void onRandomTask();
    void onNumberedTask();
    void onSubmitAnswer();
    void onTaskReceived(const QJsonObject &task);
    void onAnswerResult(bool correct, const QString &message);

private:
    ApiClient *m_apiClient;
    int m_currentTaskId;
    int m_userId;

    QTextEdit *m_taskEdit;
    QTextEdit *m_answerEdit;
    QPushButton *m_randomButton;
    QSpinBox *m_taskNumberSpin;
    QPushButton *m_numberedButton;
    QPushButton *m_submitButton;
    QLabel *m_resultLabel;
};

#endif // TASKSPAGE_H
