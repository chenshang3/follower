#include "model/linkedlist.h"

LinkedList::LinkedList() : head(nullptr) {}
LinkedList::~LinkedList() {
    Node* cur = head;
    while(cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
}

void LinkedList::add(const User& u) {
    Node* n = new Node(u);
    n->next = nullptr;
    if(!head) {
        head = n;
    } else {
        Node* cur = head;
        while(cur->next) cur = cur->next;
        cur->next = n;
    }
}

bool LinkedList::remove(int id) {
    Node* cur = head;
    Node* prev = nullptr;
    while(cur) {
        if(cur->data.id == id) {
            if(prev) prev->next = cur->next;
            else head = cur->next;
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

Node* LinkedList::find(int id) {
    Node* cur = head;
    while(cur) {
        if(cur->data.id == id) return cur;
        cur = cur->next;
    }
    return nullptr;
}

std::vector<User> LinkedList::list() const {
    std::vector<User> result;
    Node* cur = head;
    while(cur) {
        result.push_back(cur->data);
        cur = cur->next;
    }
    return result;
}
