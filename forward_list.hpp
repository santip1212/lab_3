#pragma once
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
class forward_list {
private:
    struct Node {
        T data;
        Node* next; // только следующий
        Node(const T& val) : data(val), next(nullptr) {}
        Node(T&& val) : data(std::move(val)), next(nullptr) {}
    };

    Node* first;
    size_t count;

public:
    class iterator {
    private:
        Node* curr;
    public:
        iterator() : curr(nullptr) {}
        explicit iterator(Node* n) : curr(n) {}
        T& operator*() const { return curr->data; }
        T* operator->() const { return &curr->data; }
        T& get() { return curr->data; }
        iterator& operator++() { curr = curr->next; return *this; }
        bool operator==(const iterator& o) const { return curr == o.curr; }
        bool operator!=(const iterator& o) const { return curr != o.curr; }
    };

    explicit forward_list();
    forward_list(forward_list&& other) noexcept;
    forward_list& operator=(forward_list&& other) noexcept;
    forward_list(const forward_list&) = delete;
    forward_list& operator=(const forward_list&) = delete;
    ~forward_list();

    size_t size() const;
    bool empty() const;

    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);

    void insert(size_t index, const T& value);
    void insert(size_t index, T&& value);
    void erase(size_t index);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    iterator begin() { return iterator(first); }
    iterator end()   { return iterator(nullptr); }

private:
    void clear(); // удаляем все узлы
};

template <typename T>
forward_list<T>::forward_list() : first(nullptr), count(0) {}

template <typename T>
forward_list<T>::forward_list(forward_list&& other) noexcept
    : first(other.first), count(other.count) {
    other.first = nullptr;
    other.count = 0;
}

template <typename T>
forward_list<T>& forward_list<T>::operator=(forward_list&& other) noexcept {
    if (this != &other) {
        clear();
        first = other.first;
        count = other.count;
        other.first = nullptr;
        other.count = 0;
    }
    return *this;
}

template <typename T>
forward_list<T>::~forward_list() { clear(); }

template <typename T>
void forward_list<T>::clear() {
    while (first) { Node* n = first->next; delete first; first = n; }
    count = 0;
}

template <typename T>
size_t forward_list<T>::size() const { return count; }

template <typename T>
bool forward_list<T>::empty() const { return count == 0; }

template <typename T>
void forward_list<T>::push_front(const T& value) {
    Node* node = new Node(value);
    node->next = first;
    first = node;
    ++count;
}

template <typename T>
void forward_list<T>::push_front(T&& value) {
    Node* node = new Node(std::move(value));
    node->next = first;
    first = node;
    ++count;
}

template <typename T>
void forward_list<T>::push_back(const T& value) {
    Node* node = new Node(value);
    if (!first) first = node;
    else {
        Node* cur = first;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
    ++count;
}

template <typename T>
void forward_list<T>::push_back(T&& value) {
    Node* node = new Node(std::move(value));
    if (!first) first = node;
    else {
        Node* cur = first;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
    ++count;
}

template <typename T>
void forward_list<T>::insert(size_t index, const T& value) {
    if (index > count) throw std::out_of_range("forward_list::insert: index out of range");
    if (index == 0) { push_front(value); return; }

    Node* cur = first;
    for (size_t i = 0; i < index - 1; ++i) cur = cur->next;

    Node* node = new Node(value);
    node->next = cur->next;
    cur->next = node;
    ++count;
}

template <typename T>
void forward_list<T>::insert(size_t index, T&& value) {
    if (index > count) throw std::out_of_range("forward_list::insert: index out of range");
    if (index == 0) { push_front(std::move(value)); return; }

    Node* cur = first;
    for (size_t i = 0; i < index - 1; ++i) cur = cur->next;

    Node* node = new Node(std::move(value));
    node->next = cur->next;
    cur->next = node;
    ++count;
}

template <typename T>
void forward_list<T>::erase(size_t index) {
    if (index >= count) throw std::out_of_range("forward_list::erase: index out of range");

    if (index == 0) {
        Node* tmp = first;
        first = first->next;
        delete tmp;
        --count;
        return;
    }

    Node* cur = first;
    for (size_t i = 0; i < index - 1; ++i) cur = cur->next;

    Node* tmp = cur->next;
    cur->next = tmp->next;
    delete tmp;
    --count;
}

template <typename T>
T& forward_list<T>::operator[](size_t index) {
    Node* cur = first;
    for (size_t i = 0; i < index; ++i) cur = cur->next;
    return cur->data;
}

template <typename T>
const T& forward_list<T>::operator[](size_t index) const {
    Node* cur = first;
    for (size_t i = 0; i < index; ++i) cur = cur->next;
    return cur->data;
}