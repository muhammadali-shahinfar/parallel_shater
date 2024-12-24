#include<iostream>
#include "Baker.hpp"
#include "CustomerQueue.hpp"
#include <thread>
#include <sstream>

int main(){
    int input_baker_numbers;
    std::cout << "Enter bakers number:" << std::endl;
    std::cin >> input_baker_numbers;
    std::string new_line;
    getline(std::cin, new_line);
    int baker_number = input_baker_numbers;
    pthread_t baker_threads[baker_number];
    Baker bakers[baker_number];
    pthread_mutex_t oven_lock;
    pthread_mutex_init(&oven_lock,NULL);
    pthread_mutex_t shared_table_lock;
    pthread_mutex_init(&shared_table_lock,NULL);
    for(int i =0;i < baker_number;i++){
        std::string customer_names, orders;
        std::cout << "Enter customer names: ";
        getline(std::cin, customer_names);

        std::cout << "Enter orders (space-separated): ";
        getline(std::cin, orders);
        bakers[i].get_inputs(customer_names,orders,&shared_table_lock,&oven_lock);
        bakers[i].set_baker_number(baker_number);
        bakers[i].set_id(i + 1);

    }
    for(int i=0;i<baker_number;i++){
        pthread_create(&baker_threads[i], nullptr,Baker::start,(void*) &bakers[i]);
    }
    for(int i=0;i<baker_number;i++){
        std::cout << "waiting to join threads" << std::endl;
        pthread_join(baker_threads[i],nullptr);
        std:: cout << "thread " << i << " joined" << std::endl;
    }

}