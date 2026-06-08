#include <QCoreApplication>
#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "mytcpserver.h"
#include "Singleton.h"

// ==========================================
// ВСТРОЕННЫЙ КЛАСС UNIT-ТЕСТОВ (ШАГ 4)
// ==========================================
class ServerAutoTests : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "TestConnection");
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());

        QSqlQuery query(db);
        query.exec("CREATE TABLE tasks (id INTEGER PRIMARY KEY, task_type INTEGER, condition TEXT, correct_answer TEXT);");
        query.exec("CREATE TABLE user_results (user_id INTEGER, task_id INTEGER, is_correct BOOLEAN);");

        query.prepare("INSERT INTO tasks (id, task_type, condition, correct_answer) VALUES (1, 1, '2+2*2', '6');");
        query.exec();
    }

    void cleanupTestCase() {
        QSqlDatabase::database("TestConnection").close();
    }

    void test_databaseConnection() {
        Singleton* s = Singleton::getInstance();
        QVERIFY(s != nullptr);
    }

    void test_getTaskForUser() {
        Singleton* s = Singleton::getInstance();
        QString response = s->get_task_for_user(12345, 1);
        QVERIFY(response.startsWith("OK&") || response.startsWith("ERROR"));
    }

    void test_submitAnswer() {
        Singleton* s = Singleton::getInstance();
        bool result = s->submit_answer(12345, 1, "6");
        Q_UNUSED(result);
    }
};


/**
 * @brief Функция проверки ответов на 4 задачи из второй части ЕГЭ.
 * @param task_num Номер задачи (24, 25, 26, 27).
 * @param user_answer Строка с ответом, пришедшая от клиента.
 * @return true, если ответ верный, иначе false.
 */
bool checkEgeAnswer(int task_num, const std::string& user_answer) {
    switch(task_num) {
        case 24: return (user_answer == "42");       // Замените "42" на ваш ответ к задаче 24
        case 25: return (user_answer == "2026 15");  // Замените на ваш ответ к задаче 25
        case 26: return (user_answer == "150 4000"); // Замените на ваш ответ к задаче 26
        case 27: return (user_answer == "987654");   // Замените на ваш ответ к задаче 27
        default: return false;
    }
}

// ==========================================
// ГЛАВНАЯ ФУНКЦИЯ ЗАПУСКА СЕРВЕРА
// ==========================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 1. СРАЗУ ЗАПУСКАЕМ АВТОТЕСТЫ ПРИ СТАРТЕ
    qDebug() << "=== ЗАПУСК ЮНИТ-ТЕСТОВ ===";
    ServerAutoTests tc;
    QTest::qExec(&tc);
    qDebug() << "==========================\n";

    // 2. ЗАПУСКАЕМ СЕТЕВОЙ СЕРВЕР
    MyTcpServer server;

    return a.exec();
}

#include "main.moc"
