#include "MyThread.h"

#include <QHostAddress>
#include <QDateTime>
#include <QMutex>

MyThread::MyThread(int socketDescriptor, QObject *parent)
:   QThread(parent)
{
    m_socket = new QTcpSocket(this);
    m_socket->setSocketDescriptor(socketDescriptor);
    connect(m_socket, &QTcpSocket::readyRead, this, &MyThread::SocketReady);
    connect(m_socket, &QTcpSocket::disconnected, this, &MyThread::quit);

    m_sendValueTimer.setParent(this);
    m_sendValueTimer.setInterval(50);
    connect(&m_sendValueTimer, &QTimer::timeout, this, &MyThread::IncrementAndSend);
    m_sendValueTimer.start();

    m_value = 2;
}

void MyThread::SocketReady()
{
    QByteArray data;
    data = m_socket->readAll();

    QString strVal = "";
    if(data.front() == 'n')
    {
        data.remove(0, 1);

        int receiveVal = data.toInt();

        if(receiveVal % 2 != 0)
            strVal = QString::number(receiveVal);
        else strVal = "invalid value!";
    }
    else if(data.front() == 'd')
    {
         data.remove(0, 1);

         QDateTime dt = QDateTime::fromString(data);
         strVal = dt.toString();
    }

    if(strVal.size() != 0)
    {
        strVal = "Value: " + strVal;
        strVal += "    Client address: " + m_socket->peerAddress().toString();
        strVal += "    Port: " + QString::number(m_socket->peerPort());
        emit ReceivedNewData(strVal);
    }
}

void MyThread::IncrementAndSend()
{
    if(m_socket->state() == QTcpSocket::ConnectedState)
        m_socket->write(QByteArray::number(m_value));
    m_value += 2;
}