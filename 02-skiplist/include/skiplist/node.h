#ifndef __NODE_H
#define __NODE_H
#include <cassert>

/**
 * Skiplist Node
 */
template <class Key, class Value>
class Node {
public:
    Node() {}
    virtual ~Node() {}

    /**
   * Return key assosiated with the given node
   */
    virtual const Key& key() const = 0;

    /**
   * Returns value assosiated with the given node
   */
    virtual Value& value() = 0;

    /**
   * Returns next node in the sequence
   */
    virtual Node& next() const = 0;
};

/**
 * Skiplist data nodes that holds actual key/value pair
 */
template <class Key, class Value>
class DataNode : public Node<Key, Value> {
private:
    const Key* pKey;
    Value* pValue;
    DataNode<Key, Value>* pNext;

public:
    DataNode(const Key* pKey, Value* pValue)
        : pKey(pKey)
        , pValue(pValue)
        , pNext(nullptr) {
    }

    virtual ~DataNode() {}

    /**
   * Return key assosiated with the given node
   */
    virtual const Key& key() const {
        assert(pKey != nullptr);
        return *pKey;
    }

    /**
   * Returns value assosiated with the given node
   */
    virtual Value& value() {
        assert(pValue != nullptr);
        return *pValue;
    };
  /**
   * Returns value assosiated with the given node
   */
    virtual void set_value(Value *val) {
        delete pValue;  
        pValue = val;
    };

    /**
   * Returns next node in the sequence
   */
    virtual Node<Key, Value>& next() const {
        return *pNext;
    };

    /**
   * Set next pointer
   */
    virtual void next(DataNode<Key, Value>* next) {
        pNext = next;
    };
};

/**
 * Skiplist index nodes that keep references onto data layer
 */
template <class Key, class Value>
class IndexNode : public Node<Key, Value> {
private:
    Node<Key, Value>* pDown;
    Node<Key, Value>* pRoot;
    IndexNode<Key, Value>* pNext;

public:
    IndexNode(Node<Key, Value>* down, Node<Key, Value>* root)
        : pDown(down)
        , pRoot(root)
        , pNext(nullptr) {
    }

    virtual ~IndexNode() {}

  /**
   * Return key assosiated with the given node
   */
    virtual const Key& key() const {
        assert(pRoot != nullptr);
        return pRoot->key();
    }

    /**
   * Returns value assosiated with the given node
   */
    virtual Value& value() {
        assert(pRoot != nullptr);
        return pRoot->value();
    };

    /**
   * Returns next node in the sequence
   */
    virtual Node<Key, Value>& next() const {
        return *pNext;
    };

    /**
   * Set next pointer
   */
    virtual void next(IndexNode<Key, Value>* next) {
        pNext = next;
    };
  

  /**
   * Returns the root node
   */  
    virtual Node<Key, Value>& root() const {
        return *pRoot;
    } 


  /**
   * Returns the down node
   */  
    virtual Node<Key, Value>& down() const {
        return *pDown;
    }

};
#endif // __NODE_H
