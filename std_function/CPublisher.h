#pragma once
#include <iostream>
#include <functional>
#include <map>

/**
 * \brief a subscriber class to register callbacks
 */
class Publisher{
    std::map<size_t, std::function<void(const char *, int)>> subscribers;

    // https://stackoverflow.com/questions/18039723/c-trying-to-get-function-address-from-a-stdfunction
    // get address of std::function directing to function callback
    template<typename T, typename... U>
    size_t getAddress(std::function<T(U...)> f) {
        typedef T(fnType)(U...);
        fnType ** fnPointer = f.template target<fnType*>();
        if (fnPointer != nullptr){
            return (size_t) *fnPointer;
        }
        return 0;
    }

    template <typename Function>
    struct function_traits
            : public function_traits<decltype(&Function::operator())> {
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...) const> {
        typedef ReturnType (*pointer)(Args...);
        typedef std::function<ReturnType(Args...)> function;
    };

    template <typename Function>
    typename function_traits<Function>::function
    to_function (Function & lambda) {
        return static_cast<typename function_traits<Function>::function>(lambda);
    }

    // get address of lambda
    template <typename Lambda>
    size_t getAddressLambda(Lambda lambda) {
        auto function = new decltype(to_function(lambda))(to_function(lambda));
        void * func = static_cast<void *>(function);
        return (size_t)func;
    }

public:
    /**
     * \brief call all registered subscribers with the given arguments
     */
    void call(const char* arg1, int arg2);

    /**
     * \brief remove all registered subscribers
     */
    void clear();

    /**
     * \brief get number of registered subscribers
     */
    size_t getSize();

    /**
     * \brief print number of registered subscribers to std::out
     */
    void printSize();

    /**
     * \brief   register a new callback
     * \return  address of the callback, can be used to unsubscribe
     */
    size_t subscribe (std::function<void(const char*, int)> callback);

    /**
     * \brief deregister a callback using the std::function interface
     */
    void unsubscribe(std::function<void(const char*, int)> callback);

    /**
     * \brief deregister a callback using its address
     * \param address was obtained by using using \ref subscribe
     */
    void unsubscribe(size_t address);
};