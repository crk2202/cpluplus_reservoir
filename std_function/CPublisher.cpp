

#include "CPublisher.h"

void Publisher::call(const char *arg1, int arg2) {
        for ( auto entry: subs ) { entry.second(arg1, arg2); }
}

void Publisher::clear() {
        subs.clear();
}

void Publisher::printSize() {
        std::cout << "Subscriber size: " << subs.size() << std::endl;
}

size_t Publisher::subscribe(std::function<void(const char *, int)> callback) {
        auto address = getAddress(callback);

        if (address == 0) {
                // std::function wraps a lambda
                address = getAddressLambda(callback);

        }

        subs.insert(std::make_pair(address, callback));
        return address;
}

void Publisher::unsubscribe(std::function<void(const char *, int)> callback) {
        auto address = getAddress(callback);
        if (address == 0) {
                // std::function wraps lambda
                address = getAddressLambda(callback);

        }
        subs.erase(address);
}

void Publisher::unsubscribe(size_t address) {
        subs.erase(address);
}
