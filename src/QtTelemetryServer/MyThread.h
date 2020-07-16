#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QTimer>

class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(int socketDescriptor, QObject *parent);

private slots:
    void SocketReady();

    void IncrementAndSend();

signals:
    void ReceivedNewData(QString const& data);

private:
    int m_value;
    QTimer m_sendValueTimer;

    QTcpSocket* m_socket;
};

#endif // MYTHREAD_H
