#include <iostream>
#include <string>

#include "dynamic_arry.hpp"
#include "new_array.hpp"
#include "list.hpp"
#include "forward_list.hpp"

//вывод любого контейнера
template <typename Container>
void print_container(const Container& c) {
    for (size_t i = 0; i < c.size(); ++i) std::cout << c[i] << " ";
    std::cout << std::endl;
}

template <typename Container>
void demo(Container& c, const std::string& title) {
    std::cout << title << std::endl;
    for (int i = 0; i < 10; ++i) c.push_back(i);
    print_container(c);
    std::cout << "size = " << c.size() << std::endl;    

    // удаление с конца из-за смещения индексов
    c.erase(6);   
    c.erase(4);   
    c.erase(2);   
    print_container(c);                            

    c.push_front(10);
    print_container(c);                       

    c.insert(4, 20);
    print_container(c);                      

    c.push_back(30);
    print_container(c);
    std::cout << std::endl;
}

int main() {
    dynamicarry<int> a;
    demo(a, "Dynamic_array");

    newdynamicarry<int> c;
    demo(c, "NewDynamic_array");

    list<int> b;
    demo(b, "Bidirectional list");

    forward_list<int> f;
    demo(f, "Forward list");

    std::cout << "Iterator demo (list)" << std::endl;
    list<int> it_list;
    for (int i = 0; i < 10; ++i) it_list.push_back(i);
    for (auto it = it_list.begin(); it != it_list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Iterator demo (forward_list)"<< std::endl;
    forward_list<int> it_f;
    for (int i = 0; i < 10; ++i) it_f.push_back(i);
    for (auto it = it_f.begin(); it != it_f.end(); ++it) {
        std::cout << it.get() << " ";
    }
    std::cout << std::endl;

    std::cout << "Move semantics demo" << std::endl;
    dynamicarry<int> moved_from;
    for (int i = 0; i < 5; ++i) moved_from.push_back(i);
    dynamicarry<int> moved_to = std::move(moved_from);
    std::cout << "moved_to:   "; print_container(moved_to);
    std::cout << "moved_from size = " << moved_from.size() << std::endl;

    return 0;
}