#ifndef LIST_TYPE_H
#define LIST_TYPE_H

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename Type>
struct NodeType {
    Type data;
    NodeType<Type>* next;
    NodeType<Type>* prev;
};

template <typename Type>
class ListType {
public:
    ListType() noexcept : count(0), head(nullptr), tail(nullptr) {}

    ListType(const ListType& other) : ListType() {
        try {
            for (const NodeType<Type>* current = other.head;
                 current != nullptr; current = current->next) {
                appendCopy(current->data);
            }
        } catch (...) {
            destroyList();
            throw;
        }
    }

    ListType(ListType&& other) noexcept
        : count(other.count), head(other.head), tail(other.tail) {
        other.count = 0;
        other.head = nullptr;
        other.tail = nullptr;
    }

    ListType& operator=(const ListType& other) {
        if (this == &other) {
            return *this;
        }

        // Build a complete independent chain first. If allocation or a Type
        // copy throws, the current list remains unchanged.
        NodeType<Type>* newHead = nullptr;
        NodeType<Type>* newTail = nullptr;
        int newCount = 0;
        try {
            for (const NodeType<Type>* current = other.head;
                 current != nullptr; current = current->next) {
                NodeType<Type>* node = new NodeType<Type>{current->data, nullptr, newTail};
                if (newTail != nullptr) {
                    newTail->next = node;
                } else {
                    newHead = node;
                }
                newTail = node;
                ++newCount;
            }
        } catch (...) {
            while (newHead != nullptr) {
                NodeType<Type>* next = newHead->next;
                delete newHead;
                newHead = next;
            }
            throw;
        }

        destroyList();
        head = newHead;
        tail = newTail;
        count = newCount;
        return *this;
    }

    ListType& operator=(ListType&& other) noexcept {
        if (this != &other) {
            destroyList();
            count = other.count;
            head = other.head;
            tail = other.tail;
            other.count = 0;
            other.head = nullptr;
            other.tail = nullptr;
        }
        return *this;
    }

    virtual ~ListType() { destroyList(); }

    bool operator==(const ListType& other) const {
        if (count != other.count) {
            return false;
        }

        const NodeType<Type>* left = head;
        const NodeType<Type>* right = other.head;
        while (left != nullptr && right != nullptr) {
            if (!(left->data == right->data)) {
                return false;
            }
            left = left->next;
            right = right->next;
        }
        return left == nullptr && right == nullptr;
    }

    bool operator!=(const ListType& other) const { return !(*this == other); }

    void initializeList() { destroyList(); }

    bool isEmptyList() const noexcept { return head == nullptr; }

    int length() const noexcept { return count; }

    void destroyList() noexcept {
        NodeType<Type>* current = head;
        while (current != nullptr) {
            NodeType<Type>* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    Type front() const {
        if (isEmptyList()) {
            throw std::out_of_range("front() called on an empty list");
        }
        return head->data;
    }

    Type back() const {
        if (isEmptyList()) {
            throw std::out_of_range("back() called on an empty list");
        }
        return tail->data;
    }

    void print() const {
        for (const NodeType<Type>* current = head;
             current != nullptr; current = current->next) {
            std::cout << current->data << ' ';
        }
    }

    virtual void print(bool forward) const = 0;
    virtual bool search(const Type& searchItem) const = 0;
    virtual void insertFirst(const Type& newItem) = 0;
    virtual void insertLast(const Type& newItem) = 0;
    virtual void insertNode(const Type& newItem, int index) = 0;
    virtual Type replace(const Type& newItem, int index) = 0;
    virtual void deleteNode(const Type& deleteItem) = 0;

protected:
    int count;
    NodeType<Type>* head;
    NodeType<Type>* tail;

    void swap(ListType& other) noexcept {
        using std::swap;
        swap(count, other.count);
        swap(head, other.head);
        swap(tail, other.tail);
    }

    void appendCopy(const Type& value) {
        NodeType<Type>* node = new NodeType<Type>{value, nullptr, tail};
        if (tail != nullptr) {
            tail->next = node;
        } else {
            head = node;
        }
        tail = node;
        ++count;
    }
};

#endif // LIST_TYPE_H
