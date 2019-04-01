#include "CPublisher.h"
#include <iostream>
#include <cassert>


// function callback
void my_func(const char* arg1, int arg2){
    std::cout << "my_func: " << arg1 << "  "<< arg2 << std::endl;
}

void my_func2(const char* arg1, int arg2){
    std::cout << "my_func2: " << arg1 << "  "<< arg2 << std::endl;
}

// lambda callback
auto lambda = [](const char* arg1, int arg2){
    std::cout << "lambda: " << arg1 << "  " << arg2 << std::endl;
};

// a lambda wrapping a function callback
auto lambda_func = [](const char *arg1, int arg2) {
    my_func2(arg1, arg2);
};

// functor callback
struct functor{
    int operator ()(const char* arg1, int arg2){
        std::cout << "functor operator() " << arg1 << "  " << arg2 << std::endl;
    }
};

int main() {
    Publisher pub;

    auto addr1 = pub.subscribe(my_func);
    auto addr2 = pub.subscribe(my_func2);
    auto addr3 = pub.subscribe(lambda);

    int i = 12;
    auto lambda_capture = [&i](const char *arg1, int arg2) {
        std::cout << "lambda with capture: " << arg1 << "  " << arg2 << i << std::endl;
    };

    auto addr4 = pub.subscribe(lambda_capture);

    functor func;
    auto addr5 = pub.subscribe(func);

    pub.call("call1", 12);
    assert(pub.getSize() == 5);
    pub.printSize();


    std::cout << "unsubscribe" << std::endl;
    pub.unsubscribe(my_func);
    assert(pub.getSize() == 4);
    pub.unsubscribe(my_func2);
    pub.unsubscribe(lambda);
    pub.unsubscribe(addr3);
    pub.call("call1", 12);
    assert(pub.getSize() == 2);
    pub.printSize();


    pub.unsubscribe(lambda_capture);
    pub.unsubscribe(addr4);
    assert(pub.getSize() == 1);
    pub.unsubscribe(func);
    pub.unsubscribe(addr5);
    assert(pub.getSize() == 0);
    pub.call("call1", 12);
    assert(pub.getSize() == 0);
    pub.printSize();


    pub.clear();
    assert(pub.getSize() == 0);
    pub.printSize();

    return 0;
}
