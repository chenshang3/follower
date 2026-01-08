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
    
    // 分页按钮
    connect(ui->btnNext, &QPushButton::clicked, this, [this]() {
        int totalPages = (currentUsers.size() + pageSize - 1) / pageSize;
        if (currentPage < totalPages - 1) {
            ++currentPage;
            refreshUserTable();
        }
    });

    connect(ui->btnPrev, &QPushButton::clicked, this, [this]() {
        if (currentPage > 0) {
            --currentPage;
            refreshUserTable();
        }
    });
    
    // 夜间模式切换
    connect(ui->btnTheme, &QPushButton::clicked, this, [this]() {
        darkModeEnabled = !darkModeEnabled;

        if (darkModeEnabled) {
            applyDarkTheme();
            ui->btnTheme->setText(QStringLiteral("🌙 夜间模式"));
        } else {
            applyLightTheme();
            ui->btnTheme->setText(QStringLiteral("☀️ 日间模式"));
        }
    });
    
    // 更新页面显示
    updatePageInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshUserTable()
{
    int total = static_cast<int>(currentUsers.size());
    int start = currentPage * pageSize;
    int end = std::min(start + pageSize, total);

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(end - start);

    for (int i = start; i < end; ++i) {
        int row = i - start;
        const User& u = currentUsers[i];

        ui->tableWidget->setItem(
            row, 0,
            new QTableWidgetItem(QString::number(u.id))
        );

        ui->tableWidget->setItem(
            row, 1,
            new QTableWidgetItem(QString::fromStdString(u.name))
        );
    }
    
    // 更新页面信息
    updatePageInfo();
}

void MainWindow::updatePageInfo()
{
    int totalPages = (currentUsers.size() + pageSize - 1) / pageSize;
    if (totalPages == 0) totalPages = 1;
    
    int currentPageDisplay = currentPage + 1; // 显示给用户的页码从1开始
    
    ui->labelPageInfo->setText(
        QString("第 %1/%2 页，共 %3 条")
            .arg(currentPageDisplay)
            .arg(totalPages)
            .arg(currentUsers.size())
    );
    
    // 更新按钮状态
    ui->btnPrev->setEnabled(currentPage > 0);
    ui->btnNext->setEnabled(currentPage < totalPages - 1);
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

    currentUsers = seqList.list();
    currentPage = 0;
    refreshUserTable();

    QMessageBox::information(
        this,
        "完成",
        QString("成功生成 %1 条测试数据").arg(N-1)
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
    currentUsers = seqList.list();  // 修正这里：使用 seqList.list()
    
    // 找到目标在列表中的位置，计算应显示在第几页
    int foundIndex = -1;
    for (int i = 0; i < (int)currentUsers.size(); ++i) {
        if (currentUsers[i].id == targetId) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex != -1) {
        currentPage = foundIndex / pageSize;
        refreshUserTable();
        
        // 定位到对应行（在当前页中的行号）
        int rowInPage = foundIndex % pageSize;
        ui->tableWidget->selectRow(rowInPage);
    }

    auto cost = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    QMessageBox::information(this, "顺序表查找", 
        QString("查找耗时：%1 ns\n已跳转到第%2页")
            .arg(cost)
            .arg(currentPage + 1)); // 页码从1开始显示
}

// -------------------- 链表查找 --------------------
void MainWindow::findByLinkedList()
{
    QString input = ui->lineEditID->text().trimmed();

    if (input.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入 ID");
        return;
    }

    bool ok = false;
    int targetId = input.toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "错误", "请输入合法的数字 ID");
        return;
    }

    QElapsedTimer timer;
    timer.start();

    Node* node = linkedList.find(targetId);
    qint64 elapsed = timer.nsecsElapsed();

    if (node) {
        // 刷新表格（链表）
        currentUsers = linkedList.list();  // 修正这里：使用 linkedList.list()
        
        // 找到目标在列表中的位置，计算应显示在第几页
        int foundIndex = -1;
        for (int i = 0; i < (int)currentUsers.size(); ++i) {
            if (currentUsers[i].id == targetId) {
                foundIndex = i;
                break;
            }
        }
        
        if (foundIndex != -1) {
            currentPage = foundIndex / pageSize;
            refreshUserTable();
            
            // 定位到对应行（在当前页中的行号）
            int rowInPage = foundIndex % pageSize;
            ui->tableWidget->selectRow(rowInPage);
        }

        QMessageBox::information(
            this,
            "链表查找成功",
            QString("ID: %1\nName: %2\n耗时: %3 ns\n已跳转到第%4页")
                .arg(node->data.id)
                .arg(QString::fromStdString(node->data.name))
                .arg(elapsed)
                .arg(currentPage + 1)  // 页码从1开始显示
        );
    } else {
        QMessageBox::information(
            this,
            "链表查找失败",
            QString("未找到，耗时 %1 ns").arg(elapsed)
        );
    }
}

void MainWindow::applyDarkTheme()
{
    QPalette dark;

    dark.setColor(QPalette::Window, QColor(53, 53, 53));
    dark.setColor(QPalette::WindowText, Qt::white);

    dark.setColor(QPalette::Base, QColor(35, 35, 35));
    dark.setColor(QPalette::AlternateBase, QColor(53, 53, 53));

    dark.setColor(QPalette::ToolTipBase, Qt::white);
    dark.setColor(QPalette::ToolTipText, Qt::white);

    dark.setColor(QPalette::Text, Qt::white);
    dark.setColor(QPalette::Button, QColor(53, 53, 53));
    dark.setColor(QPalette::ButtonText, Qt::white);

    dark.setColor(QPalette::BrightText, Qt::red);

    dark.setColor(QPalette::Highlight, QColor(42, 130, 218));
    dark.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(dark);
}

void MainWindow::applyLightTheme()
{
    qApp->setPalette(style()->standardPalette());
}