#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <stdexcept>
#include <functional>
#include <utility>

template <typename T>
class Optional {
private:
    bool has_value;
    T value;

public:
    Optional() : has_value(false) {}
    Optional(const T& val) : has_value(true), value(val) {}
    Optional(T&& val) : has_value(true), value(std::move(val)) {}

    
    Optional(Optional&& other) noexcept : has_value(other.has_value), value(std::move(other.value)) {
        other.has_value = false;
    }

    
    Optional& operator=(Optional&& other) noexcept {
        if (this != &other) {
            has_value = other.has_value;
            value = std::move(other.value);
            other.has_value = false;
        }
        return *this;
    }

    
    Optional(const Optional& other) : has_value(other.has_value), value(other.value) {}

    
    Optional& operator=(const Optional& other) {
        if (this != &other) {
            has_value = other.has_value;
            value = other.value;
        }
        return *this;
    }

    bool is_present() const { return has_value; }

    T& operator*() {
        if (!has_value) {
            throw std::runtime_error("Accessing empty Optional");
        }
        return value;
    }

    const T& operator*() const {
        if (!has_value) {
            throw std::runtime_error("Accessing empty Optional");
        }
        return value;
    }

    T* operator->() {
        if (!has_value) {
            throw std::runtime_error("Accessing empty Optional");
        }
        return &value;
    }

    const T* operator->() const {
        if (!has_value) {
            throw std::runtime_error("Accessing empty Optional");
        }
        return &value;
    }

    void reset() { has_value = false; }

    T value_or(const T& default_value) const {
        return has_value ? value : default_value;
    }

    template <typename F>
    auto map(F&& func) const -> Optional<decltype(func(value))> {
        if (has_value) {
            return Optional<decltype(func(value))>(func(value));
        }
        return Optional<decltype(func(value))>();
    }

    template <typename F>
    auto flat_map(F&& func) const -> decltype(func(value)) {
        if (has_value) {
            return func(value);
        }
        return decltype(func(value))();
    }

    bool operator==(const Optional& other) const {
        if (has_value != other.has_value) return false;
        return value == other.value;
    }

    bool operator!=(const Optional& other) const {
        return !(*this == other);
    }

    bool operator<(const Optional& other) const {
        if (!has_value) return other.has_value;
        if (!other.has_value) return false;
        return value < other.value;
    }

    bool operator>(const Optional& other) const {
        return other < *this;
    }

    bool operator<=(const Optional& other) const {
        return !(other < *this);
    }

    bool operator>=(const Optional& other) const {
        return !(*this < other);
    }
};

#endif 
