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
    Node* head;
public:
    LinkedList();
    ~LinkedList();
    void add(const User& u);
    bool remove(int id);
    Node* find(int id);
    Node* headNode() const; // return head pointer
    void clear();
    int size() const;
    std::vector<User> list() const;
};
