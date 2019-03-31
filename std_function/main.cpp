#include "CPublisher.h"
#include <iostream>


// the callbacks
void my_func(const char* arg1, int arg2){
    std::cout << "my_func: " << arg1 << "  "<< arg2 << std::endl;
}

void my_func2(const char* arg1, int arg2){
    std::cout << "my_func2: " << arg1 << "  "<< arg2 << std::endl;
}

auto lambda = [](const char* arg1, int arg2){
    std::cout << "lambda: " << arg1 << "  "<< arg2 << std::endl;
};

struct functor{
    int operator ()(const char* arg1, int arg2){
        std::cout << "functor operator() " << arg1 << "  "<< arg2 << std::endl;
    }
};

int main() {
    Publisher pub;

    pub.subscribe(my_func);
    pub.subscribe(my_func2);
    auto address = pub.subscribe(lambda);

    int i = 12;
    auto lambda_capture = [&i](const char* arg1, int arg2){
        std::cout << "lambda with capture: " << arg1 << "  "<< arg2 << i <<std::endl;
    };

    auto address2 = pub.subscribe(lambda_capture);

    functor func;
    auto address3 = pub.subscribe(func);

    pub.call("call1",12);
    pub.printSize();

    std::cout << "unsubscribe" << std::endl;

    pub.unsubscribe(my_func);
    pub.unsubscribe(my_func2);
    pub.unsubscribe(lambda);
    pub.unsubscribe(address2);
    pub.unsubscribe(address3);
    pub.call("call1",12);
    pub.printSize();


    pub.clear();
    pub.printSize();

    return 0;
}
