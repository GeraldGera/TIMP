#ifndef FUNCTIONSTOSERVER_H
#define FUNCTIONSTOSERVER_H

#include <QString>

QString reg(const QString &login, const QString &pass);
QString auth(const QString &login, const QString &pass);
QString getTask(const QString &userId, const int type_num);
QString getStat(const QString &userId);
QString parsing(const QString &message);

#endif // FUNCTIONSTOSERVER_H
