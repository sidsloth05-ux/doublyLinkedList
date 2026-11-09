#include "DoublyLinkedList.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

void testEmptyAndEndpoints() {
    DoublyLinkedList<int> list;
    assert(list.isEmptyList());
    assert(list.length() == 0);
    assert(list.validate());

    bool threw = false;
    try { list.insertNode(7, 1); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw && list.isEmptyList() && list.validate());
    threw = false;
    try { (void)list.replace(7, 0); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw && list.isEmptyList() && list.validate());
    threw = false;
    try { (void)list.front(); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw);
    threw = false;
    try { (void)list.back(); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    list.insertFirst(2);
    assert(list.front() == 2 && list.back() == 2);
    assert(list.validate());

    list.insertFirst(1);
    list.insertLast(3);
    assert(list.front() == 1 && list.back() == 3);
    assert(list.length() == 3);
    assert(list.validate());

    list.destroyList();
    assert(list.isEmptyList() && list.length() == 0);
    assert(list.validate());

    list.insertLast(9); // Reuse after destroyList().
    assert(list.front() == 9 && list.back() == 9);
    assert(list.validate());
}

void testIndexedInsertionAndReplacement() {
    DoublyLinkedList<int> list;
    list.insertNode(20, 0); // Empty list.
    list.insertNode(10, 0); // New head.
    list.insertNode(30, 2); // New tail.
    list.insertNode(25, 2); // Middle.

    assert(list.length() == 4);
    assert(list.front() == 10 && list.back() == 30);
    assert(list.validate());

    assert(list.replace(22, 1) == 20);
    assert(list.replace(5, 0) == 10);
    assert(list.replace(35, 3) == 30);
    assert(list.front() == 5 && list.back() == 35);
    assert(list.validate());

    bool threw = false;
    try { list.insertNode(99, -1); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw && list.length() == 4 && list.validate());

    threw = false;
    try { list.insertNode(99, 5); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw && list.length() == 4 && list.validate());

    threw = false;
    try { (void)list.replace(99, 4); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw && list.length() == 4 && list.validate());

    threw = false;
    try { (void)list.replace(99, -1); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw && list.length() == 4 && list.validate());
}

void testDeletionCases() {
    DoublyLinkedList<int> list;
    list.deleteNode(10); // Empty list: no-op.

    list.insertLast(10);
    list.deleteNode(10); // Only node.
    assert(list.isEmptyList() && list.validate());

    list.insertLast(10);
    list.insertLast(20);
    list.insertLast(30);
    list.insertLast(20); // Duplicate value.

    list.deleteNode(10); // Head.
    assert(list.front() == 20 && list.validate());
    list.deleteNode(30); // Middle.
    assert(list.length() == 2 && list.validate());
    list.deleteNode(20); // First matching duplicate (now head).
    assert(list.front() == 20 && list.back() == 20 && list.length() == 1);
    assert(list.validate());
    list.deleteNode(20); // Last remaining node.
    assert(list.isEmptyList() && list.validate());
    list.deleteNode(999); // Missing value: no-op.
    assert(list.isEmptyList() && list.validate());
}

void testCopyAndAssignment() {
    DoublyLinkedList<int> original;
    original.insertLast(1);
    original.insertLast(2);
    original.insertLast(3);

    DoublyLinkedList<int> copied(original);
    assert(copied == original);
    assert(copied.validate());

    copied.deleteNode(2);
    assert(copied.length() == 2);
    assert(original.length() == 3); // Deep copy, not shared nodes.
    assert(original.validate() && copied.validate());

    DoublyLinkedList<int> assigned;
    assigned.insertLast(100);
    assigned = original;
    assert(assigned == original && assigned.validate());

    assigned = assigned; // Self-assignment.
    assert(assigned == original && assigned.validate());

    DoublyLinkedList<int> emptyA;
    DoublyLinkedList<int> emptyB;
    assert(emptyA == emptyB); // Two empty lists are equal.
    emptyB.insertLast(7);
    assert(emptyA != emptyB); // Different lengths are not equal.
    emptyB.destroyList();

    assigned.destroyList();
    assert(assigned.isEmptyList() && assigned.validate());
    assigned = original; // Assignment into an empty list.
    assert(assigned == original && assigned.validate());

    DoublyLinkedList<int> moved(std::move(copied));
    assert(moved.validate() && moved.length() == 2);
    assert(copied.isEmptyList() && copied.validate());

    DoublyLinkedList<int> moveAssigned;
    moveAssigned = std::move(moved);
    assert(moveAssigned.length() == 2 && moveAssigned.validate());
    assert(moved.isEmptyList() && moved.validate());
}

void testTemplateTypesAndPolymorphism() {
    DoublyLinkedList<std::string> words;
    words.insertFirst("two");
    words.insertFirst("one");
    words.insertLast("three");
    assert(words.search("two"));
    assert(words.front() == "one" && words.back() == "three");
    assert(words.validate());

    // The virtual destructor must safely destroy a derived object through a base pointer.
    ListType<int>* polymorphicList = new DoublyLinkedList<int>();
    polymorphicList->insertLast(42);
    delete polymorphicList;
}

int main() {
    testEmptyAndEndpoints();
    testIndexedInsertionAndReplacement();
    testDeletionCases();
    testCopyAndAssignment();
    testTemplateTypesAndPolymorphism();

    std::cout << "All doubly linked list tests passed.\n";
    return 0;
}
