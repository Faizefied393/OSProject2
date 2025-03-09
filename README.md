# CYB-3053 Project 2 - Memory Allocator

## Overview
This project implements a custom memory allocator in C that provides `malloc()`, `calloc()`, `realloc()`, and `free()`. It manages heap memory using a **free list** and allows different allocation strategies (**First Fit, Best Fit, Worst Fit, Next Fit**).

## Features
- Custom memory allocation (`tumalloc`, `tucalloc`, `turealloc`, `tufree`)
- Implements multiple allocation strategies:
  - **First Fit** (default)
  - **Best Fit** (minimizes fragmentation)
  - **Worst Fit** (preserves larger blocks)
  - **Next Fit** (efficient for sequential allocations)
- Uses `sbrk()` to request memory from the OS
- Splitting and coalescing for memory efficiency

## Project Structure
