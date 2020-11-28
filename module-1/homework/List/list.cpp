#include <stdexcept>
#include <vector>
#include <iostream>
#include "list.h"

// Your code goes here...
task::list::list() {
    size_ = 0;
    NIL->setNext(NIL);
    NIL->setPrev(NIL);
}

task::list::list(size_t count, const int& value): list() {
    for (size_t i = 0; i < count; ++i) {
        this->push_back(value);
    }

    size_ = count;
}

void task::list::push_back(const int& value) {
    Node* curr_last = NIL->getPrev();
    Node* begin = NIL;
    Node* new_node = new Node(value);

    new_node->setNext(begin);
    new_node->setPrev(curr_last);
    begin->setPrev(new_node);
    curr_last->setNext(new_node);
    ++size_;
}

task::list::~list() {
    Node* curr = NIL->getNext();
    while (curr->getNext() != NIL && curr) {
        Node* to_delete = curr;
        curr = curr->getNext();
        delete to_delete;
    }
    delete NIL;
}

void task::list::clear() {
    while (NIL->getNext() != NIL) {
        pop_back();
    }
    size_ = 0;
}

void task::list::pop_back() {
    Node* last = NIL->getPrev();
    if (last == NIL) {
        throw std::runtime_error("Pop back in empty list");
    }

    Node* prevLast = last->getPrev();
    prevLast->setNext(NIL);
    NIL->setPrev(prevLast);
    delete last;
    --size_;
}

int& task::list::front() {
    if (NIL->getNext() == NIL) {
        throw std::runtime_error("Accessing front element in empty list");
    }
    return NIL->getNext()->getValueRef();
}

const int& task::list::front() const {
    if (NIL->getNext() == NIL) {
        throw std::runtime_error("Accessing front element in empty list");
    }
    return NIL->getNext()->getValueRef();
}

int& task::list::back() {
    if (NIL->getNext() == NIL) {
        throw std::runtime_error("Accessing back element in empty list");
    }
    return NIL->getPrev()->getValueRef();
}

const int& task::list::back() const {
    if (NIL->getNext() == NIL) {
        throw std::runtime_error("Accessing back element in empty list");
    }
    return NIL->getPrev()->getValueRef();
}

bool task::list::empty() const {
    return size_ == 0;
}

size_t task::list::size() const {
    return size_;
}

void task::list::push_front(const int& value) {
    Node* new_node = new Node(value);
    Node* curr_front = NIL->getNext();
    NIL->setNext(new_node);
    new_node->setPrev(NIL);

    new_node->setNext(curr_front);
    curr_front->setPrev(new_node);
    size_++;
}
void task::list::pop_front() {
    if (NIL->getNext() == NIL) {
        //throw std::runtime_error("Pop front in empty list");
        return;
    }
    Node* curr_front = NIL->getNext();
    Node* second = curr_front->getNext();
    NIL->setNext(second);
    second->setPrev(NIL);
    delete curr_front;
    size_--;
}

void task::list::resize(size_t count) {
    while (size_ > count) {
        pop_back();
    }
    while (size_ < count) {
        push_back(0);
    }
    size_ = count;
}

task::list& task::list::operator=(const task::list& other) {
    if (this != &other) {
        clear();
        Node* other_head = other.NIL->getNext();
        for (int i = 0; i < other.size(); ++i) {
            this->push_back(other_head->getValue());
            other_head = other_head->getNext();
        }
        size_ = other.size();
    }
    return *this;
}

task::list::list(const task::list& other): list() {
    Node* other_head = other.NIL->getNext();
    for (int i = 0; i < other.size(); ++i) {
        this->push_back(other_head->getValue());
        other_head = other_head->getNext();
    }
    size_ = other.size();
}

void task::list::swap(task::list& other) {
    Node* self_nil_next = NIL->getNext();
    Node* self_nil_prev = NIL->getPrev();

    NIL->getNext()->setPrev(other.NIL);
    NIL->setNext(other.NIL->getNext());
    NIL->getPrev()->setNext(other.NIL);
    NIL->setPrev(other.NIL->getPrev());
    other.NIL->getNext()->setPrev(NIL);
    other.NIL->setNext(self_nil_next);
    other.NIL->getPrev()->setNext(NIL);
    other.NIL->setPrev(self_nil_prev);
    std::swap(size_, other.size_);
}

void task::list::remove(const int& value) {
    Node* curr = NIL->getNext();
    std::vector<Node*> to_delete;
    while (curr != NIL) {
        if (curr->getValue() == value) {
            to_delete.push_back(curr);
            Node* prev = curr->getPrev();
            prev->setNext(curr->getNext());
            curr->getNext()->setPrev(prev);
            curr = curr->getNext();
            size_--;
        } else {
            curr = curr->getNext();
        }
    }
    for (auto& el : to_delete) {
        delete el;
    }
}
void task::list::sort() {
    *this = sorted(*this);
}

task::list task::list::merge(const task::list& left, const task::list& right) {
    task::list result = task::list();

    const Node* curr_left = left.NIL->getNext();
    const Node* curr_right = right.NIL->getNext();

    while (curr_left != left.NIL && curr_right != right.NIL) {
        if (curr_left->getValue() < curr_right->getValue()) {
            result.push_back(curr_left->getValue());
            curr_left = curr_left->getNext();
        } else {
            result.push_back(curr_right->getValue());
            curr_right = curr_right->getNext();
        }
    }

    while (curr_left != left.NIL) {
        result.push_back(curr_left->getValue());
        curr_left = curr_left->getNext();
    }

    while (curr_right != right.NIL) {
        result.push_back(curr_right->getValue());
        curr_right = curr_right->getNext();
    }

    return result;
}

std::pair<task::list*, task::list*> task::list::bisect(const task::list& source) {
    int size = source.size();
    task::list *left = new task::list();
    task::list *right = new task::list();

    Node* curr = source.NIL->getNext();

    for (int i = 0; i < size / 2; ++i, curr = curr->getNext()) {
        left->push_back(curr->getValue());
    }

    for (int i = size / 2; i < size; ++i, curr = curr->getNext()) {
        right->push_back(curr->getValue());
    }

    return {left, right};
}

task::list task::list::sorted(const task::list& source) {
    if (source.size() > 1) {
        std::pair<task::list*, task::list*> res = task::list::bisect(source);
        return task::list::merge(task::list::sorted(*res.first), task::list::sorted(*res.second));
    }
    return source;
}
void task::list::unique() {
    Node* curr = NIL->getNext()->getNext();

    while (curr->getPrev() != NIL) {
        Node* prev = curr->getPrev();
        if (prev != NIL && prev->getValue() == curr->getValue()) {
            Node* next = curr->getNext();
            prev->setNext(next);
            next->setPrev(prev);
            size_--;
            Node* to_delete = curr;
            curr = curr->getNext();
            delete to_delete;
        } else {
            curr = curr->getNext();
        }
    }

}




