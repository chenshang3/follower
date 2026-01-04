#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <chrono>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 生成测试数据
    connect(ui->btnGen, &QPushButton::clicked, this, &MainWindow::generateUsers);

    // 顺序表查找
    connect(ui->btnFindSeq, &QPushButton::clicked, this, &MainWindow::findBySeqList);

    // 链表查找
    connect(ui->btnFindLink, &QPushButton::clicked, this, &MainWindow::findByLinkedList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshUserTable(const std::vector<User>& users)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(users.size());

    for (int row = 0; row < (int)users.size(); ++row) {
        const User& u = users[row];

        ui->tableWidget->setItem(
            row, 0,
            new QTableWidgetItem(QString::number(u.id))
        );

        ui->tableWidget->setItem(
            row, 1,
            new QTableWidgetItem(QString::fromStdString(u.name))
        );
    }
}

// -------------------- 生成测试数据 --------------------
void MainWindow::generateUsers()
{
    seqList.clear();
    linkedList.clear();

    const int N = 1000;

    for (int i = 1; i < N; ++i) {
        User u;
        u.id = i;
        u.name = "user_" + std::to_string(i);

        seqList.add(u);
        linkedList.add(u);
    }

    refreshUserTable(seqList.list());

    QMessageBox::information(
        this,
        "完成",
        QString("成功生成 %1 条测试数据").arg(N)
    );
}

// -------------------- 顺序表查找 --------------------
void MainWindow::findBySeqList()
{
    bool ok;
    int targetId = ui->lineEditID->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "错误", "请输入合法 ID");
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();
    User* u = seqList.find(targetId);
    auto end = std::chrono::high_resolution_clock::now();

    if (!u) {
        QMessageBox::information(this, "结果", "未找到用户");
        return;
    }

    // 刷新表格（顺序表）
    auto users = seqList.list();
    refreshUserTable(users);

    // 定位到对应行
    for (int i = 0; i < (int)users.size(); ++i) {
        if (users[i].id == targetId) {
            ui->tableWidget->selectRow(i);
            break;
        }
    }

    auto cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    QMessageBox::information(this, "顺序表查找", QString("查找耗时：%1 ns").arg(cost));

}


// -------------------- 链表查找 --------------------
void MainWindow::findByLinkedList()
{
    QString input = ui->lineEditID->text().trimmed();
    ui->lineEditID->clear();

    if (input.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入 ID");
        return;
    }

    bool ok = false;
    int targetId = input.toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "错误", "链表仅支持按 ID 查找");
        return;
    }

    QElapsedTimer timer;
    timer.start();

    Node* node = linkedList.find(targetId);

    qint64 elapsed = timer.nsecsElapsed();

    auto users = linkedList.list();
    refreshUserTable(users);  // 🔹 刷新表格

    if (node) {
        // 定位行
        for (int i = 0; i < (int)users.size(); ++i) {
            if (users[i].id == targetId) {
                ui->tableWidget->selectRow(i);
                break;
            }
        }

        QMessageBox::information(
            this,
            "链表查找成功",
            QString("ID: %1\nName: %2\n耗时: %3 ns")
                .arg(node->data.id)
                .arg(QString::fromStdString(node->data.name))
                .arg(elapsed)
        );
    } else {
        QMessageBox::information(
            this,
            "链表查找失败",
            QString("未找到，耗时 %1 ns").arg(elapsed)
        );
    }
}
