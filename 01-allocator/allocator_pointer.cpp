#include "allocator_pointer.h"

Pointer::Pointer(){//делает пустой умный указатель
    ptr = nullptr; 
}

Pointer::Pointer(point *new_ptr){//присваивает данный указатель умному
    if (new_ptr != nullptr){
        ptr = new_ptr;
    }
    else ptr = nullptr;
}

void* Pointer::get() const{//выдаёт указатель на первый байт памяти, на который ссылался указатель ptr
    if (ptr == nullptr){    
        return nullptr;
    }
    return ptr->first;
}

point* Pointer::get_ptr() const{//выдаёт указатель внутри умного
    return ptr;
}

size_t Pointer::get_size(){//выдаёт длину ячейки
    if(ptr == nullptr) 
        return 0;
    if(ptr->first == nullptr) 
        return 0;

    return static_cast<char *>(ptr->last) - static_cast<char *>(ptr->first);
}

void Pointer::free(){//освобождаем указатель
    /*
    if (ptr != nullptr){
        ptr->last = ptr->first;
        ptr = nullptr;
    }
    */
}
