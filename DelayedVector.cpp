#include <iostream>
#include <vector>
#include <list>

template <typename Iterator>
class LazySum {
private:
    Iterator a_begin, a_end;
    Iterator b_begin, b_end;

public:
    LazySum(Iterator a_start, Iterator a_stop, Iterator b_start, Iterator b_stop)
        : a_begin(a_start), a_end(a_stop), b_begin(b_start), b_end(b_stop) {
    }

    auto operator[](size_t index) const {
        auto a_it = a_begin;
        auto b_it = b_begin;

        std::advance(a_it, index);
        std::advance(b_it, index);

        if (a_it == a_end || b_it == b_end) {
            throw std::out_of_range("Index out of bounds");
        }
        std::cout << "Computing sum for index " << index << std::endl;

        return *a_it + *b_it;
    }
};

template <typename Container>
auto makeLazySum(Container& a, Container& b) {
    return LazySum<typename Container::iterator>(a.begin(), a.end(), b.begin(), b.end());
}

int main() {
    std::vector<int> vec1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int> vec2 = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    auto sum_vec = makeLazySum(vec1, vec2);

    std::cout << "Vector Sum at index 3: " << sum_vec[3] << std::endl;

    std::list<int> list1 = { 1, 2, 3, 4, 5};
    std::list<int> list2 = { 5, 5, 4, 4, 3 };
    auto sum_list = makeLazySum(list1, list2);

    std::cout << "List Sum at index 4: " << sum_list[4] << std::endl;

    return 0;
}