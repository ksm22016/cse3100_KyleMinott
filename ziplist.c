//Do not modify starter code
//You may add helper functions if needed
#include <stdio.h>
#include <stdlib.h>

// Node structure
typedef struct Node {
  int data;
  struct Node *next;
} Node;

// Create a new node
Node *createNode(int data) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

// Insert at end
void insertEnd(Node **head, int data) {
  Node *newNode = createNode(data);
  if (!*head) {
    *head = newNode;
    return;
  }
  Node *temp = *head;
  while (temp->next)
    temp = temp->next;
  temp->next = newNode;
}

// Print list
void printList(Node *head) {
  while (head) {
    printf("%d ", head->data);
    head = head->next;
  }
  printf("\n");
}


void zipList(Node **headRef) {

  // fill code here
    if (headRef == NULL || *headRef == NULL || (*headRef)->next == NULL) return;

    Node *head = *headRef;

    Node *right = head;
    Node *left = head;
    while (left->next != NULL && left->next->next != NULL) {
        right = right->next;
        left = left->next->next;
    }
    
    Node *second = right->next;
    right->next = NULL;

    Node *prev = NULL;
    Node *cur = second;
    while (cur != NULL) {
        Node *nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    second = prev;

    Node *first = head;
    while (second != NULL) {
        Node *fnext = first->next;
        Node *snext = second->next;

        first->next = second;
        second->next = fnext;

        first = fnext;
        second = snext;
    }
    *headRef = head;
}

int main(int argc, char *argv[]) {
  Node *head = NULL;

  if (argc < 2) {
    printf("Usage: %s <list of integers>\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    int val = atoi(argv[i]);
    insertEnd(&head, val);
  }

  printf("Original list:\n");
  printList(head);

  zipList(&head);

  printf("Zipped list:\n");
  printList(head);
 
	// fill code here
  free(head);
  return 0;
}