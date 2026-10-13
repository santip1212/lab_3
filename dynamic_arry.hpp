#pragma once
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
class dynamicarry {
private:
    std::unique_ptr<T[]> data;
    size_t sizearry;
    size_t capacityarry;

public:
    using iterator = T*;
    using const_iterator = const T*;

    explicit dynamicarry();
    dynamicarry(dynamicarry&& other) noexcept;
    dynamicarry& operator=(dynamicarry&& other) noexcept;
    dynamicarry(const dynamicarry&) = delete;
    dynamicarry& operator=(const dynamicarry&) = delete;
    ~dynamicarry();

    size_t size() const;
    bool empty() const;

    void push_back(const T& value);
    void push_back(T&& value);
    void push_front(const T& value);
    void push_front(T&& value);

    void erase(size_t index);
    void insert(size_t index, const T& value);
    void insert(size_t index, T&& value);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    iterator begin() { return data.get(); }
    iterator end()   { return data.get() + sizearry; }
    const_iterator begin() const { return data.get(); }
    const_iterator end()   const { return data.get() + sizearry; }

private:
    void reallocate();
};

template <typename T>
dynamicarry<T>::dynamicarry() : data(nullptr), sizearry(0), capacityarry(0) {}

template <typename T>
dynamicarry<T>::dynamicarry(dynamicarry&& other) noexcept
    : data(std::move(other.data)),
      sizearry(other.sizearry),
      capacityarry(other.capacityarry) {
    other.sizearry = 0;
    other.capacityarry = 0;
}

template <typename T>
dynamicarry<T>& dynamicarry<T>::operator=(dynamicarry&& other) noexcept {
    if (this != &other) {
        data = std::move(other.data);
        sizearry = other.sizearry;
        capacityarry = other.capacityarry;
        other.sizearry = 0;
        other.capacityarry = 0;
    }
    return *this;
}

template <typename T>
dynamicarry<T>::~dynamicarry() {}

template <typename T>
size_t dynamicarry<T>::size() const { return sizearry; }

template <typename T>
bool dynamicarry<T>::empty() const { return sizearry == 0; }

template <typename T>
void dynamicarry<T>::reallocate() {
    size_t new_capacity = (capacityarry == 0) ? 1 : capacityarry * 2;
    std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);
    for (size_t i = 0; i < sizearry; ++i) {
        new_data[i] = std::move(data[i]);
    }
    data = std::move(new_data);
    capacityarry = new_capacity;
}

template <typename T>
void dynamicarry<T>::push_back(const T& value) {
    if (sizearry == capacityarry) reallocate();
    data[sizearry++] = value;
}

template <typename T>
void dynamicarry<T>::push_back(T&& value) {
    if (sizearry == capacityarry) reallocate();
    data[sizearry++] = std::move(value);
}

template <typename T>
void dynamicarry<T>::push_front(const T& value) {
    if (sizearry == capacityarry) reallocate();
    for (size_t i = sizearry; i > 0; --i) data[i] = std::move(data[i - 1]);
    data[0] = value;
    ++sizearry;
}

template <typename T>
void dynamicarry<T>::push_front(T&& value) {
    if (sizearry == capacityarry) reallocate();
    for (size_t i = sizearry; i > 0; --i) data[i] = std::move(data[i - 1]);
    data[0] = std::move(value);
    ++sizearry;
}

template <typename T>
void dynamicarry<T>::erase(size_t index) {
    if (index >= sizearry) throw std::out_of_range("dynamicarry::erase: index out of range");
    for (size_t i = index; i < sizearry - 1; ++i) data[i] = std::move(data[i + 1]);
    --sizearry;
}

template <typename T>
void dynamicarry<T>::insert(size_t index, const T& value) {
    if (index > sizearry) throw std::out_of_range("dynamicarry::insert: index out of range");
    if (sizearry == capacityarry) reallocate();
    for (size_t i = sizearry; i > index; --i) data[i] = std::move(data[i - 1]);
    data[index] = value;
    ++sizearry;
}

template <typename T>
void dynamicarry<T>::insert(size_t index, T&& value) {
    if (index > sizearry) throw std::out_of_range("dynamicarry::insert: index out of range");
    if (sizearry == capacityarry) reallocate();
    for (size_t i = sizearry; i > index; --i) data[i] = std::move(data[i - 1]);
    data[index] = std::move(value);
    ++sizearry;
}

template <typename T>
T& dynamicarry<T>::operator[](size_t index) { return data[index]; }

template <typename T>
const T& dynamicarry<T>::operator[](size_t index) const { return data[index]; }