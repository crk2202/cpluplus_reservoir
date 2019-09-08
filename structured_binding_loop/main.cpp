#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

template<typename TimeT = std::chrono::microseconds>
struct measure {
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F &&func, Args &&... args) {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast<TimeT>
                (std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};

template<typename KEY, typename VALUE>
void iterate1(std::unordered_map<KEY, VALUE> &mmap) {
    for ( auto&&[key, val] : mmap ) {
        val.push_back(key);
    }
}

template<typename KEY, typename VALUE>
void iterate2(std::unordered_map<KEY, VALUE> &mmap) {
    for ( typename std::unordered_map<KEY, VALUE>::iterator iter = mmap.begin();
          iter != mmap.end(); iter++ ) {
        iter->second.push_back(iter->first);
    }
}

template<typename KEY, typename VALUE>
void iterate3(std::unordered_map<KEY, VALUE> &mmap) {
    for ( auto&&[key, val] : mmap ) {
        val.append(key);
    }
}

template<typename KEY, typename VALUE>
void iterate4(std::unordered_map<KEY, VALUE> &mmap) {
    for ( typename std::unordered_map<KEY, VALUE>::iterator iter = mmap.begin();
          iter != mmap.end(); iter++ ) {
        iter->second.append(iter->first);
    }
}

int main() {
    std::unordered_map<int, std::string> map1;
    std::unordered_map<int, std::string> map2;
    for ( int i = 0; i < 1000000; i++ ) {
        map1.emplace(i, std::to_string(i));
        map2.emplace(i, std::to_string(i));
    }


    std::cout << measure<>::execution(iterate1<int, std::string>, map1) << std::endl;
    std::cout << measure<>::execution(iterate2<int, std::string>, map2) << std::endl;


    std::unordered_map<std::string, std::string> map3;
    std::unordered_map<std::string, std::string> map4;
    for ( int i = 0; i < 1000000; i++ ) {
        auto r = std::rand();
        map3.emplace(std::to_string(r), std::to_string(i));
        map4.emplace(std::to_string(r), std::to_string(i));
    }

    std::cout << measure<>::execution(iterate3<std::string, std::string>, map3) << std::endl;
    std::cout << measure<>::execution(iterate4<std::string, std::string>, map4) << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}