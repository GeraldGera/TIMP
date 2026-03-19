#include "taskspage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QJsonObject>

TasksPage::TasksPage(ApiClient *apiClient, QWidget *parent)
    : QWidget(parent)
    , m_apiClient(apiClient)
    , m_currentTaskId(-1)
    , m_userId(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_taskLabel = new QLabel("Нажмите 'Получить задание'", this);
    m_answerEdit = new QTextEdit(this);
    m_generateButton = new QPushButton("Получить случайное задание", this);
    m_submitButton = new QPushButton("Отправить ответ", this);
    m_resultLabel = new QLabel(this);

    layout->addWidget(m_taskLabel);
    layout->addWidget(m_answerEdit);
    layout->addWidget(m_generateButton);
    layout->addWidget(m_submitButton);
    layout->addWidget(m_resultLabel);

    connect(m_generateButton, &QPushButton::clicked, this, &TasksPage::onGenerateTask);
    connect(m_submitButton, &QPushButton::clicked, this, &TasksPage::onSubmitAnswer);
    connect(m_apiClient, &ApiClient::taskReceived, this, &TasksPage::onTaskReceived);
    connect(m_apiClient, &ApiClient::answerResult, this, &TasksPage::onAnswerResult);
}

void TasksPage::onUserChanged(const UserInfo &user)
{
    m_userId = user.id;
}

void TasksPage::onGenerateTask()
{
    if (m_userId <= 0) return;
    m_apiClient->fetchRandomTask(m_userId);
}

void TasksPage::onTaskReceived(const QJsonObject &task)
{
    m_currentTaskId = task["id"].toInt();
    m_taskLabel->setText(task["question"].toString());
    m_resultLabel->clear();
}

void TasksPage::onSubmitAnswer()
{
    if (m_currentTaskId == -1 || m_userId <= 0) return;
    m_apiClient->submitAnswer(m_userId, m_currentTaskId, m_answerEdit->toPlainText());
}

void TasksPage::onAnswerResult(bool correct, const QString &message)
{
    m_resultLabel->setText(message);
    if (correct) {
        m_currentTaskId = -1;
        m_taskLabel->setText("Нажмите 'Получить задание' для нового");
        m_answerEdit->clear();
    }
}
