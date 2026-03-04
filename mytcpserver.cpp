#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

MyTcpServer::~MyTcpServer()
{
    for(QTcpSocket* socket : mTcpSockets.values()) {
        socket->close();
    }
    mTcpServer->close();
}

void MyTcpServer::slotNewConnection()
{
    QTcpSocket* clientSocket = mTcpServer->nextPendingConnection();
    int desc = clientSocket->socketDescriptor();
    
    mTcpSockets.insert(desc, clientSocket);

    clientSocket->write("Hello! Multi-client server is ready!\r\n");

    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
}

void MyTcpServer::slotServerRead()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    while(clientSocket->bytesAvailable() > 0)
    {
        QByteArray array = clientSocket->readAll();
        qDebug() << array;
        clientSocket->write(array);
    }
}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        int desc = clientSocket->socketDescriptor();
        mTcpSockets.remove(desc);
        clientSocket->deleteLater();
    }
}