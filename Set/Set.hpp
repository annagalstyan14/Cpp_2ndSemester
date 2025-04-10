#ifndef SET_HPP
#define SET_HPP

#include <iostream>
#include <unordered_map>
#include <initializer_list>
#include <functional>
#include <iterator>

template <typename T>
class Set {
private:
    std::unordered_map<T, bool> elements;

public:
    
    Set() = default;

    
    Set(std::initializer_list<T> list) {
        for (const auto& item : list) {
            insert(item);
        }
    }

    
    bool insert(const T& value) {
        return elements.emplace(value, true).second;
    }

    bool insert(T&& value) {
        return elements.emplace(std::move(value), true).second;
    }

    
    void insert_or_assign(const T& value) {
        elements[value] = true;
    }

    
    bool contains(const T& value) const {
        return elements.find(value) != elements.end();
    }

    
    bool erase(const T& value) {
        return elements.erase(value) > 0;
    }

    
    size_t size() const {
        return elements.size();
    }

    bool empty() const {
        return elements.empty();
    }

    // Clear the set
    void clear() {
        elements.clear();
    }

    // Print the set
    void print() const {
        for (const auto& item : elements) {
            std::cout << item.first << " ";
        }
        std::cout << std::endl;
    }

    // Comparison operators
    bool operator==(const Set& other) const {
        return elements == other.elements;
    }

    bool operator!=(const Set& other) const {
        return !(*this == other);
    }

    bool operator<(const Set& other) const {
        return elements < other.elements;
    }

    bool operator>(const Set& other) const {
        return other < *this;
    }

    bool operator<=(const Set& other) const {
        return !(other < *this);
    }

    bool operator>=(const Set& other) const {
        return !(*this < other);
    }

    
    typename std::unordered_map<T, bool>::const_iterator begin() const {
        return elements.begin();
    }

    typename std::unordered_map<T, bool>::const_iterator end() const {
        return elements.end();
    }
};

#endif 
