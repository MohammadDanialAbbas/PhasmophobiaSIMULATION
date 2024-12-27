#include "defs.h"

//leaveEvidence adds a random piece of evidence in a room and logs it
//The parameters it takes in GhostType* ghost and use it to find it's rooms evidencelist 
void leaveEvidence(GhostType* ghost) {
    sem_wait(&ghost->room->evidenceList->semaphore);
    EvidenceType evidence = randomEvidenceType(ghost->class);

    // add evidence to the rooms list of evidence
    addEvidence(ghost->room->evidenceList, evidence);

    // log
    l_ghostEvidence(evidence, ghost->room->name);
    sem_post(&ghost->room->evidenceList->semaphore);

}

//ghostMove chooses a room to go to and has the ghost be inserted to a connected room and removed from it's previous room 
//the only parameter is the GhostType to move
void ghostMove(GhostType* ghost) {
    RoomType* oldRoom = ghost->room;
    sem_wait(&oldRoom->semaphore);
    RoomNodeType* current = ghost->room->rooms.head;

    // traverse the linked list of rooms and stop randomly
    while (current != NULL) {
        if (randInt(0, 101) < 20) {
            break;
        }
        // stop at the last node
        if (current->next == NULL) {
            break;
        }
        current = current->next;
    }

    RoomType* newRoom = current->data;

    // add the ghost to the new room and remove it from the previous room
    sem_wait(&newRoom->semaphore);
    ghostAddToRoom(newRoom, ghost);
    ghostRemoveFromRoom(oldRoom);
    sem_post(&oldRoom->semaphore);
    // log
    l_ghostMove(newRoom->name);
    sem_post(&newRoom->semaphore);
}

//hunterMove chooses a room to go to and has the hunter be inserted to a connected room and removed from it's previous room 
//the only parameter is the HunterType to move
void hunterMove(HunterType* hunter) {

    RoomType* oldRoom = hunter->room;
    sem_wait(&oldRoom->semaphore);
    RoomNodeType* current = hunter->room->rooms.head;

    // traverse the linked list of rooms and stop randomly
    while (current != NULL) {
        if (randInt(0, 101) < 20) {
            break;
        }
        if (current->next == NULL) {
            break;
        }
        current = current->next;
    }

    RoomType* newRoom = current->data;

    // add the hunter to the new room and remove it from the previous room
    sem_wait(&newRoom->semaphore);
    hunterAddToRoom(newRoom, hunter);
    hunterRemoveFromRoom(oldRoom, hunter);
    sem_post(&oldRoom->semaphore);

    // log
    l_hunterMove(hunter->name, newRoom->name);
    sem_post(&newRoom->semaphore);

}

//hunterCollect has a hunter take a piece of evidence and add it to a Evidence masterlist and remove it from the room 
// the only parameter is the hunter that is collecting the evidence
void hunterCollect(HunterType* hunter) {
    sem_wait(&hunter->room->evidenceList->semaphore);
    sem_wait(&hunter->collectedEvidence->semaphore);
    EvidenceNodeType* current = hunter->room->evidenceList->head;
    EvidenceType evidence;

    if (current == NULL) {
        sem_post(&hunter->room->evidenceList->semaphore);
        sem_post(&hunter->collectedEvidence->semaphore);
        return;
    }


    int found = 0;


    // traverse the evidence list in the hunters room until we find a piece of evidence that matches our type
    while (current != NULL) {
        // we have found matching evidence types
        if (current->evidence == hunter->type) {
            evidence = current->evidence;
            found = 1;
            break;
        }


        current = current->next;
    }


    // matching evidence type is found, we should collect it
    if (found == 1) {


        current = hunter->collectedEvidence->head;
        // traverse the list of evidence collected
        while (current != NULL) {
            // if the hunters already have the evidence that is being collected, we don't collect the evidence
            if (current->evidence == evidence) {
                sem_post(&hunter->room->evidenceList->semaphore);
                sem_post(&hunter->collectedEvidence->semaphore);
                return;
            }


            current = current->next;
        }
        
        // log
        l_hunterCollect(hunter->name, hunter->type, hunter->room->name);

        // add evidence to the hunters list of evidence
        addEvidence(hunter->collectedEvidence, evidence);

        // remove evidence from the rooms list of evidence
        removeEvidence(hunter->room->evidenceList, evidence);
        
    }
    sem_post(&hunter->room->evidenceList->semaphore);
    sem_post(&hunter->collectedEvidence->semaphore);
}

//reviewEvidence reviews the list of evidence and returns true if it has enough to capture the ghost and false otherwise
//the parameter it takes is a HunterType* hunter to get the hunter's name and to check the collectedEvidence 
int reviewEvidence(HunterType* hunter) {
    sem_wait(&hunter->collectedEvidence->semaphore);
    EvidenceNodeType* current = hunter->collectedEvidence->head;
    sem_post(&hunter->collectedEvidence->semaphore);

    // we dont have any evidence
    if (current == NULL) {
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
        return 0;
    }

    int counter = 0;
    // traverse the evidence list and count how much evidence we have
    while (current != NULL) {
        counter++;
        current = current->next;
    }


    //  return true if we have enough evidence to capture the ghost
    if (counter >= 3) {
        l_hunterReview(hunter->name, LOG_SUFFICIENT);
        return C_TRUE;
    }

    // otherwise, return false
    else {
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
        return C_FALSE;
    }
}

//ghostMain is the used for the ghost thread, loops 3 different posibilities and ends if ending conditions are fufilled
//the parameters it takes is a void* ptr to be reconverted into a GhostType and use it to allow the ghost to perform actions
void* ghostMain(void* ptr){

    GhostType *ghost; 
    ghost = (GhostType*) ptr;
    while(1) {
        //printf("Ghost %d\n",ghost->boredom);
        // ghost bored, exit thread
        if (ghost->boredom >= BOREDOM_MAX){
            l_ghostExit(LOG_BORED);
            sem_wait(&ghost->room->semaphore);
            ghostRemoveFromRoom(ghost->room);
            sem_post(&ghost->room->semaphore);

            pthread_exit(ghost);
            break;
        }
        
        //if the ghost is in the room with a hunter, ghost isnt bored and may leave evidence
        else if (ghost->room->numOfHunters>0){
            ghost->boredom = 0;
            int randomNumber = randInt(0,10);
            if (randomNumber >=4){    
                leaveEvidence(ghost);
            }
        }
        //if the ghost is alone, boredom increases, may move or leave evidence or do nothing
        else {
            ghost->boredom++;
            int randomNumber = randInt(0,3);
            if (randomNumber==0){
                ghostMove(ghost);
            }else if (randomNumber == 1){
                leaveEvidence(ghost);
            }
        }
        
        usleep(100000);
    }
    
}


//hunterMain is the used for the hunter thread, loops 3 different posibilities and ends if ending conditions are fufilled
//the parameters it takes is a void* ptr to be reconverted into a HunterType and use it to allow the hunter to perform actions
void* hunterMain(void* ptr){
    HunterType* hunter;
    hunter = (HunterType*) ptr;
    while (1) {
        //printf("hunter %s %d\n",hunter->name,hunter->boredom);
        //if hunter is too scared or bored, thread exit
        if (hunter->boredom >= BOREDOM_MAX){
            l_hunterExit(hunter->name, LOG_BORED);
            sem_wait(&hunter->room->semaphore);
            hunterRemoveFromRoom(hunter->room, hunter);
            sem_post(&hunter->room->semaphore);
            pthread_exit(hunter);

        }else if(hunter->fear >= FEAR_MAX){
            l_hunterExit(hunter->name, LOG_FEAR);
            sem_wait(&hunter->room->semaphore);
            hunterRemoveFromRoom(hunter->room, hunter);
            sem_post(&hunter->room->semaphore);
            pthread_exit(hunter);
        }

        //if ghost is in the room then hunter is scared and boredom resets
        if (hunter->room->ghost!=NULL){
            hunter->boredom = 0;
            hunter->fear++;


        // if ghost isnt in room hunter is bored
        }else{
            hunter->boredom++;
        }


        //hunter randomly decides to pick an action to do
        int randomNumber = randInt(0,101);

        //move to different room
        if (randomNumber > 60){
            hunterMove(hunter);


        //pick up evidence
        }else if (randomNumber >= 30){
            hunterCollect(hunter);


        //review evidence  
        } else{
            if(reviewEvidence(hunter)==1){
                sem_wait(&hunter->room->semaphore);
                hunterRemoveFromRoom(hunter->room, hunter);
                sem_post(&hunter->room->semaphore);
                pthread_exit(hunter);

            }
        }

        usleep(100000);
    }
}
