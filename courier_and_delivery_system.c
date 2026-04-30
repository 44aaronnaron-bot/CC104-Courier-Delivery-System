/* ==============================================================================
 * COURIER & DELIVERY MANAGEMENT SYSTEM
 * Data Structures: Doubly Linked List | Stack | Priority Queue
 * Algorithms: Linear Search | Bubble Sort
 * CC104 Final Project
 * ============================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 100

/* ========================== DATA STRUCTURES ========================== */

/* 1. DOUBLY LINKED LIST NODE - Main package registry */
typedef struct Package {
    int id;
    char dest[100];
    float weight;
    int priority;          /* 1=Express, 2=Standard, 3=Economy */
    struct Package *prev, *next;
} Package;

/* 2. STACK (Array-based) - Delivery history */
typedef struct {
    int ids[MAX_HISTORY];
    int top;
} HistoryStack;

/* 3. PRIORITY QUEUE (Linked) - Dispatch queue using same node structure */
Package *registry = NULL;    /* Doubly Linked List */
Package *dispatch = NULL;    /* Priority Queue */
HistoryStack history;        /* Stack */

/* ========================== HELPER FUNCTIONS ========================== */

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void initStack() {
    history.top = -1;
}

/* ========================== ALGORITHM 1: LINEAR SEARCH ========================== */
Package* findPackage(int id) {
    Package *curr = registry;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return NULL;
}

/* ========================== DOUBLY LINKED LIST OPERATIONS ========================== */

void registerPackage(int id, char *dest, float weight, int priority) {
    if (findPackage(id)) {
        printf("--> ERROR: ID %d already exists!\n", id);
        return;
    }
    
    Package *newPkg = (Package*)malloc(sizeof(Package));
    newPkg->id = id;
    strcpy(newPkg->dest, dest);
    newPkg->weight = weight;
    newPkg->priority = priority;
    newPkg->next = newPkg->prev = NULL;
    
    /* Insert at end of list */
    if (!registry) {
        registry = newPkg;
    } else {
        Package *temp = registry;
        while (temp->next) temp = temp->next;
        temp->next = newPkg;
        newPkg->prev = temp;
    }
    printf("--> Package %d registered!\n", id);
}

void deletePackage(int id) {
    Package *pkg = findPackage(id);
    if (!pkg) {
        printf("--> Package %d not found.\n", id);
        return;
    }
    
    /* Relink neighbors */
    if (pkg->prev) pkg->prev->next = pkg->next;
    else registry = pkg->next;
    
    if (pkg->next) pkg->next->prev = pkg->prev;
    
    free(pkg);
    printf("--> Package %d deleted.\n", id);
}

void displayRegistry() {
    if (!registry) {
        printf("--> Registry is empty.\n");
        return;
    }
    
    Package *curr = registry;
    printf("\n%-8s | %-20s | %-8s | %-10s\n", "ID", "Destination", "Weight", "Priority");
    printf("------------------------------------------------------------\n");
    while (curr) {
        printf("%-8d | %-20s | %-8.2f | %-10d\n", 
               curr->id, curr->dest, curr->weight, curr->priority);
        curr = curr->next;
    }
}

/* ========================== ALGORITHM 2: BUBBLE SORT ========================== */
void sortRegistry() {
    if (!registry || !registry->next) {
        printf("--> Not enough packages to sort.\n");
        return;
    }
    
    int swapped;
    Package *curr;
    Package *last = NULL;
    
    do {
        swapped = 0;
        curr = registry;
        
        while (curr->next != last) {
            if (curr->id > curr->next->id) {
                /* Swap data only (simpler than relinking) */
                int tempId = curr->id;
                char tempDest[100];
                float tempWeight = curr->weight;
                int tempPrio = curr->priority;
                
                strcpy(tempDest, curr->dest);
                
                curr->id = curr->next->id;
                strcpy(curr->dest, curr->next->dest);
                curr->weight = curr->next->weight;
                curr->priority = curr->next->priority;
                
                curr->next->id = tempId;
                strcpy(curr->next->dest, tempDest);
                curr->next->weight = tempWeight;
                curr->next->priority = tempPrio;
                
                swapped = 1;
            }
            curr = curr->next;
        }
        last = curr;
    } while (swapped);
    
    printf("--> Registry sorted by ID!\n");
}

/* ========================== PRIORITY QUEUE OPERATIONS ========================== */

void enqueuePackage(Package *pkg) {
    /* Prevent duplicate queuing */
    Package *curr = dispatch;
    while (curr) {
        if (curr->id == pkg->id) {
            printf("--> Package %d already in queue!\n", pkg->id);
            return;
        }
        curr = curr->next;
    }
    
    Package *newPkg = (Package*)malloc(sizeof(Package));
    *newPkg = *pkg;  /* Copy data */
    newPkg->next = newPkg->prev = NULL;
    
    /* Priority insertion (1 is highest = goes to front) */
    if (!dispatch || dispatch->priority > newPkg->priority) {
        newPkg->next = dispatch;
        if (dispatch) dispatch->prev = newPkg;
        dispatch = newPkg;
    } else {
        curr = dispatch;
        while (curr->next && curr->next->priority <= newPkg->priority) {
            curr = curr->next;
        }
        newPkg->next = curr->next;
        if (curr->next) curr->next->prev = newPkg;
        curr->next = newPkg;
        newPkg->prev = curr;
    }
    printf("--> Package %d queued (Priority: %d)\n", pkg->id, pkg->priority);
}

void processDelivery() {
    if (!dispatch) {
        printf("--> No packages in dispatch queue.\n");
        return;
    }
    
    Package *delivered = dispatch;
    dispatch = dispatch->next;
    if (dispatch) dispatch->prev = NULL;
    
    printf("\n[DELIVERING] Package %d to %s...\n", delivered->id, delivered->dest);
    
    /* Push to history stack */
    if (history.top < MAX_HISTORY - 1) {
        history.ids[++history.top] = delivered->id;
    }
    
    /* Remove from registry (without double free) */
    if (delivered->prev) delivered->prev->next = delivered->next;
    if (delivered->next) delivered->next->prev = delivered->prev;
    if (registry == delivered) registry = delivered->next;
    
    free(delivered);
    printf("--> Delivery complete! Removed from system.\n");
}

/* ========================== STACK OPERATIONS (HISTORY) ========================== */

void viewHistory() {
    if (history.top == -1) {
        printf("--> No deliveries yet.\n");
        return;
    }
    
    printf("\n=== RECENT DELIVERIES (Latest First) ===\n");
    for (int i = history.top; i >= 0; i--) {
        printf("  Delivered: Package #%d\n", history.ids[i]);
    }
}

/* ========================== MEMORY CLEANUP ========================== */

void freeMemory() {
    Package *temp;
    while (registry) {
        temp = registry;
        registry = registry->next;
        free(temp);
    }
    while (dispatch) {
        temp = dispatch;
        dispatch = dispatch->next;
        free(temp);
    }
    printf("--> Memory freed.\n");
}

/* ========================== MAIN MENU ========================== */

int main() {
    int choice, id, priority;
    float weight;
    char dest[100];
    
    initStack();
    
    do {
        printf("\n=========================================\n");
        printf("  COURIER & DELIVERY MANAGEMENT SYSTEM   \n");
        printf("=========================================\n");
        printf("1. Register Package\n");
        printf("2. View All Packages\n");
        printf("3. Search Package\n");
        printf("4. Queue Package for Delivery\n");
        printf("5. Process Next Delivery\n");
        printf("6. View Delivery History\n");
        printf("7. Sort Registry by ID\n");
        printf("8. Delete Package\n");
        printf("0. Exit\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            printf("--> Invalid input.\n");
            continue;
        }
        
        switch(choice) {
            case 1:
                printf("ID: "); scanf("%d", &id);
                printf("Destination: "); clearBuffer(); fgets(dest, 100, stdin);
                dest[strcspn(dest, "\n")] = 0;
                printf("Weight (kg): "); scanf("%f", &weight);
                printf("Priority (1=Express,2=Standard,3=Economy): "); scanf("%d", &priority);
                registerPackage(id, dest, weight, priority);
                break;
                
            case 2:
                displayRegistry();
                break;
                
            case 3:
                printf("Enter ID to search: "); scanf("%d", &id);
                Package *found = findPackage(id);
                if (found) {
                    printf("\n--- PACKAGE FOUND ---\n");
                    printf("ID: %d\nDestination: %s\nWeight: %.2f kg\nPriority: %d\n",
                           found->id, found->dest, found->weight, found->priority);
                } else {
                    printf("--> Package %d not found.\n", id);
                }
                break;
                
            case 4:
                printf("Enter ID to queue: "); scanf("%d", &id);
                Package *toQueue = findPackage(id);
                if (toQueue) enqueuePackage(toQueue);
                else printf("--> Package %d not in registry.\n", id);
                break;
                
            case 5:
                processDelivery();
                break;
                
            case 6:
                viewHistory();
                break;
                
            case 7:
                sortRegistry();
                displayRegistry();
                break;
                
            case 8:
                printf("Enter ID to delete: "); scanf("%d", &id);
                deletePackage(id);
                break;
                
            case 0:
                printf("Shutting down...\n");
                freeMemory();
                break;
                
            default:
                printf("--> Invalid choice.\n");
        }
    } while (choice != 0);
    
    return 0;
}