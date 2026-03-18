#ifndef FUNCTIONSTOSERVER_H
#define FUNCTIONSTOSERVER_H

#include <QString>

QString reg(const QString &login, const QString &pass, const QString &email, long socketDesc);
QString auth(const QString &login, const QString &pass, long socketDesc);
QString getTask(long socketDesc, int type_num);
QString submitAnswer(long socketDesc, int taskId, const QString &answer);
QString getStat(long socketDesc);
QString getAllStat(long socketDesc);
QString parsing(const QString &message, long socketDesc);

#endif // FUNCTIONSTOSERVER_H
