#include "userdetailwindow.h"
#include <QVBoxLayout>
#include <QString>

UserDetailWindow::UserDetailWindow(QWidget *parent)
    : QWidget(parent), labelInfo(new QLabel(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(labelInfo);
    setLayout(layout);
    setWindowTitle("用户详情");
}

void UserDetailWindow::setUser(const User &u)
{
    QString info = QString("ID: %1\n昵称: %2")
                       .arg(u.id)
                       .arg(QString::fromStdString(u.name));
    labelInfo->setText(info);
}
