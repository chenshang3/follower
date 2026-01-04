#pragma once
#include "user.h"
#include <vector>

class SeqList {
private:
    std::vector<User> users;
public:
    void add(const User& u);
    bool remove(int id);
    User* find(int id);
    std::vector<User> list() const;
};
