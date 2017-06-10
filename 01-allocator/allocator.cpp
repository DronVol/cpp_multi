#include "allocator.h"
#include "allocator_pointer.h"
#include "allocator_error.h"


Allocator::Allocator(void* base, size_t base_size){
    buff.first = base;//указатель на начало выделенной области
    buff.last = static_cast<char *>(base) + base_size - sizeof(point);//указатель на конец
    root = static_cast<point *>(buff.last);//указатель на начальную точку ячеек с данными
    root->prev = nullptr;
    root->next  = nullptr; 
    root->first = nullptr; 
    root->last  = nullptr;
    last_node = root;
    free_points = nullptr;
}

Pointer Allocator::_alloc_root(size_t N){
    root->first = buff.first;//корневая ячейка начинает ссылаться на первый элемент в буфере
    root->last = static_cast<void*>(static_cast<char*>(buff.first) + N);//на последний
    return Pointer(root);
}

Pointer Allocator::_alloc_last_node(size_t N){
    if (free_points != nullptr){
        point *new_free_point = free_points->prev;
        free_points->prev = root;//указатель на следующий из предыдущего = текущий 
        free_points->first = root->last;//начало новой строки - конец предыдущей
        root->next = free_points;
        free_points->last = static_cast<void*>(static_cast<char*>(root->last) + N);
        root = free_points;
        free_points = new_free_point;       
        root->next = nullptr;
    }
    else{
        last_node--;
        root->next = last_node;//добавили новый элемент
        last_node->prev = root;//указатель на следующий из предыдущего = текущий 
        last_node->first = root->last;//начало новой строки - конец предыдущей
        last_node->last = static_cast<void*>(static_cast<char*>(root->last) + N);
        last_node->next = nullptr;//следующего пока нет       
        root = last_node;//last_node = новая верхушка    
    }    
    return Pointer(root);
    
}

Pointer Allocator::_alloc_find_free_place(size_t N){        

    for(point *cur = static_cast<point *>(buff.last); cur->next != nullptr; cur = cur->next){//идём по всем ячейкам в конце буфера
        if((cur->next)->first != nullptr){//если есть следующий
            size_t sz = static_cast<char*>((cur->next)->first) - static_cast<char*>(cur->last);//размер кусочка
            if(sz >= N){
                if (free_points != nullptr){
                    point *new_free_points = free_points->prev;
                    free_points->next = cur->next;
                    free_points->prev = cur;//указатель на следующий из предыдущего = текущий 
                    free_points->first = cur->last;//начало новой строки - конец предыдущей
                    free_points->last = static_cast<void*>(static_cast<char*>(cur->last) + N);

                    (cur->next)->prev = free_points;
                    point *new_one = free_points;
                    free_points = new_free_points;
                    return Pointer(new_one);
                }
                else{
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
    }   



throw AllocError(AllocErrorType::NoMemory, "Try defrag");
}

/* выделяем память размера N */
Pointer Allocator::alloc(size_t N) {    

    //Память в буфере ещё не выделялась
    //просто создаём нулевой элемент в корне root
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
                size_t sz = static_cast<char*>(cur->last) - static_cast<char*>(cur->first);//ищем, есть ли свободное место между соседними блоками
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
    point *prev = nullptr;
    point *get = p.get_ptr();

    if (get != nullptr){
        // Если free_points пока не существует
        if (free_points == nullptr){
            free_points = get;
            prev = nullptr;
        }
        else{
            // Если существует, то создадим новый
            free_points->next = get;
            prev = free_points;
            free_points = free_points->next;
        }


        // Если элемент не последний
        if (free_points->next != nullptr){

            // если не в начале
            if (free_points->prev != nullptr){        

                // меняем местами
                point *next = free_points->next;
                point *last = free_points->prev;
                next->prev = last;
                last->next = next;

            }
            else{
                // если он первый,
                // удаляем предыдущий элемент у следующего        
                (free_points->next)->prev = nullptr;
            }
        }
        else{        
            // если последний, значит он root!
            if (free_points->prev != nullptr){
                
                // если последний, но не первый,
                // то смещаем root назад
                //cout << "free_points->prev != nullptr\n";
                root = root->prev;
                root->next = nullptr;

                //free_points уже не равен root
            }
            else{            
                //если он последний и первый, то он просто root
                // и больше ничего нет
                // удалим его

                root->next = nullptr;
                root->last = nullptr;
                root->first = nullptr;
                root->last = nullptr;
            }
        }


        if (root != free_points){
            free_points->first = nullptr;
            free_points->last = nullptr;
            free_points->prev = prev;
            free_points->next = nullptr;
        }
        else{        
            // если попали сюда, то уже удлалили вообще всё
            free_points = prev;
            if (free_points != nullptr)
                free_points->next = nullptr;        
        }
    }

    p = nullptr;

}
