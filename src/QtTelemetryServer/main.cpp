#include "Window.h"
#include "MyTcpServer.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Window w;

    MyTcpServer server(&w);

    QObject::connect(&server, &MyTcpServer::ReceiveValuesListWasChanged, &w, &Window::UpdateDataList);
    QObject::connect(&server, &MyTcpServer::ConnectionLost, &w, &Window::MessageFromMyServer);

    server.StartServer();

    w.show();

    return a.exec();
}
