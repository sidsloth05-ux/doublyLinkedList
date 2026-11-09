# Doubly Linked List (C++)

A templated doubly linked list implemented from scratch in C++. Each node stores a value and pointers to both its previous and next nodes. The list maintains `head`, `tail`, and `count`, allowing constant-time insertion and removal at either endpoint.

## Features

- Templated implementation (`DoublyLinkedList<T>`), tested with `int` and `std::string`
- Forward and reverse traversal
- Insert at the front, back, or a specified index
- Replace an element by index and retrieve the replaced value
- Delete the first node matching a value
- Search, endpoint access, list comparison, and clearing
- Deep-copy construction and copy assignment
- Move construction and move assignment
- Structural validation for checking forward/backward links
- Automated tests for empty lists, singleton lists, endpoints, invalid indices, duplicates, copying, and polymorphic destruction

## Project files

| File | Description |
|---|---|
| `ListType.h` | Abstract base class, node definition, shared ownership and copy/move behavior |
| `DoublyLinkedList.h` | Templated doubly linked list implementation |
| `main.cpp` | Automated test driver |

## Build and run

Requires a C++17-compatible compiler.

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o dll_tests
./dll_tests
```

A successful run prints:

```text
All doubly linked list tests passed.
```

For additional memory and undefined-behavior checks with GCC or Clang:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer -g \
    main.cpp -o dll_tests
./dll_tests
```

## Operation complexity

Let `n` be the number of elements and `i` be a valid zero-based index.

| Operation | Time complexity | Notes |
|---|---:|---|
| `insertFirst` | O(1) | Updates the head and at most one neighboring link |
| `insertLast` | O(1) | Updates the tail and at most one neighboring link |
| `front`, `back`, `length`, `isEmptyList` | O(1) | Direct access to stored state |
| `search` | O(n) | May inspect every node |
| `deleteNode(value)` | O(n) | Finds the first matching value, then unlinks it in O(1) |
| `insertNode(value, i)` | O(min(i, n - i)) | Traverses from whichever endpoint is closer |
| `replace(value, i)` | O(min(i, n - 1 - i)) | Traverses from whichever endpoint is closer |
| Copy construction / assignment | O(n) | Creates an independent chain of nodes |
| Equality comparison | O(n) | Compares values in order |
| `destroyList` | O(n) | Deletes every node |

The indexed operations remain O(n) in the worst case; choosing the nearer endpoint reduces the number of pointer hops on average.

## API behavior and edge cases

- Indices are zero-based.
- `insertNode(value, index)` accepts indices from `0` through `length()` inclusive. Invalid indices throw `std::out_of_range`.
- `replace(value, index)` accepts indices from `0` through `length() - 1`. Invalid indices throw `std::out_of_range`.
- `front()` and `back()` throw `std::out_of_range` when the list is empty.
- `deleteNode(value)` removes the first matching node. If the value is absent, the list is unchanged.
- Deleting from an empty list is a no-op.
- Two empty lists compare equal.
- Copies own independent nodes; changing or destroying one list does not affect another.

## Design notes

`ListType` owns the node chain and provides the destructor, deep-copy behavior, move operations, and common list operations. Its destructor is virtual so a derived list can safely be destroyed through a base-class pointer. `DoublyLinkedList` is responsible for maintaining both directions of every link when nodes are inserted or removed.

`validate()` checks the node count, endpoint pointers, and consistency of `next` and `prev` links in both traversal directions. It is useful in tests and debugging; it is O(n).
