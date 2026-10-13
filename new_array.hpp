#pragma once
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
class newdynamicarry {
private:
    std::unique_ptr<T[]> data;
    size_t sizearry;
    size_t capacityarry;

    void allocate();

public:
    using iterator = T*;
    using const_iterator = const T*;

    explicit newdynamicarry();
    newdynamicarry(newdynamicarry&& other) noexcept;
    newdynamicarry& operator=(newdynamicarry&& other) noexcept;
    newdynamicarry(const newdynamicarry&) = delete;
    newdynamicarry& operator=(const newdynamicarry&) = delete;
    ~newdynamicarry();

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
};

template <typename T>
newdynamicarry<T>::newdynamicarry() : data(nullptr), sizearry(0), capacityarry(0) {}

template <typename T>
newdynamicarry<T>::newdynamicarry(newdynamicarry&& other) noexcept
    : data(std::move(other.data)),
      sizearry(other.sizearry),
      capacityarry(other.capacityarry) {
    other.sizearry = 0;
    other.capacityarry = 0;
}

template <typename T>
newdynamicarry<T>& newdynamicarry<T>::operator=(newdynamicarry&& other) noexcept {
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
newdynamicarry<T>::~newdynamicarry() {}

template <typename T>
size_t newdynamicarry<T>::size() const { return sizearry; }

template <typename T>
bool newdynamicarry<T>::empty() const { return sizearry == 0; }

template <typename T>
void newdynamicarry<T>::allocate() {
    size_t newcapacityarry;
    if (capacityarry == 0)       newcapacityarry = 1;
    else if (capacityarry == 1)  newcapacityarry = 2;  
    else newcapacityarry = capacityarry * 3 / 2;
    std::unique_ptr<T[]> new_data = std::make_unique<T[]>(newcapacityarry);
    for (size_t i = 0; i < sizearry; ++i) new_data[i] = std::move(data[i]);
    data = std::move(new_data);
    capacityarry = newcapacityarry;
}

template <typename T>
void newdynamicarry<T>::push_back(const T& value) {
    if (sizearry == capacityarry) allocate();
    data[sizearry++] = value;
}

template <typename T>
void newdynamicarry<T>::push_back(T&& value) {
    if (sizearry == capacityarry) allocate();
    data[sizearry++] = std::move(value);
}

template <typename T>
void newdynamicarry<T>::push_front(const T& value) {
    if (sizearry == capacityarry) allocate();
    for (size_t i = sizearry; i > 0; --i) data[i] = std::move(data[i - 1]);
    data[0] = value;
    ++sizearry;
}

template <typename T>
void newdynamicarry<T>::push_front(T&& value) {
    if (sizearry == capacityarry) allocate();
    for (size_t i = sizearry; i > 0; --i) data[i] = std::move(data[i - 1]);
    data[0] = std::move(value);
    ++sizearry;
}

template <typename T>
void newdynamicarry<T>::erase(size_t index) {
    if (index >= sizearry) throw std::out_of_range("newdynamicarry::erase: index out of range");
    for (size_t i = index; i < sizearry - 1; ++i) data[i] = std::move(data[i + 1]);
    --sizearry;
}

template <typename T>
void newdynamicarry<T>::insert(size_t index, const T& value) {
    if (index > sizearry) throw std::out_of_range("newdynamicarry::insert: index out of range");
    if (sizearry == capacityarry) allocate();
    for (size_t i = sizearry; i > index; --i) data[i] = std::move(data[i - 1]);
    data[index] = value;
    ++sizearry;
}

template <typename T>
void newdynamicarry<T>::insert(size_t index, T&& value) {
    if (index > sizearry) throw std::out_of_range("newdynamicarry::insert: index out of range");
    if (sizearry == capacityarry) allocate();
    for (size_t i = sizearry; i > index; --i) data[i] = std::move(data[i - 1]);
    data[index] = std::move(value);
    ++sizearry;
}

template <typename T>
T& newdynamicarry<T>::operator[](size_t index) { return data[index]; }

template <typename T>
const T& newdynamicarry<T>::operator[](size_t index) const { return data[index]; }