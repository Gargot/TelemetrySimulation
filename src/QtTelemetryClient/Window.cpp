#include "Window.h"

#include <QDebug>
#include <QGridLayout>
#include <QMetaEnum>
#include <QGroupBox>
#include <QDateTime>

using namespace s;

Window::Window(QWidget *parent)
:   QWidget(parent)
{
    m_value = 1;

    m_sendDateButton = new QPushButton(this);
    m_sendDateButton->setText("Send current date");
    connect(m_sendDateButton, &QPushButton::clicked, this, &Window::SendCurrentDate);

    QLabel* valueFromServer = new QLabel(this);
    valueFromServer->setText("Value from server: ");

    m_recValLabel = new QLabel(this);
    m_recValLabel->resize(100, 1);

    QLabel* integerLabel = new QLabel(this);
    integerLabel->setText("Send interval: ");

    QSpinBox* integerSpinBox = new QSpinBox(this);
    integerSpinBox->setRange(0, 980);
    integerSpinBox->setSingleStep(1);
    integerSpinBox->setValue(100);
    connect(integerSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &Window::ChangeTimerInterval);

    m_socketState = new QLabel(this);

    m_serverPort = new QSpinBox(this);
    m_serverPort->setRange(0, 65535);
    m_serverPort->setSingleStep(1);
    m_serverPort->setValue(1234);

    m_serverName = new QLineEdit(this);
    m_serverName->setPlaceholderText("Server address");
    m_serverName->setText("localhost");

    QPushButton* connectButton = new QPushButton(this);
    connectButton->setText("Connect to server");
    connect(connectButton, &QPushButton::clicked, this, &Window::ConnectToServer);

    QGridLayout* mainLayout = new QGridLayout(this);

    {
        QGroupBox* connectBox = new QGroupBox(this);
        connectBox->setTitle("Server connection");

        QGridLayout* connectBoxLayout = new QGridLayout(connectBox);
        connectBoxLayout->addWidget(m_socketState, 0, 0);
        connectBoxLayout->addWidget(m_serverName, 1, 0);
        connectBoxLayout->addWidget(m_serverPort, 1, 1);
        connectBoxLayout->addWidget(connectButton, 2, 0);

        connectBox->setLayout(connectBoxLayout);

        mainLayout->addWidget(connectBox);
    }

    {
        QGroupBox* dataBox = new QGroupBox(this);
        dataBox->setTitle("Data");

        QGridLayout* dataBoxLayout = new QGridLayout(dataBox);

        dataBoxLayout->addWidget(valueFromServer, 0, 0);
        dataBoxLayout->addWidget(m_recValLabel, 0, 1);
        dataBoxLayout->addWidget(integerLabel, 1, 0);
        dataBoxLayout->addWidget(integerSpinBox, 1, 1);
        dataBoxLayout->addWidget(m_sendDateButton, 2, 0, 1, 2);

        dataBox->setLayout(dataBoxLayout);

        mainLayout->addWidget(dataBox);
    }

    m_timer = new QTimer(this);
    m_timer->setInterval(integerSpinBox->value());
    connect(m_timer, &QTimer::timeout, this, &Window::IncrementAndSend);
    m_timer->start();

    m_connectionLostTimer = new QTimer(this);
    m_connectionLostTimer->setInterval(1000);
    connect(m_connectionLostTimer, &QTimer::timeout, this, &Window::ConnectionLost);

    // tcp socket
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead , this, &Window::SocketReady);
    connect(m_socket, &QTcpSocket::stateChanged, this, &Window::SocketChangeState);
    m_socket->connectToHost(m_serverName->text(), m_serverPort->value());
    // tcp socket
}

void Window::SocketReady()
{
    m_connectionLostTimer->start();

    QByteArray data;
    data = m_socket->readAll();

    int receiveVal = data.toInt();

    if(receiveVal % 2 == 0)
        m_recValLabel->setNum(receiveVal);
    else m_recValLabel->setText("Invalid value!");

}

void Window::IncrementAndSend()
{
    if(m_socket->state() == QTcpSocket::ConnectedState)
    {
        QByteArray array;
        array += 'n';
        array += QByteArray::number(m_value);
        m_socket->write(array);
    }

    m_value += 2;
}

void Window::SendCurrentDate()
{
    if(m_socket->state() == QTcpSocket::ConnectedState)
    {
        QByteArray array;
        array += 'd';
        array += QDateTime::currentDateTime().toString();
        m_socket->write(array);
    }
}

void Window::ChangeTimerInterval(int interval)
{
    m_timer->setInterval(interval);
    m_timer->start();
}

void Window::SocketChangeState(QTcpSocket::SocketState socketState)
{
    QMetaEnum mf = QMetaEnum::fromType<QTcpSocket::SocketState>();
    QString strState = mf.valueToKey(socketState);
    m_socketState->setText("Connection state: " + strState.left(strState.length() - 5));
}

void Window::ConnectToServer()
{
    if(m_socket->state() != QTcpSocket::SocketState::ConnectingState && m_socket->state() != QTcpSocket::SocketState::ConnectedState)
        m_socket->connectToHost(m_serverName->text(), m_serverPort->value());
    else if(m_socket->peerName() != m_serverName->text() || m_socket->peerPort() != m_serverPort->value())
    {
        m_socket->disconnectFromHost();
        m_socket->connectToHost(m_serverName->text(), m_serverPort->value());
    }
}

void Window::ConnectionLost()
{
    m_socket->disconnectFromHost();
}
