#include "singletonclient.h"

SingletonClient* SingletonClient::p_instance = nullptr;
SingletonDestroyer SingletonClient::destroyer;

SingletonClient::SingletonClient(QObject *parent) : QObject(parent)
{
    mTcpSocket = new QTcpSocket(this);
    mTcpSocket->connectToHost("127.0.0.1", 33333);

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &SingletonClient::slotServerRead);

}

SingletonClient::~SingletonClient()
{
    if (mTcpSocket) {
        mTcpSocket->disconnectFromHost();
        mTcpSocket->deleteLater();
    }
}

SingletonClient* SingletonClient::getInstance()
{
    if (!p_instance) {
        p_instance = new SingletonClient();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

void SingletonClient::sendMessageToServer(const QString& query)
{
    if (mTcpSocket && mTcpSocket->state() == QAbstractSocket::ConnectedState) {
        mTcpSocket->write(query.toUtf8());
    } else {
        qDebug() << "Ошибка: нет подключения к серверу";
    }
}

void SingletonClient::slotServerRead()
{
    QString msg;
    while (mTcpSocket->bytesAvailable() > 0) {
        QByteArray array = mTcpSocket->readAll();
        msg.append(array);
    }
    qDebug() << "Получено от сервера:" << msg;
    emit messageFromServer(msg);
}
