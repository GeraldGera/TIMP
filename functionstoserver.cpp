#include "functionstoserver.h"
#include <QMap>
#include <QList>
#include <QRandomGenerator>
#include <QDebug>


QString reg(const QString &login, const QString &pass)
{
    //Реализация регистрации
    return "Successful registration. Your userID: " + login;
}

QString auth(const QString &login, const QString &pass)
{
    //Реализация авторизации
    return "Successful authentication. Your userID: " + login;
}

QString getTask(const QString &userId, const int type_num)
{
    //Код по предоставлению задания для пользователя
    return QString("Here are 2 suggested tasks:\n");
}


QString getStat(const QString &userId)
{
    return QString("Here's a statistic of your past activity:\n");
}

QString parsing(const QString &message)
{
    if (message.isEmpty())
        return "ERROR: пустое сообщение";

    QStringList parts = message.trimmed().split('&', Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return "ERROR: неверный формат";

    QString cmd = parts[0].toUpper();

    if (cmd == "REG" && parts.size() >= 3)
        return reg(parts[1], parts[2]);

    if (cmd == "AUTH" && parts.size() >= 3)
        return auth(parts[1], parts[2]);

    if (cmd == "GET_TASK" && parts.size() >= 3) {
        bool ok = false;
        int type_num = parts[2].toInt(&ok);
        if (!ok)
            return "ERROR: номер задания должен быть числом";
        return getTask(parts[1], type_num);
    }

    if (cmd == "STAT" && parts.size() >= 2)
        return getStat(parts[1]);

    if (cmd == "HELP")
        return "OK: Доступные команды (разделитель &):\n"
               "REG&логин&пароль\n"
               "AUTH&логин&пароль\n"
               "GET_TASK&логин&номер_задания\n"
               "STAT&логин\n"
               "HELP\n\n"
               "Пример: GET_TASK&MyLogin&12";

    return "ERROR: неизвестная команда. Напиши HELP";
}
