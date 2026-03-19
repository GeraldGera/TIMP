#ifndef USERINFO_H
#define USERINFO_H

#include <QString>

struct UserInfo
{
    int id = 0;
    QString username;
    QString email;
    bool isValid() const { return id > 0; }
};

#endif // USERINFO_H
