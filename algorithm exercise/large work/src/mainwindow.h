#pragma once
#include <QMainWindow>
#include "model/seqlist.h"
#include "model/linkedlist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onFindClicked();
    void onPrevPageClicked();
    void onNextPageClicked();
private:
    Ui::MainWindow *ui;
    SeqList seqList;
    LinkedList linkedList;
    int currentPage = 0;
    int pageSize = 10;
    void refreshTable();
};
