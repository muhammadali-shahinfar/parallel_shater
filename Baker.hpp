#ifndef __BAKER_HPP__
#define __BAKER_HPP__

#include "CustomerQueue.hpp"
#include <csignal>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <thread>

enum State {
    WAIT_ORDER,
    WAIT_BACKING,
    WAIT_DELIVER
};
    
class Baker
{
private:
    enum State state;
    CustomerQueue queue;
    Customer now_customer;
    pthread_mutex_t* shared_space_lock;
    pthread_mutex_t* oven_lock;
    pthread_cond_t* oven_cond;
    void sleep_until_bake();
    int* oven_free_capacity;
public:
    int id;
    Baker();
    Baker(std::string customers_name,std::string customers_order,
            pthread_mutex_t* shared_space_lock, pthread_mutex_t* oven_lock);
    ~Baker();
    void get_order();
    void bake_breads();
    void put_breads_in_space();
    void get_inputs(std::string customers_name,std::string customers_order,
            pthread_mutex_t* shared_space_lock, pthread_mutex_t* oven_lock);

    static void* start(void*);
    void set_oven_free_capacity(int* oven_free_capacity);
    void set_id(int id);
    void set_oven_cond_var(pthread_cond_t*);

    

};

#endif