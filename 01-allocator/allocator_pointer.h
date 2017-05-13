#ifndef ALLOCATOR_POINTER
#define ALLOCATOR_POINTER
#include <cstddef>
#include <iostream>
#include <set>

using namespace std;
// Forward declaration. Do not include real class definition
// to avoid expensive macros calculations and increase compile speed
class Allocator;

struct point{
    void* first;
    void* last;
    point* next;
    point* prev;
};


class Pointer {
public:
    Pointer();
    Pointer(point *new_ptr);
    ~Pointer(){};

    void *get() const;
    size_t get_size();
    void free();

private:
    point* ptr;
};

#endif //ALLOCATOR_POINTER