#ifndef SINGLETONCLIENT_H
#define SINGLETONCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

class SingletonClient;


class SingletonDestroyer
{
private:
    SingletonClient* p_instance;
public:
    ~SingletonDestroyer() { delete p_instance; }
    void initialize(SingletonClient* p) { p_instance = p; }
};

class SingletonClient : public QObject
{
    Q_OBJECT
private:
    static SingletonClient* p_instance;
    static SingletonDestroyer destroyer;
    QTcpSocket* mTcpSocket;


    explicit SingletonClient(QObject *parent = nullptr);

    SingletonClient(const SingletonClient&) = delete;
    SingletonClient& operator=(const SingletonClient&) = delete;
    ~SingletonClient();

    friend class SingletonDestroyer;

public:

    static SingletonClient* getInstance();


    void sendMessageToServer(const QString& query);

signals:

    void messageFromServer(const QString& msg);

private slots:
    void slotServerRead();
};

#endif
