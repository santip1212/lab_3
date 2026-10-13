#pragma once
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
class list {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
        Node(T&& val)      : data(std::move(val)), prev(nullptr), next(nullptr) {}
    };

    Node* first;
    Node* last;
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
        iterator operator++(int) { iterator tmp = *this; curr = curr->next; return tmp; }
        bool operator==(const iterator& o) const { return curr == o.curr; }
        bool operator!=(const iterator& o) const { return curr != o.curr; }
    };

    class const_iterator {
    private:
        const Node* curr;
    public:
        const_iterator() : curr(nullptr) {}
        explicit const_iterator(const Node* n) : curr(n) {}
        const T& operator*() const { return curr->data; }
        const T* operator->() const { return &curr->data; }
        const T& get() const { return curr->data; }
        const_iterator& operator++() { curr = curr->next; return *this; }
        bool operator==(const const_iterator& o) const { return curr == o.curr; }
        bool operator!=(const const_iterator& o) const { return curr != o.curr; }
    };

    explicit list();
    list(list&& other) noexcept;
    list& operator=(list&& other) noexcept;
    list(const list&) = delete;
    list& operator=(const list&) = delete;
    ~list();

    size_t size() const;
    bool empty() const;

    void push_back(const T& value);
    void push_back(T&& value);
    void push_front(const T& value);
    void push_front(T&& value);

    void insert(size_t index, const T& value);
    void insert(size_t index, T&& value);
    void erase(size_t index);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    iterator begin() { return iterator(first); }
    iterator end()   { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(first); }
    const_iterator end()   const { return const_iterator(nullptr); }
};

template <typename T>
list<T>::list() : first(nullptr), last(nullptr), count(0) {}

template <typename T>
list<T>::list(list&& other) noexcept
    : first(other.first), last(other.last), count(other.count) {
    other.first = other.last = nullptr;
    other.count = 0;
}

template <typename T>
list<T>& list<T>::operator=(list&& other) noexcept {
    if (this != &other) {
        Node* cur = first;
        while (cur) { Node* n = cur->next; delete cur; cur = n; }
        first = other.first;
        last = other.last;
        count = other.count;
        other.first = other.last = nullptr;
        other.count = 0;
    }
    return *this;
}

template <typename T>
list<T>::~list() {
    Node* cur = first;
    while (cur) { Node* n = cur->next; delete cur; cur = n; }
}

template <typename T>
size_t list<T>::size() const { return count; }

template <typename T>
bool list<T>::empty() const { return count == 0; }

template <typename T>
void list<T>::push_back(const T& value) {
    Node* node = new Node(value);
    if (!first) { first = last = node; }
    else { node->prev = last; last->next = node; last = node; }
    ++count;
}

template <typename T>
void list<T>::push_back(T&& value) {
    Node* node = new Node(std::move(value));
    if (!first) { first = last = node; }
    else { node->prev = last; last->next = node; last = node; }
    ++count;
}

template <typename T>
void list<T>::push_front(const T& value) {
    Node* node = new Node(value);
    if (!first) { first = last = node; }
    else { node->next = first; first->prev = node; first = node; }
    ++count;
}

template <typename T>
void list<T>::push_front(T&& value) {
    Node* node = new Node(std::move(value));
    if (!first) { first = last = node; }
    else { node->next = first; first->prev = node; first = node; }
    ++count;
}

template <typename T>
void list<T>::insert(size_t index, const T& value) {
    if (index > count) throw std::out_of_range("list::insert: index out of range");
    if (index == 0) { push_front(value); return; }
    if (index == count) { push_back(value); return; }

    Node* cur = first;
    for (size_t i = 0; i < index; ++i) cur = cur->next;

    Node* node = new Node(value);
    node->prev = cur->prev;
    node->next = cur;
    cur->prev->next = node;
    cur->prev = node;
    ++count;
}

template <typename T>
void list<T>::insert(size_t index, T&& value) {
    if (index > count) throw std::out_of_range("list::insert: index out of range");
    if (index == 0) { push_front(std::move(value)); return; }
    if (index == count) { push_back(std::move(value)); return; }

    Node* cur = first;
    for (size_t i = 0; i < index; ++i) cur = cur->next;

    Node* node = new Node(std::move(value));
    node->prev = cur->prev;
    node->next = cur;
    cur->prev->next = node;
    cur->prev = node;
    ++count;
}

template <typename T>
void list<T>::erase(size_t index) {
    if (index >= count) throw std::out_of_range("list::erase: index out of range");

    Node* cur = first;
    for (size_t i = 0; i < index; ++i) cur = cur->next;

    if (cur->prev) cur->prev->next = cur->next;
    else first = cur->next;

    if (cur->next) cur->next->prev = cur->prev;
    else last = cur->prev;

    delete cur;
    --count;
}

template <typename T>
T& list<T>::operator[](size_t index) {
    Node* cur = first;
    for (size_t i = 0; i < index; ++i) cur = cur->next;
    return cur->data;
}

template <typename T>
const T& list<T>::operator[](size_t index) const {
    Node* cur = first;
    for (size_t i = 0; i < index; ++i) cur = cur->next;
    return cur->data;
}