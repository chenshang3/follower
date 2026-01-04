#include "model/seqlist.h"

void SeqList::add(const User& u) {
    users.push_back(u);
}

bool SeqList::remove(int id) {
    for(auto it = users.begin(); it != users.end(); ++it) {
        if(it->id == id) {
            users.erase(it);
            return true;
        }
    }
    return false;
}

User* SeqList::find(int id) {
    for(auto &u : users) {
        if(u.id == id) return &u;
    }
    return nullptr;
}

std::vector<User> SeqList::list() const {
    return users;
}
