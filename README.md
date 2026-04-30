# CC104-Courier-Delivery-System
Final Project for CC104: Data Structure and Algorithms

## Overview
This is a console-based application developed in C for the CC104 Final Project. It simulates a backend management system for a courier service, allowing users to register packages, queue them based on shipping priority, process deliveries, and maintain a history of completed jobs.

## Data Structures & Algorithms Used
This project implements the following core concepts:

**Data Structures:**
* **Doubly Linked List:** Used as the main package registry to allow dynamic memory allocation and easy traversal of all active packages.
* **Priority Queue:** Used for the dispatch system to ensure Express packages are delivered before Standard or Economy packages.
* **Stack (Array-based):** Used to maintain a delivery history, allowing the system to view the most recently delivered packages first (LIFO).

**Algorithms:**
* **Linear Search:** Used to search the registry by Package ID to find specific shipments and prevent duplicate ID entries.
* **Bubble Sort:** Implemented to sort the entire package registry in ascending order based on Package ID for organized viewing.

## How to Compile and Run
To run this application, you need a standard C compiler (like GCC) installed on your system.

1. Clone this repository or download the `courier_and_delivery_management_system.c` file.
2. Open your terminal or command prompt and navigate to the folder containing the file.
3. Compile the code using the following command:
   ```bash
   gcc courier_and_delivery_management_system.c -o courier_app
