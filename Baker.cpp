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
    std::cout << "Backer" << this->id << " gets order of customer " << now_customer.name << std::endl;
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
    std::cout << "Backer " << this->id << " puted breads on oven" << std::endl;
    int baked_breads = (this->now_customer.count > *this->oven_free_capacity? *this->oven_free_capacity:this->now_customer.count);
    this->now_customer.count -= baked_breads;
    *(this->oven_free_capacity) -= baked_breads;
    pthread_mutex_unlock(this->oven_lock);
    std::this_thread::sleep_for(std::chrono::milliseconds(std::chrono::milliseconds(5000)));
    while(true)
        if(pthread_mutex_trylock(this->oven_lock) == 0){
            *(this->oven_free_capacity) += baked_breads;
            std::cout << "Backer" << this->id << "baked breads" << std::endl;
            pthread_mutex_unlock(this->oven_lock);
            break;
        }
    
    
}
void Baker::bake_breads()
{
    std::cout << "Backer" << this->id << " trying to put breads on oven"  << std::endl;
    while(this->now_customer.count != 0){
        if(*(this->oven_free_capacity) != 0 && pthread_mutex_trylock(this->oven_lock) == 0){
            sleep_until_bake();            
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
void Baker::set_oven_free_capacity(int* oven_free_capacity)
{
    this->oven_free_capacity = oven_free_capacity;
}
void Baker::set_id(int id)
{
    this->id = id;
}
void Baker::set_oven_cond_var(pthread_cond_t* cond_t)
{
    this->oven_cond = cond_t;
}

Baker::~Baker()
{
}
