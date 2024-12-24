#include"CustomerQueue.hpp"


CustomerQueue::CustomerQueue(std::string names,std::string counts,pthread_mutex_t* oven_lock){
    std::istringstream nameStream(names);
    std::vector<std::string> customer_names;
    std::string name;
    this->is_disorder = false;
    while (nameStream >> name) {
        customer_names.push_back(name);
    }

    std::istringstream order_stream(counts);
    std::vector<int> customer_orders;
    int order;
    while (order_stream >> order) {
        customer_orders.push_back(order);
    }
    for(int i=0;i<customer_names.size(); i++){
        Customer new_customer;
        new_customer.count = customer_orders[i];
        new_customer.name = customer_names[i];
        this->customers.push_back(new_customer);
    }
    this->oven_lock = oven_lock;
}

CustomerQueue::~CustomerQueue()
{
}
CustomerQueue::CustomerQueue()
{
}
Customer CustomerQueue::order(){
    Customer now_customer;
    if(!this->is_disorder){
        now_customer = this->customers[0];
    }
    else {
        
    }
    return now_customer;
}
void CustomerQueue::get_bread(){
    std::cout << "customer " << this->customers[0].name << " get his breads" << std::endl;
    this->customers.erase(this->customers.begin());
}
bool CustomerQueue::finish_orders(){
    return this->customers.empty();
}