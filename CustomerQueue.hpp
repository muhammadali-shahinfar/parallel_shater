#ifndef __CUSTOMERQUEUE_HPP__
#define __CUSTOMERQUEUE_HPP__
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <chrono>
#include <cmath> 

struct Customer
    {
        std::string name;
        int count;

    };

class CustomerQueue
{
private:
    /* data */
    

    std::vector<Customer> customers;
    pthread_mutex_t* oven_lock;
    bool is_disorder;
    

public:
    CustomerQueue();
    CustomerQueue(std::string names,std::string counts,pthread_mutex_t* oven_lock);
    Customer order();
    void get_bread();
    bool finish_orders();
    ~CustomerQueue();
};
#endif
