#include "functionstoserver.h"
#include "Singleton.h"
#include <QMap>
#include <QList>
#include <QRandomGenerator>
#include <QDebug>


QString reg(const QString &login, const QString &pass, const QString &email, long socketDesc){
    if (Singleton::getInstance()->is_reg_ok(login, pass, email, socketDesc))
        return "Registration successful";
    else
        return "Registration failed";
}

QString auth(const QString &login, const QString &pass, long socketDesc){
    if (Singleton::getInstance()->is_auth_ok(login, pass, socketDesc))
        return "Successful authentication ";
    else
        return "Authentication failed";
}

QString getTask(long socketDesc, int type_num){
    QString task = Singleton::getInstance()->get_task_for_user(socketDesc, type_num);
    if (task.startsWith("ERROR"))
        return task;
    return task;
}

QString submitAnswer(long socketDesc, int taskId, const QString &answer){
    bool correct = Singleton::getInstance()->submit_answer(socketDesc, taskId, answer);
    if (correct)
        return "Correct. You're breathtaking!";
    else
        return "Incorrect. Try again.";
}

QString getStat(long socketDesc){
    return Singleton::getInstance()->get_curr_user_stat(socketDesc);
}

QString getAllStat(long socketDesc)
{
    return Singleton::getInstance()->get_all_stat(socketDesc);
}

QString parsing(const QString &message, long socketDesc)
{
    if (message.isEmpty())
        return "ERROR: пустое сообщение";

    QStringList parts = message.trimmed().split('&', Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return "ERROR: неверный формат";

    QString cmd = parts[0].toUpper();

    if (cmd == "REG" && parts.size() >= 4)
        return reg(parts[1], parts[2], parts[3], socketDesc);

    if (cmd == "AUTH" && parts.size() >= 3)
        return auth(parts[1], parts[2], socketDesc);

    if (cmd == "GET_TASK") {
        int type_num = -1;              // по умолчанию - любой тип
        if (parts.size() >= 2) {
            bool ok;
            type_num = parts[1].toInt(&ok);
            if (!ok) type_num = -1;
        }
        return getTask(socketDesc, type_num);
    }

    if (cmd == "SUBMIT" && parts.size() >= 3) {
        bool ok;
        int taskId = parts[1].toInt(&ok);
        if (!ok)
            return "ERROR: task ID must be a number";
        return submitAnswer(socketDesc, taskId, parts[2]);
    }

    if (cmd == "STAT")
        return getStat(socketDesc);

    if (cmd == "ALLSTAT")
        return getAllStat(socketDesc);

    if (cmd == "HELP")
        return "OK: Доступные команды (разделитель &):\n"
               "REG&логин&пароль&email\n"
               "AUTH&логин&пароль\n"
               "GET_TASK\n"
               "GET_TASK&номер_задания\n"
               "SUBMIT&номер_задания&ответ\n"
               "STAT\n"
               "ALLSTAT (admin only)\n"
               "HELP\n\n"
               "Пример: GET_TASK&12";

    return "ERROR: неизвестная команда. Напиши HELP";
}
