// ============================================================================
// CS32 P01 Part 1: Vector implementation
//
// This is the ONLY file you edit and submit.
//
// Note: this file is #included at the bottom of Vector.h rather than compiled
// on its own (templates must be visible wherever they are used). Do not add
// it to a g++ command line; `make` already does the right thing.
// ============================================================================

#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "Vector.h"

// ---- Non-member helper ----

// Capacity to grow to when a buffer of capacity cap is full.
template <typename T>
std::size_t next_capacity(std::size_t cap) {
    return std::max(cap + 1, (std::size_t)(cap * Vector<T>::growth_factor));
}

// ---- Constructors and the Big 3 ----

template <typename T>
Vector<T>::Vector() : begin_(nullptr), end_(nullptr), end_cap_(nullptr) {}

template <typename T>
Vector<T>::Vector(std::size_t n) : Vector() {
    if (n == 0) {
        return;
    }
    begin_ = new T[n]();
    end_ = begin_ + n;
    end_cap_ = end_;
}

template <typename T>
Vector<T>::Vector(std::size_t n, const T& value) : Vector(n) {
    std::fill(begin_, end_, value);
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> il) : Vector(il.size()) {
    std::copy(il.begin(), il.end(), begin_);
}

template <typename T>
Vector<T>::Vector(const Vector& other) : Vector(other.size()) {
    std::copy(other.begin(), other.end(), begin_);
}

// Copy-and-swap: copy builds the new buffer, and its destructor frees ours.
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this == &other) {
        return *this;
    }
    Vector copy(other);
    std::swap(begin_, copy.begin_);
    std::swap(end_, copy.end_);
    std::swap(end_cap_, copy.end_cap_);
    return *this;
}

template <typename T>
Vector<T>::~Vector() {
    delete[] begin_;
}

// ---- Private helper ----

template <typename T>
void Vector<T>::reallocate(std::size_t new_cap) {
    std::size_t n = size();
    T* new_buf = nullptr;
    if (new_cap > 0) {
        new_buf = new T[new_cap];
        std::copy(begin_, end_, new_buf);
    }
    delete[] begin_;
    begin_ = new_buf;
    end_ = new_buf + n;
    end_cap_ = new_buf + new_cap;
}

// ---- Element access ----

template <typename T>
T& Vector<T>::operator[](std::size_t i) {
    return begin_[i];
}

template <typename T>
const T& Vector<T>::operator[](std::size_t i) const {
    return begin_[i];
}

template <typename T>
T& Vector<T>::at(std::size_t i) {
    if (i >= size()) {
        throw std::out_of_range("Vector::at: index out of range");
    }
    return begin_[i];
}

template <typename T>
T& Vector<T>::front() {
    if (empty()) {
        throw std::out_of_range("Vector::front: vector is empty");
    }
    return *begin_;
}

template <typename T>
T& Vector<T>::back() {
    if (empty()) {
        throw std::out_of_range("Vector::back: vector is empty");
    }
    return *(end_ - 1);
}

template <typename T>
T* Vector<T>::data() {
    return begin_;
}

// ---- Iterators ----

template <typename T>
T* Vector<T>::begin() {
    return begin_;
}

template <typename T>
const T* Vector<T>::begin() const {
    return begin_;
}

template <typename T>
T* Vector<T>::end() {
    return end_;
}

template <typename T>
const T* Vector<T>::end() const {
    return end_;
}

// ---- Capacity ----

template <typename T>
std::size_t Vector<T>::size() const {
    return static_cast<std::size_t>(end_ - begin_);
}

template <typename T>
std::size_t Vector<T>::capacity() const {
    return static_cast<std::size_t>(end_cap_ - begin_);
}

template <typename T>
bool Vector<T>::empty() const {
    return begin_ == end_;
}

template <typename T>
void Vector<T>::reserve(std::size_t n) {
    if (n > capacity()) {
        reallocate(n);
    }
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    if (capacity() != size()) {
        reallocate(size());
    }
}

// ---- Modifiers ----

template <typename T>
void Vector<T>::push_back(const T& value) {
    insert(size(), value);
}

template <typename T>
void Vector<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("Vector::pop_back: vector is empty");
    }
    --end_;
}

template <typename T>
void Vector<T>::clear() {
    end_ = begin_;
}

template <typename T>
void Vector<T>::resize(std::size_t n) {
    reserve(n);
    for (T* p = end_; p < begin_ + n; ++p) {
        *p = T();
    }
    end_ = begin_ + n;
}

template <typename T>
void Vector<T>::insert(std::size_t index, const T& value) {
    if (index > size()) {
        throw std::out_of_range("Vector::insert: index out of range");
    }
    // value may live in our buffer, which reallocate frees, so copy it first.
    T copy = value;
    if (end_ == end_cap_) {
        reallocate(next_capacity<T>(capacity()));
    }
    // Shift right, starting from the back so nothing is overwritten.
    for (std::size_t i = size(); i > index; --i) {
        begin_[i] = begin_[i - 1];
    }
    begin_[index] = copy;
    ++end_;
}

template <typename T>
void Vector<T>::erase(std::size_t index) {
    if (index >= size()) {
        throw std::out_of_range("Vector::erase: index out of range");
    }
    // Shift left, starting from the front so nothing is overwritten.
    for (std::size_t i = index; i + 1 < size(); ++i) {
        begin_[i] = begin_[i + 1];
    }
    --end_;
}

// ---- Non-member operators ----

template <typename T>
bool operator==(const Vector<T>& a, const Vector<T>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (!(a[i] == b[i])) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool operator!=(const Vector<T>& a, const Vector<T>& b) {
    return !(a == b);
}

#endif