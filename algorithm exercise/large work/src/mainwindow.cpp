#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model/seqlist.h"
#include "model/linkedlist.h"
#include "userdetailwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>
#include <chrono>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->btnRemove, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    // 新增：顺序表与链表查找按钮连接
    if (ui->btnFindSeq)
        connect(ui->btnFindSeq, &QPushButton::clicked, this, &MainWindow::findUserSeqList);
    if (ui->btnFindLink)
        connect(ui->btnFindLink, &QPushButton::clicked, this, &MainWindow::findUserLinkedList);
    if (ui->btn_findAndJump)
        connect(ui->btn_findAndJump, &QPushButton::clicked, this, &MainWindow::findAndJump);
    qDebug() << "connect status: btn_findAndJump" << (ui->btn_findAndJump != nullptr);
    // 生成与基准按钮连接
    if (ui->btn_generate)
        connect(ui->btn_generate, &QPushButton::clicked, [this](){ qDebug() << "btn_generate clicked"; generateTestUsers(10000); });
    if (ui->btn_benchmark)
        connect(ui->btn_benchmark, &QPushButton::clicked, [this](){ qDebug() << "btn_benchmark clicked"; benchmarkSearch(1000); });
    connect(ui->btnPrev, &QPushButton::clicked, this, &MainWindow::onPrevPageClicked);
    connect(ui->btnNext, &QPushButton::clicked, this, &MainWindow::onNextPageClicked);
    // 按回车也触发添加操作（方便用户直接回车提交）
    connect(ui->lineEditID, &QLineEdit::returnPressed, this, &MainWindow::onAddClicked);
    connect(ui->lineEditName, &QLineEdit::returnPressed, this, &MainWindow::onAddClicked);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onAddClicked() {
    QString idText = ui->lineEditID->text().trimmed();
    QString name = ui->lineEditName->text().trimmed();
    if(idText.isEmpty()) {
        QMessageBox::information(this, "提示", "请输入ID");
        return;
    }
    int id = idText.toInt();
    User u{id, name.toStdString()};
    seqList.add(u);
    linkedList.add(u);
    refreshTable();

    // 清空输入并给出提示
    ui->lineEditID->clear();
    ui->lineEditName->clear();
    QMessageBox::information(this, "提示", "已添加用户 " + QString::number(id));
}

void MainWindow::onRemoveClicked() {
    int id = ui->lineEditID->text().toInt();
    bool removedSeq = seqList.remove(id);
    bool removedLink = linkedList.remove(id);
    if(!removedSeq) QMessageBox::information(this, "提示", "ID不存在");
    refreshTable();
}

void MainWindow::onFindClicked() {
    int id = ui->lineEditID->text().toInt();
    User* u = seqList.find(id);
    if(u) QMessageBox::information(this, "查找结果", QString::fromStdString(u->name));
    else QMessageBox::information(this, "查找结果", "未找到");
}

// 顺序表查找（遍历 vector）
void MainWindow::findUserSeqList()
{
    QString userIDStr = ui->lineEditID->text().trimmed();
    if(userIDStr.isEmpty()){
        QMessageBox::warning(this, "提示", "请输入用户ID！");
        return;
    }
    int id = userIDStr.toInt();
    auto start = std::chrono::high_resolution_clock::now();

    bool found = false;
    auto users = seqList.list();
    for(const auto &u : users){
        if(u.id == id){ found = true; break; }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    ui->lineEditID->clear();

    QMessageBox::information(this, "顺序表查找结果",
                             QString("用户 %1 %2\n耗时: %3 微秒")
                                     .arg(userIDStr)
                                     .arg(found ? "存在" : "不存在")
                                     .arg(duration));
}

// 链表查找（遍历链表）
void MainWindow::findUserLinkedList()
{
    QString userIDStr = ui->lineEditID->text().trimmed();
    if(userIDStr.isEmpty()){
        QMessageBox::warning(this, "提示", "请输入用户ID！");
        return;
    }
    int id = userIDStr.toInt();
    auto start = std::chrono::high_resolution_clock::now();

    bool found = false;
    Node* n = linkedList.find(id);
    if(n) found = true;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    ui->lineEditID->clear();

    QMessageBox::information(this, "链表查找结果",
                             QString("用户 %1 %2\n耗时: %3 微秒")
                                     .arg(userIDStr)
                                     .arg(found ? "存在" : "不存在")
                                     .arg(duration));
}

// 查找并跳转到用户详情（ID 或 昵称）
void MainWindow::findAndJump()
{
    qDebug() << "findAndJump clicked";
    QString keyword = ui->lineEditID->text().trimmed();
    if(keyword.isEmpty()){
        QMessageBox::warning(this, "提示", "请输入用户ID或昵称！");
        return;
    }

    // 先尝试按 ID 查找
    User* pUser = nullptr;
    bool isNumber = false;
    int id = keyword.toInt(&isNumber);
    if(isNumber){
        pUser = seqList.find(id); // pointer into internal storage if exists
        if(!pUser){
            Node* n = linkedList.find(id);
            if(n) pUser = &n->data;
        }
    }

    // 如果还没找到，按昵称查找（优先 seqList，再链表）
    if(!pUser){
        std::string nameKey = keyword.toStdString();
        pUser = seqList.findByName(nameKey);
        if(!pUser){
            Node* cur = linkedList.headNode();
            while(cur){
                if(QString::fromStdString(cur->data.name) == keyword){ pUser = &cur->data; break; }
                cur = cur->next;
            }
        }
    }

    // 清空并恢复焦点
    ui->lineEditID->clear();
    ui->lineEditID->setEnabled(true);
    ui->lineEditID->setFocus();

    if(pUser){
        if(!detailWindow) detailWindow = new UserDetailWindow(this);
        detailWindow->setUser(*pUser);
        detailWindow->show();
        detailWindow->raise();
        detailWindow->activateWindow();
        qDebug() << "findAndJump: found user id" << pUser->id << "name" << QString::fromStdString(pUser->name);
    } else {
        QMessageBox::information(this, "查找结果", "未找到对应用户！");
        qDebug() << "findAndJump: not found";
    }
}

// 批量生成测试用户（同时填充顺序表与链表）
void MainWindow::generateTestUsers(int count)
{
    qDebug() << "generateTestUsers start" << count;
    seqList.clear();
    linkedList.clear();

    for(int i = 0; i < count; ++i){
        User u;
        u.id = i;
        u.name = std::string("User") + std::to_string(i);
        seqList.add(u);
        linkedList.add(u);
    }

    QMessageBox::information(this, "生成完成",
                             QString("已生成 %1 个测试用户").arg(count));
    qDebug() << "generateTestUsers done" << count;
    // 生成后刷新表格以在 UI 中展示数据
    refreshTable();
}

// 基准测试：随机查找 numTests 次，统计平均耗时
void MainWindow::benchmarkSearch(int numTests)
{
    int dataSize = seqList.size();
    if(dataSize == 0 || linkedList.size() == 0){
        QMessageBox::warning(this, "警告", "请先生成用户！");
        return;
    }

    qint64 seqTotal = 0;
    qint64 linkTotal = 0;

    for(int i = 0; i < numTests; ++i){
        int idx = QRandomGenerator::global()->bounded(dataSize);

        // 顺序表查找
        auto startSeq = std::chrono::high_resolution_clock::now();
        auto users = seqList.list();
        for(const auto &u : users){ if(u.id == idx) break; }
        auto endSeq = std::chrono::high_resolution_clock::now();
        seqTotal += std::chrono::duration_cast<std::chrono::microseconds>(endSeq - startSeq).count();

        // 链表查找
        auto startLink = std::chrono::high_resolution_clock::now();
        Node* cur = linkedList.headNode();
        while(cur){ if(cur->data.id == idx) break; cur = cur->next; }
        auto endLink = std::chrono::high_resolution_clock::now();
        linkTotal += std::chrono::duration_cast<std::chrono::microseconds>(endLink - startLink).count();
    }

    double avgSeq = double(seqTotal) / numTests;
    double avgLink = double(linkTotal) / numTests;

    QMessageBox::information(this, "查找性能对比",
                             QString("数据量: %1 用户, 测试次数: %2\n\n顺序表平均查找耗时: %3 微秒\n链表平均查找耗时: %4 微秒")
                                     .arg(dataSize)
                                     .arg(numTests)
                                     .arg(avgSeq, 0, 'f', 2)
                                     .arg(avgLink, 0, 'f', 2));
}

void MainWindow::onPrevPageClicked() {
    if(currentPage > 0) currentPage--;
    refreshTable();
}

void MainWindow::onNextPageClicked() {
    currentPage++;
    refreshTable();
}

void MainWindow::refreshTable() {
    auto list = seqList.list();
    int start = currentPage * pageSize;
    int end = std::min((int)list.size(), start + pageSize);
    int rows = std::max(0, end - start);
    ui->tableWidget->setRowCount(rows);
    for(int i=start;i<end;i++){
        ui->tableWidget->setItem(i-start,0,new QTableWidgetItem(QString::number(list[i].id)));
        ui->tableWidget->setItem(i-start,1,new QTableWidgetItem(QString::fromStdString(list[i].name)));
    }
}
