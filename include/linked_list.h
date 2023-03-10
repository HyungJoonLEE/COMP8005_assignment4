#ifndef PROJECT_LINKEDLIST_H
#define PROJECT_LINKEDLIST_H
#include "common.h"

typedef struct ListNodeType {
    int pid;
    int ppid;
    char comm[256];
    char fd;
    char cmdline[512];
    struct ListNodeType *pLink;
    struct LinkedListType *pLinkFD;
} ListNode;


typedef struct LinkedListType {
  int currentElementCount;
  ListNode headerNode;
} LinkedList;

LinkedList *createLinkedList(void);
int addLLElement(LinkedList *pList, int position, ListNode element);
int removeLLElement(LinkedList *pList, int position);
ListNode *getLLElement(LinkedList *pList, int position);

void clearLinkedList(LinkedList *pList);
int getLinkedListLength(LinkedList *pList);
void deleteLinkedList(LinkedList *pList);
void displayLinkedList(LinkedList *pList);

void save_processes(LinkedList* proc_list);
void save_pid_to_linked_list(LinkedList *proc_list, char *pid_str, int i, int pid);
void process_directory_processing(LinkedList *proc_list);
void save_ppid_to_linked_list(char *path, LinkedList *proc_list, int index);
void save_comm_to_linked_list(char *path, LinkedList *proc_list, int index);
void save_cmdline_to_linked_list(char *path, LinkedList *proc_list, int index);



#define FALSE 0
#define TRUE 1

#endif // PROJECT_LINKEDLIST_H
