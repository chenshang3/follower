// mainwindow.h
#pragma once

#include <QMainWindow>
#include "model/seqlist.h"
#include "model/linkedlist.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generateUsers();        // 生成测试数据
    void findBySeqList();        // 顺序表查找
    void findByLinkedList();     // 链表查找

private:
    Ui::MainWindow *ui;

    SeqList seqList;
    LinkedList linkedList;

    // ---------- 夜间模式相关 ----------
    bool darkModeEnabled = false;
    void applyDarkTheme();
    void applyLightTheme();
    // --------------------------------

    void refreshUserTable(const std::vector<User>& users);
};
