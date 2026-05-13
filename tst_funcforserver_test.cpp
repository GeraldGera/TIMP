#include <QtTest>
#include "../functionstoserver.h"
#include "../Singleton.h"

class FuncForServer_Test : public QObject
{
    Q_OBJECT

public:
    FuncForServer_Test();
    ~FuncForServer_Test();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testAuth_data();                               // Параметризованный тест авторизации (подробный)
    void testAuth();

    void testReg();                                     // Простые тесты
    void testGetTask();
    void testSubmit();
    void testStat();
    void testUnknownCommand();
};

FuncForServer_Test::FuncForServer_Test() {}
FuncForServer_Test::~FuncForServer_Test() {}

void FuncForServer_Test::initTestCase()
{
    qDebug() << "\n\\tTEST START\n";

    QSqlQuery query(Singleton::getInstance()->getDatabase());
    query.exec("DELETE FROM user_results");
    query.exec("DELETE FROM users");
    query.exec("DELETE FROM sqlite_sequence");
}

void FuncForServer_Test::cleanupTestCase()
{
    qDebug() << "\n\tTEST END\n";
}

void FuncForServer_Test::init()
{
    QSqlQuery query(Singleton::getInstance()->getDatabase());
    query.prepare("DELETE FROM users WHERE login = 'testuser'");
    query.exec();
    query.prepare("DELETE FROM users WHERE login = 'authuser'");
    query.exec();
}

void FuncForServer_Test::cleanup()
{
    QSqlQuery query(Singleton::getInstance()->getDatabase());
    query.prepare("DELETE FROM users WHERE login = 'testuser'");
    query.exec();
    query.prepare("DELETE FROM users WHERE login = 'authuser'");
    query.exec();
}

// ----- Тест авторизации (параметризованный)
void FuncForServer_Test::testAuth_data()
{
    QTest::addColumn<QString>("regCommand");
    QTest::addColumn<QString>("authCommand");
    QTest::addColumn<bool>("shouldSucceed");

    QTest::newRow("auth: correct password")
        << "REG&authuser&123&auth@test.com"
        << "AUTH&authuser&123" << true;

    QTest::newRow("auth: wrong password")
        << "REG&authuser&123&auth@test.com"
        << "AUTH&authuser&wrong" << false;

    QTest::newRow("auth: non-existent user")
        << ""
        << "AUTH&nonexistent&123" << false;

    QTest::newRow("auth: missing password")
        << "REG&authuser&123&auth@test.com"
        << "AUTH&authuser" << false;

    QTest::newRow("auth: empty login")
        << "REG&authuser&123&auth@test.com"
        << "AUTH&&123" << false;
}

void FuncForServer_Test::testAuth()
{
    QFETCH(QString, regCommand);
    QFETCH(QString, authCommand);
    QFETCH(bool, shouldSucceed);

    long socketId = 14523;

    if (!regCommand.isEmpty()) {
        parsing(regCommand, socketId);
    }

    QString result = parsing(authCommand, socketId);

    bool success = result.contains("Successful authentication");

    QString msg = QString("Auth: %1\nExpected: %2\nGot: %3")
                      .arg(authCommand)
                      .arg(shouldSucceed ? "SUCCESS" : "FAILURE")
                      .arg(result);

    QVERIFY2(success == shouldSucceed, msg.toUtf8().constData());
}

// ----- Тест регистрации
void FuncForServer_Test::testReg()
{
    long socketId = 1337;

    QString result = parsing("REG&testuser&123&test@mail.com", socketId);

    QVERIFY2(result.contains("Registration successful"),
             qPrintable(QString("Ожидалась успешная регистрация, получено: %1").arg(result)));
}

// ----- Тест GET_TASK
void FuncForServer_Test::testGetTask()
{
    long socketId = 12804;

    parsing("REG&testuser&123&test@mail.com", socketId);
    parsing("AUTH&testuser&123", socketId);

    QString result = parsing("GET_TASK", socketId);

    QVERIFY2(!result.isEmpty(),
             "GET_TASK не должен возвращать пустой ответ");
}

// ----- Тест SUBMIT
void FuncForServer_Test::testSubmit()
{
    long socketId = 12805;

    parsing("REG&testuser&123&test@mail.com", socketId);
    parsing("AUTH&testuser&123", socketId);

    QString result = parsing("SUBMIT&2&8", socketId);                       // Задание с id=2, правильный ответ 8

    QVERIFY2(result.contains("Correct") || result.contains("Incorrect"),
             qPrintable(QString("SUBMIT должен вернуть 'Correct' или 'Incorrect', получено: %1").arg(result)));
}

// ----- Тест статистики
void FuncForServer_Test::testStat()
{
    long socketId = 12806;

    parsing("REG&testuser&123&test@mail.com", socketId);
    parsing("AUTH&testuser&123", socketId);

    QString result = parsing("STAT", socketId);

    QVERIFY2(result.contains("||"),
             qPrintable(QString("STAT должен вернуть 'логин||решено||всего', получено: %1").arg(result)));
}

// ----- Тест неизвостной команды
void FuncForServer_Test::testUnknownCommand()
{
    long socketId = 6666;

    QString result = parsing("SOMETHING_WEIRD", socketId);

    QVERIFY2(result.contains("ERROR") || result.contains("неизвестная"),
             qPrintable(QString("Неизвестная команда должна возвращать ошибку, получено: %1").arg(result)));
}

// ----- Запуск тестов
QTEST_MAIN(FuncForServer_Test)

#include "tst_funcforserver_test.moc"
