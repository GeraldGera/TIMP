#include <QCryptographicHash>
#include "Singleton.h"

Singleton * Singleton::p_instance = nullptr;
SingletonDestroyer Singleton::destroyer;

SingletonDestroyer::~SingletonDestroyer() { delete p_instance; }

void SingletonDestroyer::initialize(Singleton *p){ p_instance = p; }

Singleton::Singleton(){

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/User/Desktop/MPU/TIMP/Actual stuff/SQLite.db");     //Относительный путь БД, когда это отправится в Docker, пока отладка в QT - лучше абсолютный, там что-то про создание нескольких экземпляров при каждой сборке

    if(!db.open()){
        qDebug()<<db.lastError().text();
        return;
    }

QSqlQuery pragma(db);
pragma.exec("PRAGMA foreign_keys = ON;");

QSqlQuery query(db);
// Таблица users
query.exec("CREATE TABLE IF NOT EXISTS users ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "login VARCHAR(50) UNIQUE NOT NULL, "
           "password VARCHAR(255) NOT NULL, "
           "email VARCHAR(100), "
           "role VARCHAR(20) DEFAULT 'user', "
           "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
           "socket_id VARCHAR(20))");

// Таблица tasks
query.exec("CREATE TABLE IF NOT EXISTS tasks ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "task_text TEXT NOT NULL,"
           "correct_answer VARCHAR(255) NOT NULL,"
           "task_num INTEGER CHECK (task_num BETWEEN 1 and 12))");

// Таблица user_results
query.exec("CREATE TABLE IF NOT EXISTS user_results ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "user_id INTEGER NOT NULL, "
           "task_id INTEGER NOT NULL, "
           "is_correct BOOLEAN NOT NULL, "
           "attempt_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
           "user_answer VARCHAR(255), "
           "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE, "
           "FOREIGN KEY (task_id) REFERENCES tasks(id) ON DELETE CASCADE)");
}


Singleton::~Singleton() { db.close(); }

Singleton* Singleton::getInstance(){
    if (!p_instance)
    {
        p_instance = new Singleton();
        destroyer.initialize(p_instance);
    }
    return p_instance;
 }

static QString hashPassword(const QString &password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromLatin1(hash.toHex());
}

bool Singleton::is_auth_ok(QString login, QString pass, long socketDesc){
    QSqlQuery query(db);
    query.prepare("SELECT id FROM users WHERE login = :login AND password = :pass");
    query.bindValue(":login", login);
    //query.bindValue(":pass", pass);
    query.bindValue(":pass", hashPassword(pass));
    if (query.exec() && query.next()) {
        // Обновляем socket_id
        QSqlQuery update(db);
        update.prepare("UPDATE users SET socket_id = :sid WHERE login = :login");
        update.bindValue(":sid", QString::number(socketDesc));
        update.bindValue(":login", login);
        update.exec();
        return true;
    }
    return false;
 }

bool Singleton::is_reg_ok(QString login, QString pass, QString email, long socketDesc){
    QSqlQuery check(db);
    check.prepare("SELECT id FROM users WHERE login = :login");
    check.bindValue(":login", login);
    if (check.exec() && check.next()) {
        return false;                       //Логин занят
    }

    QSqlQuery insert(db);
    insert.prepare("INSERT INTO users (login, password, email, role, socket_id) "
                   "VALUES (:login, :pass, :email, 'user', :sid)");
    insert.bindValue(":login", login);
    //insert.bindValue(":pass", pass);
    insert.bindValue(":pass", hashPassword(pass));        //Хэширование
    insert.bindValue(":email", email);
    insert.bindValue(":sid", QString::number(socketDesc));
    return insert.exec();
}

QString Singleton::get_task_for_user(long socketDesc, int taskId) {
    //user_id по socket_id
    QSqlQuery userQuery(db);
    userQuery.prepare("SELECT id FROM users WHERE socket_id = :sid");
    userQuery.bindValue(":sid", QString::number(socketDesc));
    if (!userQuery.exec() || !userQuery.next()) {
        return "ERROR: user not found";
    }
    int userId = userQuery.value(0).toInt();

    // Формирование запроса на получение задания
    QSqlQuery taskQuery(db);
    if (taskId == -1) {
        taskQuery.prepare(                              // Любое нерешённое задание
            "SELECT id, task_text FROM tasks "
            "WHERE id NOT IN ("
            "SELECT task_id FROM user_results WHERE user_id = :uid AND is_correct = 1"
            ") "
            "ORDER BY RANDOM() LIMIT 1"
            );
        taskQuery.bindValue(":uid", userId);
    }
    else {
        taskQuery.prepare(                              // Задание указанного типа (task_num)
            "SELECT id, task_text FROM tasks "
            "WHERE task_num = :type AND id NOT IN ("
            "SELECT task_id FROM user_results WHERE user_id = :uid AND is_correct = 1"
            ") "
            "ORDER BY RANDOM() LIMIT 1"
            );
        taskQuery.bindValue(":type", taskId);
        taskQuery.bindValue(":uid", userId);
    }

    if (!taskQuery.exec() || !taskQuery.next())
        return "ERROR: no available tasks";

    int finTaskId = taskQuery.value(0).toInt();
    QString taskText = taskQuery.value(1).toString();
    return QString::number(finTaskId) + "||" + taskText;
}

bool Singleton::submit_answer(long socketDesc, int taskId, QString userAnswer){
    QSqlQuery userQuery(db);
    userQuery.prepare("SELECT id FROM users WHERE socket_id = :sid");
    userQuery.bindValue(":sid", QString::number(socketDesc));
    if (!userQuery.exec() || !userQuery.next())
        return false;
    int userId = userQuery.value(0).toInt();

    //правильный ответ
    QSqlQuery taskQuery(db);
    taskQuery.prepare("SELECT correct_answer FROM tasks WHERE id = :tid");
    taskQuery.bindValue(":tid", taskId);
    if (!taskQuery.exec() || !taskQuery.next())
        return false;
    QString correct = taskQuery.value(0).toString();

    userAnswer.replace(',', '.');
    userAnswer.remove(' ');
    
    bool isCorrect = (userAnswer == correct);

    //запись результата
    QSqlQuery insert(db);
    insert.prepare("INSERT INTO user_results (user_id, task_id, is_correct, user_answer) "
                   "VALUES (:uid, :tid, :correct, :answer)");
    insert.bindValue(":uid", userId);
    insert.bindValue(":tid", taskId);
    insert.bindValue(":correct", isCorrect ? 1 : 0);
    insert.bindValue(":answer", userAnswer);
    insert.exec();

    return isCorrect;
}

QString Singleton::get_curr_user_stat(long socketDesc){
    QSqlQuery query(db);
    query.prepare(
        "SELECT login, "
        "SUM(CASE WHEN is_correct = 1 THEN 1 ELSE 0 END) as correct, "
        "COUNT(user_results.id) as total "
        "FROM users LEFT JOIN user_results ON users.id = user_results.user_id "
        "WHERE users.socket_id = :sid "
        "GROUP BY users.id"
        );
    query.bindValue(":sid", QString::number(socketDesc));
    if (query.exec() && query.next()) {
        QString login = query.value(0).toString();
        int correct = query.value(1).toInt();
        int total = query.value(2).toInt();
        return QString("%1||%2||%3").arg(login).arg(correct).arg(total);
    }
    return "ERROR: user not found";
}

QString Singleton::get_all_stat(long socketDesc){
    QSqlQuery check(db);
    check.prepare("SELECT role FROM users WHERE socket_id = :sid");
    check.bindValue(":sid", QString::number(socketDesc));
    if (!check.exec() || !check.next() || check.value(0).toString() != "admin")
        return "Access denied";

    QSqlQuery query(db);
    query.exec("SELECT login, "
               "SUM(CASE WHEN is_correct = 1 THEN 1 ELSE 0 END) as correct, "
               "COUNT(user_results.id) as total "
               "FROM users LEFT JOIN user_results ON users.id = user_results.user_id "
               "GROUP BY users.id");
    QStringList result;
    while (query.next()) {
        result << QString("%1||%2||%3").arg(query.value(0).toString())
        .arg(query.value(1).toInt())
            .arg(query.value(2).toInt());
    }
    return result.join("&&");
}
void Singleton::clear_socket_id(long socketDesc){
    QSqlQuery query(db);
    query.prepare("UPDATE users SET socket_id = NULL WHERE socket_id = :sid");
    query.bindValue(":sid", QString::number(socketDesc));
    query.exec();
}
