#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QTcpSocket>
#include <QSpinBox>
#include <QLineEdit>

namespace s {
    class Window;
}

class s::Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

    ~Window() = default;

private slots:
    void SocketReady();

    void IncrementAndSend();
    void SendCurrentDate();

    void ChangeTimerInterval(int interval);

    void SocketChangeState(QTcpSocket::SocketState socketState);

    void ConnectToServer();

    void ConnectionLost();

private:
    int m_value;
    QTimer* m_timer;
    QTimer* m_connectionLostTimer;

    QPushButton* m_sendDateButton;
    QLabel* m_recValLabel;

    QLabel* m_socketState;
    QSpinBox* m_serverPort;
    QLineEdit* m_serverName;

    QTcpSocket* m_socket;
};

#endif // WINDOW_H
