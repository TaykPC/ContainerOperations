#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <regex>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <forward_list>
#include <memory>
#include <algorithm>
#include <random>
#include <numeric>
#include <iterator>
#include <functional>
#include "Timer.h"
#include "utility.h"


using std::cout, std::endl;
using namespace std::literals;


template<typename InputIt>
void fill_randomly(InputIt first, InputIt last, int max_val = 10) { // Iteratoren für die Bereichsangabe
    while (first != last)
        *first++ = random_int(0, max_val);      // aus utility.{h,cpp}
    //  *first++ = std::rand() % (max_val + 1);
}


namespace Demo {
    void run1()
    {
        const int c_size = 20'000'000;
        cout << "Anlegen eines std::vector<int> mit " << c_size << " Zufallszahlen ... " << std::flush;
        std::vector<int> vec(c_size);
        fill_randomly(std::begin(vec), std::end(vec), 100);
        cout << "fertig\n" << endl;

        const auto c_op_name_elem = std::make_pair("Elem"s, "Elems"s);
        Timer timer{ 80 };  // 80 Zeichen Präfix-Länge zur Formatierung der Ausgabe

        {
            timer.start("vector, copy construction", vec.size(), c_op_name_elem);
            std::vector<int> v{ vec };
            timer.stop_and_print_runtime();

            {
                timer.start("vector, move construction", vec.size(), c_op_name_elem);
                std::vector<int> v2{ std::move(v) };
                timer.stop_and_print_runtime();
                timer.start("vector, destruction", vec.size(), c_op_name_elem);
            }
            timer.stop_and_print_runtime();  // v2 ist jetzt out-of-scope, also destruiert
        }

        {
            const size_t test_size = vec.size();
            timer.start("vector, push_back", test_size);
            std::vector<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.push_back(vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("vector, push_back mit at()", test_size);
            std::vector<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.push_back(vec.at(i));
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("vector, reserved, push_back", test_size);
            std::vector<int> v;
            v.reserve(test_size);
            for (size_t i = 0; i < test_size; ++i)
                v.push_back(vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = std::min(vec.size(), size_t(100'000));
            timer.start("vector, insert at front", test_size);
            std::vector<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.insert(v.begin(), vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = std::min(vec.size(), size_t(100'000));
            timer.start("vector, reserved, insert at front", test_size);
            std::vector<int> v;
            v.reserve(test_size);
            for (size_t i = 0; i < test_size; ++i)
                v.insert(v.begin(), vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            std::vector<int> v{ vec };
            {
                timer.start("vector, Leeren mit erase-Schleife von hinten", test_size);
                while (v.size() > 0)
                    v.erase(v.begin() + v.size() - 1);
            }
        }

        {
            const size_t test_size = std::min(vec.size(), size_t(100'000));
            std::vector<int> v{ vec.begin(), vec.begin() + test_size };
            {
                timer.start("vector, Leeren mit erase-Schleife von vorne", test_size);
                while (v.size() > 0)
                    v.erase(v.begin());
            }
        }

        {
            const size_t test_size = vec.size();
            std::vector<int> v{ vec };
            {
                timer.start("vector, Leeren mit pop_back()-Schleife", test_size);
                while (v.size() > 0)
                    v.pop_back();
            }
        }


        {
            const size_t test_size = vec.size();
            timer.start("list, iterator construction aus vector", test_size, c_op_name_elem);
            std::list<int> v{ vec.begin(), vec.end() };
            timer.stop_and_print_runtime();

            {
                const size_t test_size = vec.size();
                timer.start("list, copy construction", test_size, c_op_name_elem);
                std::list<int> v2{ v };
                timer.stop_and_print_runtime();
            }

            {
                const size_t test_size = vec.size();
                timer.start("list, move construction", test_size, c_op_name_elem);
                std::list<int> v2{ std::move(v) };
                timer.stop_and_print_runtime();
                timer.start("list, destruction", test_size, c_op_name_elem);
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("list, push_back", test_size);
            std::list<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.push_back(vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("list, push_front", test_size);
            std::list<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.push_front(vec[i]);
            timer.stop_and_print_runtime();
        }


        {
            const size_t test_size = vec.size();
            timer.start("forward_list, iterator construction aus vector", test_size, c_op_name_elem);
            std::forward_list<int> v{ vec.begin(), vec.end() };
            timer.stop_and_print_runtime();

            {
                const size_t test_size = vec.size();
                timer.start("forward_list, copy construction", test_size, c_op_name_elem);
                std::forward_list<int> v2{ v };
                timer.stop_and_print_runtime();
            }

            {
                const size_t test_size = vec.size();
                timer.start("forward_list, move construction", test_size, c_op_name_elem);
                std::forward_list<int> v2{ std::move(v) };
                timer.stop_and_print_runtime();
                timer.start("forward_list, destruction", test_size, c_op_name_elem);
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("forward_list, push_front", test_size);
            std::forward_list<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.push_front(vec[i]);
            timer.stop_and_print_runtime();
        }


        {
            const size_t test_size = vec.size();
            timer.start("deque, iterator construction aus vector", test_size, c_op_name_elem);
            std::deque<int> v{ vec.begin(), vec.end() };
            timer.stop_and_print_runtime();

            {
                const size_t test_size = vec.size();
                timer.start("deque, copy construction", test_size, c_op_name_elem);
                std::deque<int> v2{ v };
                timer.stop_and_print_runtime();
            }

            {
                const size_t test_size = vec.size();
                timer.start("deque, move construction", test_size, c_op_name_elem);
                std::deque<int> v2{ std::move(v) };
                timer.stop_and_print_runtime();
                timer.start("deque, destruction", test_size, c_op_name_elem);
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("deque, push_back", test_size);
            std::deque<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.push_back(vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("deque, push_front", test_size);
            std::deque<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.push_front(vec[i]);
            timer.stop_and_print_runtime();
        }
    }


    void run2() {
        const int c_size = 10'000'000;
        cout << "Anlegen eines std::vector<int> mit " << c_size << " aufsteigenden Zahlen ... " << std::flush;
        std::vector<int> vec(c_size);
        std::iota(std::begin(vec), std::end(vec), 1);
        cout << "fertig\n" << endl;

        const auto c_op_name_elem = std::make_pair("Elem"s, "Elems"s);
        Timer timer{ 80 };  // 80 Zeichen Präfix-Länge zur Formatierung der Ausgabe

        {
            const size_t test_size = vec.size();
            timer.start("set, iterator construction aus geordnetem vector", test_size, c_op_name_elem);
            std::set<int> v{ vec.begin(), vec.end() };
            timer.stop_and_print_runtime();

            {
                const size_t test_size = vec.size();
                timer.start("set, copy construction", test_size, c_op_name_elem);
                std::set<int> v2{ v };
                timer.stop_and_print_runtime();
            }

            {
                const size_t test_size = vec.size();
                timer.start("set, move construction", test_size, c_op_name_elem);
                std::set<int> v2{ std::move(v) };
                timer.stop_and_print_runtime();
                timer.start("set, destruction", test_size, c_op_name_elem);
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = std::min((size_t)5000000ull, vec.size());
            std::vector<int> vec_shuffled{ vec.begin(), vec.begin() + test_size };
            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(vec_shuffled.begin(), vec_shuffled.end(), gen);

            timer.start("set, iterator construction aus ungeordnetem vector (eindeutig)", test_size, c_op_name_elem);
            std::set<int> v{ vec_shuffled.begin(), vec_shuffled.end() };
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("set, insert aus geordnetem vector (eindeutig)", test_size);
            std::set<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.insert(vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("set, std::inserter(..., end())   aus geordnetem vector (eindeutig)", test_size);
            std::set<int> v;
            std::copy(vec.begin(), vec.end(), std::inserter(v, v.end()));
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("set, std::inserter(..., begin()) aus geordnetem vector (eindeutig)", test_size);
            std::set<int> v;
            std::copy(vec.begin(), vec.end(), std::inserter(v, v.begin()));
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size() / 2;
            timer.start("set, insert aus geordnetem vector (doppelt)", 2 * test_size);
            std::set<int> v;
            for (size_t i = 0; i < test_size; ++i) {
                v.insert(vec[i]);
                v.insert(vec[i]);
            }
            timer.stop_and_print_runtime();
        }


        {
            const size_t test_size = vec.size();
            timer.start("multiset, iterator construction aus geordnetem vector", test_size, c_op_name_elem);
            std::multiset<int> v{ vec.begin(), vec.end() };
            timer.stop_and_print_runtime();

            {
                const size_t test_size = vec.size();
                timer.start("multiset, copy construction", test_size, c_op_name_elem);
                std::multiset<int> v2{ v };
                timer.stop_and_print_runtime();
            }

            {
                const size_t test_size = vec.size();
                timer.start("multiset, move construction", test_size, c_op_name_elem);
                std::multiset<int> v2{ std::move(v) };
                timer.stop_and_print_runtime();
                timer.start("multiset, destruction", test_size, c_op_name_elem);
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("multiset, insert aus geordnetem vector (eindeutig)", test_size);
            std::multiset<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.insert(vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size() / 2;
            timer.start("multiset, insert aus geordnetem vector (doppelt)", 2 * test_size);
            std::multiset<int> v;
            for (size_t i = 0; i < test_size; ++i) {
                v.insert(vec[i]);
                v.insert(vec[i]);
            }
        }


        // unordered_set
        {
            const size_t test_size = vec.size();
            timer.start("unordered_set, iterator construction aus geordnetem vector", test_size, c_op_name_elem);
            std::unordered_set<int> v{ vec.begin(), vec.end() };
            timer.stop_and_print_runtime();

            {
                const size_t test_size = vec.size();
                timer.start("unordered_set, copy construction", test_size, c_op_name_elem);
                std::unordered_set<int> v2{ v };
                timer.stop_and_print_runtime();
            }

            {
                const size_t test_size = vec.size();
                timer.start("unordered_set, move construction", test_size, c_op_name_elem);
                std::unordered_set<int> v2{ std::move(v) };
                timer.stop_and_print_runtime();
                timer.start("unordered_set, destruction", test_size, c_op_name_elem);
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = std::min((size_t)5000000ull, vec.size());
            std::vector<int> vec_shuffled{ vec.begin(), vec.begin() + test_size };
            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(vec_shuffled.begin(), vec_shuffled.end(), gen);

            timer.start("unordered_set, iterator construction aus ungeordnetem vector (eindeutig)", test_size, c_op_name_elem);
            std::unordered_set<int> v{ vec_shuffled.begin(), vec_shuffled.end() };
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("unordered_set, insert aus geordnetem vector (eindeutig)", test_size);
            std::unordered_set<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.insert(vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("unordered_set, std::inserter(..., end())   aus geordnetem vector (eindeutig)", test_size);
            std::unordered_set<int> v;
            std::copy(vec.begin(), vec.end(), std::inserter(v, v.end()));
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("unordered_set, std::inserter(..., begin()) aus geordnetem vector (eindeutig)", test_size);
            std::unordered_set<int> v;
            std::copy(vec.begin(), vec.end(), std::inserter(v, v.begin()));
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size() / 2;
            timer.start("unordered_set, insert aus geordnetem vector (doppelt)", 2 * test_size);
            std::unordered_set<int> v;
            for (size_t i = 0; i < test_size; ++i) {
                v.insert(vec[i]);
                v.insert(vec[i]);
            }
            timer.stop_and_print_runtime();
        }


        {
            const size_t test_size = vec.size();
            timer.start("unordered_multiset, iterator construction aus geordnetem vector", test_size, c_op_name_elem);
            std::unordered_multiset<int> v{ vec.begin(), vec.end() };
            timer.stop_and_print_runtime();

            {
                const size_t test_size = vec.size();
                timer.start("unordered_multiset, copy construction", test_size, c_op_name_elem);
                std::unordered_multiset<int> v2{ v };
                timer.stop_and_print_runtime();
            }

            {
                const size_t test_size = vec.size();
                timer.start("unordered_multiset, move construction", test_size, c_op_name_elem);
                std::unordered_multiset<int> v2{ std::move(v) };
                timer.stop_and_print_runtime();
                timer.start("unordered_multiset, destruction", test_size, c_op_name_elem);
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("unordered_multiset, insert aus geordnetem vector (eindeutig)", test_size);
            std::unordered_multiset<int> v;
            for (size_t i = 0; i < test_size; ++i)
                v.insert(vec[i]);
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size() / 2;
            timer.start("unordered_multiset, insert aus geordnetem vector (doppelt)", 2 * test_size);
            std::unordered_multiset<int> v;
            for (size_t i = 0; i < test_size; ++i) {
                v.insert(vec[i]);
                v.insert(vec[i]);
            }
        }


        {
            const size_t test_size = vec.size();
            timer.start("map, insert aus vector (eindeutig)", test_size);
            std::map<int, int> m;
            for (size_t i = 0; i < test_size; ++i)
                m.insert({ vec[i], vec[i] });
            timer.stop_and_print_runtime();

            {
                const size_t test_size = vec.size();
                timer.start("map, copy construction", test_size, c_op_name_elem);
                std::map<int, int> m2{ m };
                timer.stop_and_print_runtime();
            }

            {
                const size_t test_size = vec.size();
                timer.start("map, move construction", test_size, c_op_name_elem);
                std::map<int, int> m2{ std::move(m) };
                timer.stop_and_print_runtime();
                timer.start("map, destruction", test_size, c_op_name_elem);
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size();
            timer.start("map, insert mit [] aus vector (eindeutig)", test_size);
            std::map<int, int> m;
            for (size_t i = 0; i < test_size; ++i)
                m[vec[i]] = vec[i];
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size() / 2;
            timer.start("map, insert aus vector (doppelt)", 2 * test_size);
            std::map<int, int> m;
            for (size_t i = 0; i < test_size; ++i) {
                m.insert({ vec[i], vec[i] });
                m.insert({ vec[i], vec[i] });
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size() / 2;
            timer.start("map, insert mit [] aus vector (doppelt)", 2 * test_size);
            std::map<int, int> m;
            for (size_t i = 0; i < test_size; ++i) {
                m[vec[i]] = vec[i];
                m[vec[i]] = vec[i];
            }
            timer.stop_and_print_runtime();
        }


        {
            const size_t test_size = vec.size();
            timer.start("multimap, insert aus vector (eindeutig)", test_size);
            std::multimap<int, int> m;
            for (size_t i = 0; i < test_size; ++i)
                m.insert({ vec[i], vec[i] });
            timer.stop_and_print_runtime();

            {
                const size_t test_size = vec.size();
                timer.start("multimap, copy construction", test_size, c_op_name_elem);
                std::multimap<int, int> m2{ m };
                timer.stop_and_print_runtime();
            }

            {
                const size_t test_size = vec.size();
                timer.start("multimap, move construction", test_size, c_op_name_elem);
                std::multimap<int, int> m2{ std::move(m) };
                timer.stop_and_print_runtime();
                timer.start("multimap, destruction", test_size, c_op_name_elem);
            }
            timer.stop_and_print_runtime();
        }

        {
            const size_t test_size = vec.size() / 2;
            timer.start("multimap, insert aus vector (doppelt)", 2 * test_size);
            std::multimap<int, int> m;
            for (size_t i = 0; i < test_size; ++i) {
                m.insert({ vec[i], vec[i] });
                m.insert({ vec[i], vec[i] });
            }
            timer.stop_and_print_runtime();
        }
    }


    void run3() {
        const int c_size = 100'000'000;
        std::vector<int> vec;

        cout << "Wachstum von std::vector" << endl;

        size_t old_capacity = 0, new_capacity = 0;

        for (int i = 0; i < c_size; ++i) {
            vec.push_back(i);
            new_capacity = vec.capacity();
            if (new_capacity != old_capacity) {
                cout << std::setw(10) << new_capacity << " (Faktor: " << std::setprecision(3) << double(new_capacity) / old_capacity << ")" << endl;
                old_capacity = new_capacity;
            }
        }
    }
} // namespace Demo


