//
// Created by Nik Kuchin on 26.06.2020.
//

#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template<class T>
class ObjectPool {
public:
    T *Allocate() {
        T *returned = nullptr;
        if (free_.empty()) {
            returned = new T;
        } else {
            returned = free_.front();
            free_.pop();
        }
        taken_.insert(returned);
        return returned;
    }

    T *TryAllocate() {
        if (free_.empty()) {
            return nullptr;
        }
        T *returned = free_.front();
        free_.pop();
        taken_.insert(returned);
        return returned;
    }

    void Deallocate(T *object) {
        auto found = taken_.find(object);
        if (found == taken_.end()) {
            throw invalid_argument("Deallocation error: no such object exists");
        } else {
            free_.push(*found);
            taken_.erase(found);
        }
    }

    ~ObjectPool() {
        while (!free_.empty()) {
            delete free_.front();
            free_.pop();
        }
        while (!taken_.empty()) {
            delete *taken_.begin();
            taken_.erase(taken_.begin());
        }
    }

private:
    queue<T *> free_;
    set<T *> taken_;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}