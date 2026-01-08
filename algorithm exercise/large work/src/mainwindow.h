// mainwindow.h
#pragma once
#include <vector>
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
    void removeUser();           // 删除用户

private:
    Ui::MainWindow *ui;

    SeqList seqList;
    LinkedList linkedList;

        // ---------- 分页相关（新增） ----------
    int currentPage = 0;                 // 当前页号
    static constexpr int pageSize = 10;  // 每页 10 条
    std::vector<User> currentUsers;      // 当前显示的数据源
    void refreshUserTable(); 
    void updatePageInfo();            // 分页刷新（无参）
    // ------------------------------------

    // ---------- 夜间模式相关 ----------
    bool darkModeEnabled = false;
    void applyDarkTheme();
    void applyLightTheme();
    // --------------------------------
};
