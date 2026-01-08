#pragma once
#include "user.h"
#include <vector>

class SeqList {
private:
    std::vector<User> users;
public:
    void add(const User& u);
    void clear();
    bool remove(int id);
    User* find(int id);
    User* findByName(const std::string &name);
    int size() const;
    // 顺序表实现，基于动态数组（std::vector）模拟线性表
    std::vector<User> list() const;
};
// seqlist.h