#pragma once
#include <QWidget>
#include <QLabel>
#include "model/user.h"

class UserDetailWindow : public QWidget {
    Q_OBJECT
public:
    explicit UserDetailWindow(QWidget *parent = nullptr);
    void setUser(const User &u);
private:
    QLabel *labelInfo;
};
