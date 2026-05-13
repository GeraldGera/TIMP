#include "singletonclient.h"
#include <QNetworkProxy>

SingletonClient* SingletonClient::p_instance = nullptr;
SingletonDestroyer SingletonClient::destroyer;

SingletonClient::SingletonClient(QObject *parent)
    : QObject(parent)
    , m_isConnected(false)
{
    mTcpSocket = new QTcpSocket(this);
    mTcpSocket->setProxy(QNetworkProxy::NoProxy);
    connect(mTcpSocket, &QTcpSocket::connected, this, &SingletonClient::onConnected);
    connect(mTcpSocket, &QTcpSocket::errorOccurred, this, &SingletonClient::onErrorOccurred);
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &SingletonClient::slotServerRead);
    mTcpSocket->connectToHost("172.20.10.5", 33333);
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
    if (mTcpSocket && m_isConnected) {
        mTcpSocket->write(query.toUtf8());
    } else {
        qDebug() << "Ошибка: нет подключения к серверу (соединение ещё не установлено или разорвано)";
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

void SingletonClient::onConnected()
{
    m_isConnected = true;
    qDebug() << "Соединение с сервером установлено";
}

void SingletonClient::onErrorOccurred(QAbstractSocket::SocketError)
{
    qDebug() << "Ошибка сокета:" << mTcpSocket->errorString();
    m_isConnected = false;
}
