#ifndef __ITERATOR_H
#define __ITERATOR_H
#include <cassert>
#include "node.h"

/**
 * Skiplist const iterator
 */
template<class Key, class Value>
class Iterator {
private:
  Node<Key, Value> *pCurrent;

public:
  Iterator(Node<Key,Value> *p) : pCurrent(p) {}//деструктор
  virtual ~Iterator() {}

  virtual const Key& key() const {//выжаёт ключ узла. Ключ - уникальный номер узла. То есть у узла должно быть 4 характристики: ключ, значение, следующий, предыдущий узел и количество слоёв.
    assert(pCurrent != nullptr);//кидает ошибку при логическом нуле внутри функции
    return pCurrent->key();
  };

  virtual const Value& value() const {//выёт значение узла
    assert(pCurrent != nullptr);
    return pCurrent->value();
  };

  virtual const Value& operator*() {
    assert(pCurrent != nullptr);
    return pCurrent->value();
  };

  virtual const Value& operator->() {
    assert(pCurrent != nullptr);
    return pCurrent->value();
  };

  virtual bool operator==(const Iterator& other) const {//описывает сравнение двух узлов
      return pCurrent == other.pCurrent; 
  };

  virtual bool operator!=(const Iterator& other) const {
    return pCurrent != other.pCurrent;
  };


  virtual Iterator& operator=(const Iterator& other) {//присваивание значения из узла в узел
      pCurrent = other.pCurrent;
      return *this;
  };

  virtual Iterator& operator++() {//переход к следующему узлу
      pCurrent = &pCurrent->next();
      return *this;
  };

  virtual Iterator& operator++(int num) {//?
      for (int i = 0; i < num; num++)
          (*this)++;
      return *this;
  };
};

#endif // __ITERATOR_H
