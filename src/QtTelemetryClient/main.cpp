#include "Window.h"

#include <QApplication>
#include <QWidget>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    s::Window window;
    window.show();

    return a.exec();
}
