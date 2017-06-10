#ifndef ALLOCATOR_POINTER
#define ALLOCATOR_POINTER
#include <cstddef>
#include <iostream>
#include <set>
#include <cstring>

using namespace std;
// Forward declaration. Do not include real class definition
// to avoid expensive macros calculations and increase compile speed
class Allocator;

struct point{
    void* first;//указатель на первый байт памяти
    void* last;//указатель на последний
    point* next;//следующая ячейка
    point* prev;//предыдущая ячейкака
};


class Pointer {
public:
    Pointer();//прототипы, сами функции объявлены в .cpp, создаёт пустой умный указатель
    Pointer(point *new_ptr);//делает из данного указателя "умный"
    ~Pointer(){};

    void *get() const;//указатель на первый блок памяти
    point *get_ptr() const;//сам указатель
    size_t get_size();//размер объекта по указателю
    void free();//очистить содержимое указателя

private:
    point* ptr;//сам указатель
};

#endif //ALLOCATOR_POINTER