#include "MyTcpServer.h"
#include "MyThread.h"

#include <QDebug>
#include <QDateTime>
#include <QMutexLocker>

MyTcpServer::MyTcpServer(QObject* parent)
:   QTcpServer(parent)
{
    m_connectionLostTimer.setParent(this);
    m_connectionLostTimer.setInterval(1000);
    m_connectionLostTimer.setSingleShot(true);
    connect(&m_connectionLostTimer, &QTimer::timeout, this, &MyTcpServer::ConnectionLostTimerTimeout);
}

void MyTcpServer::StartServer(int port)
{
    if(listen(QHostAddress::Any, port))
        qDebug() << "Listening address: " << this->serverAddress().toString() << ", port: " << this->serverPort();
    else qDebug() << "Not listening";
}

void MyTcpServer::incomingConnection(int socketDescriptor)
{
    MyThread* thread = new MyThread(socketDescriptor, this);
    connect(thread, &MyThread::finished, thread, &MyThread::deleteLater);
    connect(thread, &MyThread::ReceivedNewData, this, &MyTcpServer::AddDataToList);
    connect(this, &MyTcpServer::destroyed, thread, &MyThread::terminate);
    thread->start();
}

void MyTcpServer::AddDataToList(QString const& data)
{
    QMutexLocker locker(&m_mutex);
    m_receiveValues.push_back(data);
    if(m_receiveValues.size() > 10) m_receiveValues.pop_front();
    emit ReceiveValuesListWasChanged(m_receiveValues);

    m_connectionLostTimer.start();
}

void MyTcpServer::ConnectionLostTimerTimeout()
{
    emit ConnectionLost("Server lost clients.");
}
