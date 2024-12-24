#include "Baker.hpp"

Baker::Baker()
{
    this->state = WAIT_ORDER;
}
Baker::Baker(std::string customers_name,std::string customers_order,
            pthread_mutex_t* shared_space_lock, pthread_mutex_t* oven_lock)
{
    this->state = WAIT_ORDER;
    this->queue = CustomerQueue(customers_name,customers_order,oven_lock);
    this->shared_space_lock = shared_space_lock;
    this->oven_lock = oven_lock;
    while(!this->queue.finish_orders()){
        this->get_order();
        this->bake_breads();
        this->put_breads_in_space();
    }
    std::cout << "bakery closed" << std::endl;
}
void Baker::get_inputs(std::string customers_name,std::string customers_order,
            pthread_mutex_t* shared_space_lock, pthread_mutex_t* oven_lock)
{
    this->state = WAIT_ORDER;
    this->queue = CustomerQueue(customers_name,customers_order,oven_lock);
    this->shared_space_lock = shared_space_lock;
    this->oven_lock = oven_lock;
}
void Baker::get_order()
{
    this->now_customer = this->queue.order();
    std::cout << "Backer" << this->id << " gets order of customer" << now_customer.name << std::endl;
    this->state = WAIT_BACKING;
}
void Baker::put_breads_in_space()
{
    std::cout << "Backer" << this->id << " try to put breads on space"  << std::endl;
    while(true){
        if(pthread_mutex_trylock(this->shared_space_lock) == 0){
            this->queue.get_bread();
            pthread_mutex_unlock(this->shared_space_lock);
            break;
        }        
    }
}
void Baker::sleep_until_bake(){
    int oven_capacity = (this ->baker_number * 10);
    while(this->now_customer.count > this->now_customer.done + oven_capacity){
        std::this_thread::sleep_for(std::chrono::milliseconds(std::chrono::milliseconds(500)));
        int baked_breads = (this->now_customer.count > oven_capacity? oven_capacity:this->now_customer.count);
        this->now_customer.done += baked_breads;
        this->now_customer.count -= baked_breads;
    }
}
void Baker::bake_breads()
{
    std::cout << "Backer" << this->id << " trying to put breads on oven"  << std::endl;
    while(true){
        if(pthread_mutex_trylock(this->oven_lock) == 0){
            sleep_until_bake();
            pthread_mutex_unlock(this->oven_lock);
            this->state = WAIT_DELIVER;
            break;
        }        
    }
}
void *Baker::start(void* baker_void){
    Baker* baker = static_cast<Baker*>(baker_void);
    std::cout << "Baker "  << baker->id << " thread started succesfully" << std::endl;
    while(!baker->queue.finish_orders()){
        baker->get_order();
        baker->bake_breads();
        baker->put_breads_in_space();
    }
    std::cout << "backers end" << std::endl;
}
void Baker::set_baker_number(int baker_number)
{
    this->baker_number = baker_number;
}
void Baker::set_id(int id)
{
    this->id = id;
}
Baker::~Baker()
{
}
