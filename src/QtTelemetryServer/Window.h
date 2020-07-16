#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGroupBox>
#include <QListWidget>
#include <QSpinBox>
#include <QLineEdit>

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);

public slots:
    void UpdateDataList(QStringList const& data);

    void MessageFromMyServer(QString const& message);

private:
    QListWidget* m_dataList;

};

#endif // WINDOW_H
