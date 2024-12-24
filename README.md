# Memory Management Analysis Library Report

## Project Overview
The purpose of this library is to serve as an aid for memory management analysis in modular C programs. By diverting standard memory management functions (`malloc`, `calloc`, `realloc`, `free`) to custom clones, the library collects runtime data, checks for anomalies, and generates detailed reports without altering the target program's behavior.

This tool acts as an alternative to **Valgrind**, offering more precise control by working at the compile-time level rather than post-compilation analysis.

---

## Features Implemented
1. **Memory Allocation Management**
   - Tracks and analyzes `malloc`, `calloc`, and `realloc` operations.
   - Ensures no anomalies occur during dynamic memory allocation.

2. **Memory Release Verification**
   - Monitors the `free` function.
   - Detects issues such as:
     - Double-free errors.
     - Illegal memory deallocations.

3. **Dynamic Array Operations**
   - **Extend**: Expands the internal array of cells when capacity is reached.
   - **Append**: Adds new elements to the internal array dynamically.

4. **Custom Memory Operations**
   - **Custom Allocators**: Overridden implementations of `malloc`, `realloc`, `calloc`, and `free` for enhanced tracking and safety.

5. **Reporting Mechanism**
   - Outputs statistics including:
     - Number of allocation/release calls.
     - Total memory allocated and freed.
     - Anomalies detected and their locations.

---

## Components Implemented
### Source Files
1. **`env.c`**
   - Environment setup and configuration for the memory tracking library.

2. **`cell.c`**
   - Core structure for managing memory cells.
   - Supports operations for individual cell manipulation and tracking.

3. **`extend.c`**
   - Implements dynamic resizing of the internal array.
   - Ensures efficient memory usage while avoiding over-allocation.

4. **`append.c`**
   - Adds new elements to the internal array.
   - Includes bounds checking and resizing logic.

5. **`malloc.c`**
   - Custom implementation of the `malloc` function.
   - Tracks memory allocation and logs metadata for reporting.

6. **`realloc.c`**
   - Overrides the standard `realloc`.
   - Ensures safe reallocation and validates source addresses.

7. **`free.c`**
   - Monitors and validates memory release operations.
   - Prevents double-free and dangling pointer issues.

8. **`calloc.c`**
   - Custom implementation of `calloc`.
   - Tracks zero-initialized allocations.

### Internal headers
1. **`env.h`**
    - Contains environment definition to track all the cells of allocated memory & global metrics.

2. **`cell.h`**
    - Contains cell definition to track single memory allocation unit & its state.

### Global headers
1. **`memtrack.h`**
    - to be included manually if library is installed globally or as dependency during the compilation otherwise

---

## Statistics and Reporting
### Key Metrics
- **Total Allocations**: Count of memory allocations performed.
- **Total Deallocations**: Count of successful memory releases.
- **Anomalies Detected**: Summary of errors, such as:
  - Double-free incidents.
  - Unreleased memory blocks.
  - Zero memory allocation warnings.
  - Null frees warnings.

### Example Output
```plaintext
=== activate tracker ===
[Call #001] file<tests/test_C.c> | function<main> | line<6> | malloc(30) -> 0x56303b423430
[Call #002] file<tests/test_C.c> | function<main> | line<7> | malloc(10) -> 0x56303b423460
[Call #003] file<tests/test_C.c> | function<main> | line<8> | realloc(0x56303b423430, 40) -> 0x56303b423430
[Call #004] file<tests/test_C.c> | function<main> | line<9> | free(0x56303b423430)
=== result tracker ===
Missing free(0x56303b423460) -> 10b lost
Total bytes allocated: 80b
Total bytes released: 70b
Total bytes forgotten: 10b

=== activate tracker ===
[Call #001] file<tests/test_D.c> | function<main> | line<6> | malloc(30) -> 0x5650e9d2a430
[Call #002] file<tests/test_D.c> | function<main> | line<7> | malloc(10) -> 0x5650e9d2a460
[Call #003] file<tests/test_D.c> | function<main> | line<8> | realloc(0x5650e9d2a430, 41) -> 0x5650e9d2a480
[Call #004] file<tests/test_D.c> | function<main> | line<9> | free(0x5650e9d2a430) | ERROR: double memory free
=== result tracker ===
Missing free(0x5650e9d2a460) -> 10b lost
Missing free(0x5650e9d2a480) -> 41b lost
Total bytes allocated: 81b
Total bytes released: 30b
Total bytes forgotten: 51b
```