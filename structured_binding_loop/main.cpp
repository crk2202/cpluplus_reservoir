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
void iterateStructuredBinding_move(std::unordered_map<KEY, VALUE> &mmap) {
    for ( auto&&[key, val] : mmap ) {
        val.push_back(key);
    }
}

template<typename KEY, typename VALUE>
void iterateStructuredBinding_ref(std::unordered_map<KEY, VALUE> &mmap) {
    for ( auto& [key, val] : mmap ) {
        val.push_back(key);
    }
}


template<typename KEY, typename VALUE>
void iterateIterator(std::unordered_map<KEY, VALUE> &mmap) {
    for ( typename std::unordered_map<KEY, VALUE>::iterator iter = mmap.begin();
          iter != mmap.end(); iter++ ) {
        iter->second.push_back(iter->first);
    }
}

template<typename KEY, typename VALUE>
void iterateGenericStructuredBinding_move(std::unordered_map<KEY, VALUE> &mmap) {
    for ( auto&&[key, val] : mmap ) {
        if constexpr (std::is_same<KEY, std::string>::value) {
            val.append(key);
        }
        if constexpr (!std::is_same<KEY, std::string>::value){
            val.push_back(key);
        }
    }
}

int main() {
    std::unordered_map<int, std::string> map1, map2, map3, map4;
    for ( int i = 0; i < 5000000; i++ ) {
        map1.emplace(i, std::to_string(i));
        map2.emplace(i, std::to_string(i));
        map3.emplace(i, std::to_string(i));
        map4.emplace(i, std::to_string(i));
    }


    std::cout << "iterateIterator<int, std::string> " << measure<>::execution(iterateIterator<int, std::string>, map1) << std::endl;
    std::cout << "iterateStructuredBinding_ref<int, std::string> " << measure<>::execution(iterateStructuredBinding_ref<int, std::string>, map2) << std::endl;
    std::cout << "iterateStructuredBinding_move<int, std::string> " << measure<>::execution(iterateStructuredBinding_move<int, std::string>, map3) << std::endl;
    std::cout << "iterateGenericStructuredBinding_move<int, std::string>" << measure<>::execution(iterateGenericStructuredBinding_move<int, std::string>, map4) << std::endl;

    return 0;
}