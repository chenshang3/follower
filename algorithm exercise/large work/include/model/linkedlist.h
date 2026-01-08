#pragma once
#include "user.h"
#include <vector>

struct Node {
    User data;
    Node* next;
    Node(const User& u) : data(u), next(nullptr) {}
};

class LinkedList {
private:
    // 单链表（不带头结点），head 指向首元结点
    Node* head;
public:
    LinkedList();
    ~LinkedList();
    void add(const User& u);
    bool remove(int id);
    // 返回包含目标用户的结点指针，用于后续操作
    Node* find(int id);
    // 返回链表头指针，仅用于遍历和分页显示
    Node* headNode() const; 
    void clear();
    int size() const;
    // 将链表数据拷贝为顺序表，便于界面显示（不改变底层结构）
    std::vector<User> list() const;
};
// linkedlist.h