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
    std::vector<User> list() const;
};
