#include "Window.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>

Window::Window(QWidget *parent)
:   QWidget(parent)
{
    m_dataList = new QListWidget(this);


    QGridLayout* mainLayout = new QGridLayout(this);

    {
        QGroupBox* listBox = new QGroupBox(this);
        listBox->setTitle("Data");

        QGridLayout* listBoxLayout = new QGridLayout(listBox);
        listBoxLayout->addWidget(m_dataList);

        mainLayout->addWidget(listBox, 0, 0, 1, 2);
    }
}

void Window::UpdateDataList(QStringList const& data)
{
    m_dataList->clear();
    m_dataList->addItems(data);
}

void Window::MessageFromMyServer(QString const& message)
{
    QMessageBox msgBox;
    msgBox.setText("Message from server: \n" + message);
    msgBox.exec();
}
