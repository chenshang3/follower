#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model/seqlist.h"
#include "model/linkedlist.h"
#include <QMessageBox>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->btnRemove, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(ui->btnFind, &QPushButton::clicked, this, &MainWindow::onFindClicked);
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
