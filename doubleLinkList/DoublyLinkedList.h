#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "ListType.h"

#include <iostream>
#include <stdexcept>

template <typename Type>
class DoublyLinkedList : public ListType<Type> {
    using Base = ListType<Type>;
    using Node = NodeType<Type>;

public:
    void print(bool forward) const override {
        const Node* current = forward ? this->head : this->tail;
        while (current != nullptr) {
            std::cout << current->data << ' ';
            current = forward ? current->next : current->prev;
        }
        std::cout << '\n';
    }

    bool search(const Type& searchItem) const override {
        for (const Node* current = this->head;
             current != nullptr; current = current->next) {
            if (current->data == searchItem) {
                return true;
            }
        }
        return false;
    }

    void insertFirst(const Type& newItem) override {
        Node* node = new Node{newItem, this->head, nullptr};
        if (this->head != nullptr) {
            this->head->prev = node;
        } else {
            this->tail = node;
        }
        this->head = node;
        ++this->count;
    }

    void insertLast(const Type& newItem) override {
        Node* node = new Node{newItem, nullptr, this->tail};
        if (this->tail != nullptr) {
            this->tail->next = node;
        } else {
            this->head = node;
        }
        this->tail = node;
        ++this->count;
    }

    // Valid insertion positions are [0, length()].
    void insertNode(const Type& newItem, int index) override {
        if (index < 0 || index > this->count) {
            throw std::out_of_range("insertNode: index is out of range");
        }
        if (index == 0) {
            insertFirst(newItem);
            return;
        }
        if (index == this->count) {
            insertLast(newItem);
            return;
        }

        Node* nextNode = nodeAt(index);
        Node* previousNode = nextNode->prev;
        Node* node = new Node{newItem, nextNode, previousNode};
        previousNode->next = node;
        nextNode->prev = node;
        ++this->count;
    }

    // Valid element positions are [0, length()). Returns the old value.
    Type replace(const Type& newItem, int index) override {
        Node* node = nodeAt(index);
        Type oldValue = node->data;
        node->data = newItem;
        return oldValue;
    }

    // Removes the first matching value, if present. No-op if absent.
    void deleteNode(const Type& deleteItem) override {
        Node* current = this->head;
        while (current != nullptr && !(current->data == deleteItem)) {
            current = current->next;
        }
        if (current == nullptr) {
            return;
        }
        unlinkAndDelete(current);
    }

    // Checks the structural invariants; useful for diagnostics and tests.
    bool validate() const noexcept {
        if (this->count < 0) return false;
        if (this->count == 0) return this->head == nullptr && this->tail == nullptr;
        if (this->head == nullptr || this->tail == nullptr) return false;
        if (this->head->prev != nullptr || this->tail->next != nullptr) return false;

        int forwardCount = 0;
        const Node* previous = nullptr;
        for (const Node* current = this->head; current != nullptr; current = current->next) {
            if (current->prev != previous) return false;
            previous = current;
            ++forwardCount;
        }
        if (previous != this->tail || forwardCount != this->count) return false;

        int backwardCount = 0;
        const Node* next = nullptr;
        for (const Node* current = this->tail; current != nullptr; current = current->prev) {
            if (current->next != next) return false;
            next = current;
            ++backwardCount;
        }
        return next == this->head && backwardCount == this->count;
    }

private:
    Node* nodeAt(int index) const {
        if (index < 0 || index >= this->count) {
            throw std::out_of_range("list index is out of range");
        }

        if (index < this->count / 2) {
            Node* current = this->head;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
            return current;
        }

        Node* current = this->tail;
        for (int i = this->count - 1; i > index; --i) {
            current = current->prev;
        }
        return current;
    }

    void unlinkAndDelete(Node* node) noexcept {
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            this->head = node->next;
        }

        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            this->tail = node->prev;
        }

        delete node;
        --this->count;
    }
};

#endif // DOUBLY_LINKED_LIST_H
