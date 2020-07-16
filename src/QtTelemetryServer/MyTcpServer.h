#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QStringList>
#include <QMutex>

class MyTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit MyTcpServer(QObject* parent = nullptr);

    void StartServer(int port = 1234);

public slots:
    void AddDataToList(QString const& data);

private slots:
    void incomingConnection(int socketDescriptor) override;

    void ConnectionLostTimerTimeout();

signals:
    void ReceiveValuesListWasChanged(QStringList const& values);

    void ConnectionLost(QString const& message);

private:
    QStringList m_receiveValues;
    QMutex m_mutex;

    QTimer m_connectionLostTimer;
};


#endif // TCPSERVER_H
