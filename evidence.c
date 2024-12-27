#include "defs.h"

//addEvidence takes a piece of evidence and adds it to a Evidence linked-list
//the parameters addEvidence takes is a EvidenceListType* Evidence List to have it's other parameter an EvidenceType evidence be inserted into it 
void addEvidence(EvidenceListType* evList, EvidenceType evidence) {
    EvidenceNodeType* newNode = (EvidenceNodeType*) malloc(sizeof(EvidenceNodeType));
    newNode->evidence = evidence;
    newNode->next = NULL;

    // if the linked list is empty
    // make the new node the head and the tail of the evidencelist
    if (evList->head == NULL) {
        evList->head = newNode;
        evList->tail = newNode;
    }

    // otherwise add the new to the end of the linked list
    else {
        evList->tail->next = newNode;
        evList->tail = newNode;
    }
    
}

//removeEvidence takes a piece of evidence and removes it to a Evidence linked-list
//the parameters removeEvidence takes is a EvidenceListType* Evidence List to have it's other parameter an EvidenceType evidence be removed from it 
void removeEvidence(EvidenceListType* evList, EvidenceType evidence) {
    EvidenceNodeType* current = evList->head;
    EvidenceNodeType* prev = NULL;

    // traverse the evidence linked list until we find a matching evidencetype
    while (current != NULL) { 
        if (current->evidence == evidence) {
            // evidence we are removing is the first node
            if (prev == NULL) {
                evList->head = current->next;
            }

            // evidence we are removing is the last node
            else if (current == evList->tail) {
                evList->tail = prev;
                prev->next = NULL;
            }

            // evidence we are removing is in the middle (between first and last)
            else {
                prev->next = current->next;
            }

            free(current);

            break;
        }

        prev = current;
        current = current->next;
    }
}

// this function cleans up the provided evidence list
// the only parameter is the evidence list to free memory for
void cleanupEvidence(EvidenceListType* evList) {
    EvidenceNodeType* current = evList->head;
    EvidenceNodeType* next = NULL;


    // traverse the evidence list and free each node
    while (current != NULL) {
        
        // make sure to save the next node
        next = current->next;
        free(current);

        current = next;
    }


    evList->head = NULL;
    evList->tail = NULL;
}
