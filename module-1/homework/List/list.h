#pragma once
#include <cstddef>


namespace task {
class list {

public:

    list();
    list(size_t count, const int& value = int());
    list(const list& other);

    ~list();
    list& operator=(const list& other);


    int& front();
    const int& front() const;

    int& back();
    const int& back() const;


    bool empty() const;
    size_t size() const;
    void clear();


    void push_back(const int& value);
    void pop_back();
    void push_front(const int& value);
    void pop_front();
    void resize(size_t count);
    void swap(list& other);


    void remove(const int& value);
    void unique();
    void sort();

    // Your code goes here?..

private:
    class Node {
    public:
        explicit Node(int value, Node* next = nullptr, Node* prev = nullptr) {
            this->value = value;
            this->next = next;
            this->prev = prev;
        }

        int getValue() const { return value; }

        int& getValueRef() { return value; }

        const int& getValueRef() const { return value;}

        const Node* getPrev() const { return prev; }

        Node* getPrev() { return prev; }

        const Node* getNext() const { return next; }

        Node* getNext() { return next; }

        void setNext(Node* newNext) { this->next = newNext; }

        void setPrev(Node* newPrev) { this->prev = newPrev; }

        ~Node() {
            this->next = nullptr;
            this->prev = nullptr;
        }

    private:
        int value;
        Node* prev;
        Node* next;
    };

    Node* NIL = new Node(0);

    size_t size_ = 0;

    static list merge(const list& left, const list& right);//begin2 == end1

    static std::pair<list*, list*> bisect(const list& source);

    static list sorted(const list& source);
};

}  // namespace task
