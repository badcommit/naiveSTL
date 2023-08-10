#include <iostream>

#include <cstring>

#include "naiveSTL/utility.h"
#include "naiveSTL/thread_pool.h"
#include <thread>
using namespace NaiveSTL;

//template<class T>
//T max(T a, T b){
//    return b < a ? a : b;
//}

template<class T>
T const & max(T const &a, T const &b){
    return b < a ? a : b;
}

char const * max(char const *a, char const *b){
    return std::strcmp(b, a) < 0 ? a : b;
}

template<class T>
T const & max(T const &a, T const &b, T const &c){
    return max(max(a, b), c);
}

int main() {

//
//    vector_test::testCase1();

//    char const *a = "abc";
//    char const *b = "abcd";
//    char const *c = "deffdsf";
//    auto m3 = max(a, b, c);
//    printf("%s\n", m3);
    auto threadpool = ThreadPool(4);
    threadpool.start();
    threadpool.submit([](){
        printf("hello world\n");
    });

    threadpool.submit([](){
        printf("hello world 2\n");
    });
    threadpool.submit([](){
        printf("hello world 3\n");
    });

    threadpool.submit([](){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("hello world 4\n");
    });

    threadpool.waitUntilAllStarted();
    return 0;
}
