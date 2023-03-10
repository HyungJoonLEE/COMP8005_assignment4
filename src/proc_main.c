#include "linked_list.h"

int main() {
    LinkedList *proc_list = NULL;

    proc_list = createLinkedList();
    save_processes(proc_list);
    process_directory_processing(proc_list);

    displayLinkedList(proc_list);




    exit(EXIT_SUCCESS);
}

