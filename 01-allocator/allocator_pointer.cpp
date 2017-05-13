#include "allocator_pointer.h"

Pointer::Pointer(){
    ptr = nullptr; 
}

Pointer::Pointer(point *new_ptr){
    if (new_ptr != nullptr){
        ptr = new_ptr;
    }
    else ptr = nullptr;
}

void* Pointer::get() const{
    if (ptr == nullptr){    
        return nullptr;
    }
    return ptr->first;
}

/* узнать размер выделенной памяти */
size_t Pointer::get_size(){
    if(ptr == nullptr) 
        return 0;
    if(ptr->first == nullptr) 
        return 0;

    return static_cast<char *>(ptr->last) - static_cast<char *>(ptr->first);
}

void Pointer:: free(){
    if (ptr != nullptr){
        ptr->last =  ptr->first;
        ptr = nullptr;
    }
}
