#include "allocator.h"
#include "allocator_pointer.h"
#include "allocator_error.h"


Allocator::Allocator(void* base, size_t base_size){
    buff.first = base;
    buff.last = static_cast<char *>(base) + base_size - sizeof(point) - 3;
    root = static_cast<point *>(buff.last);
    root->prev = nullptr;
    root->next  = nullptr; 
    root->first = nullptr; 
    root->last  = nullptr;
    last_node = root;
}

Pointer Allocator::_alloc_root(size_t N){
    root->first = buff.first;
    root->last = static_cast<void*>(static_cast<char*>(buff.first) + N);
    return Pointer(root);
}

Pointer Allocator::_alloc_last_node(size_t N){
        last_node--;
        root->next = last_node; // добавили новый элемент
        last_node->prev = root; // указатель на следующий из предыдущего = текущий 
        last_node->first = root->last; // начало новой строки - конец предыдущей
        last_node->last = static_cast<void*>(static_cast<char*>(root->last) + N);
        last_node->next = nullptr; // следующего пока нет       
        root = last_node; // last_node = новая верхушка
        return Pointer(root);
}

Pointer Allocator::_alloc_find_free_place(size_t N){
    for(point *cur = static_cast<point *>(buff.last); cur->next != nullptr; cur = cur->next){
        if((cur->next)->first != nullptr){
            size_t sz = static_cast<char*>((cur->next)->first) - static_cast<char*>(cur->last); // размер кусочка
            if(sz >= N){
                last_node--;
                last_node->prev = cur;
                last_node->next = cur->next;
                last_node->first = cur->last;
                last_node->last = static_cast<void*>(static_cast<char*>(cur->last) + N);
                (cur->next)->prev = last_node;
                cur->next = last_node;

                return Pointer(last_node);
            }
        }
    }

throw AllocError(AllocErrorType::NoMemory, "Try defraq");
}

/* выделяем память размера N */
Pointer Allocator::alloc(size_t N) {
    // Память в буфере ещё не выделялась
    // просто создаём нулевой элемент в корне root
    if (root->prev  == nullptr && 
        root->next  == nullptr && 
        root->first == nullptr && 
        root->last  == nullptr &&
        N + sizeof(point) < static_cast<char*>(buff.last) - static_cast<char*>(buff.first))
        return _alloc_root(N);

    // добавляем в конец списка
    if (last_node > static_cast<void*>(static_cast<char*>(last_node->last) + N + sizeof(point)))
        return _alloc_last_node(N);


    // посмотрим, есть ли место в дырках 
    return _alloc_find_free_place(N);
}


void Allocator::defrag() {
    int i = 0;
    for(point *cur = static_cast<point*>(buff.last)->next; cur != nullptr; cur = cur->next){
        if(cur->first != nullptr) {
            if (cur->first > cur->prev->last) {
                size_t sz = static_cast<char*>(cur->last) - static_cast<char*>(cur->first);
                memcpy(cur->prev->last, cur->first, sz);
                cur->first = cur->prev->last;
                cur->last = static_cast<char*>(cur->first) + sz;
            }
        }
        else{
            cur->last = cur->prev->last;
        }
    }
}



void Allocator::realloc(Pointer& pointer, size_t N) {
    void *info_link = pointer.get();
    size_t size = pointer.get_size();
    free(pointer);    
    pointer = alloc(N);    
    memcpy(pointer.get(), info_link, size);    
}

void Allocator::free(Pointer& p){ 
    p.free();
    p = nullptr;
}
