#ifndef ALLOCATOR
#define ALLOCATOR
#include <string>
#include "allocator_pointer.h"
#include <cstring>

// Forward declaration. Do not include real class definition
// to avoid expensive macros calculations and increase compile speed
class Pointer;

/**
 * Wraps given memory area and provides defagmentation allocator interface on
 * the top of it.
 *
 *
 */
class Allocator {
public:
    Allocator(void* base, size_t base_size);
    /**
     * TODO: semantics
     * @param N size_t
     */
    Pointer alloc(size_t N);
    Pointer _alloc_root(size_t N);
    Pointer _alloc_last_node(size_t N);
    Pointer _alloc_find_free_place(size_t N);
    /**
     * TODO: semantics
     * @param p Pointer
     * @param N size_t
     */

    void realloc(Pointer& pointer, size_t N);
    /**
     * TODO: semantics
     * @param p Pointer
     */
    void free(Pointer& p);

    /**
     * TODO: semantics
     */
    void defrag();

    /**
     * TODO: semantics
     */
    std::string dump() const { return ""; }

private:
    point *free_points;//указатель на первую свободную ячейку, которая содержала когда-то информацию
    point buff;//выделенная нам память, first - начало, last - конец
    point *root;//указатель на конец буфера, начиная с которого у нас задом наперёд идут ячейки с информацией о записанных данных (далее - текущий)
    point *last_node;//указатель на последний элемент списка
};

#endif // ALLOCATOR
