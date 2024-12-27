#include "defs.h"

//InitEvidenceList initializes an evidence list
//the parameters initEvidenceList takes is an EvidenceListType* Evidence List to initialize it
void initEvidenceList(EvidenceListType *evList) {
    evList->head = NULL;
    evList->tail = NULL;
    sem_init(&evList->semaphore,0,1);

}

//InitRoomList initializes an room list
//the parameters initRoomList takes is an RoomListType* room List to initialize it
void initRoomList(RoomListType *roomList) {
    roomList->head = NULL;
    roomList->tail = NULL;

}

//InitHouse initializes a house, it's evidencelist and it's roomlist
//the parameters initRoomList takes is an HouseType house to initialize it and it's RoomList
void initHouse(HouseType *house, EvidenceListType* evidenceList) {
    house->collectedEvidence = evidenceList;
    initRoomList(&house->rooms);
}

//createRoom creates a room gives it a name and calls to initialize it's roomlist of connected rooms and its evidencelist and returns the room
//the parameters createRoom takes is an char* name to initalize a room and give it a name
RoomType* createRoom(char *name) {
    RoomType* room = (RoomType*) malloc(sizeof(RoomType));

    strcpy((room->name), name);
    room->ghost = NULL;
    room->numOfHunters = 0;

    initRoomList(&room->rooms);

    EvidenceListType* evidenceList = (EvidenceListType*) malloc(sizeof(EvidenceListType));
    initEvidenceList(evidenceList);
    room->evidenceList = evidenceList;
    sem_init(&room->semaphore,0,1);

    return room;
}

//initHunter initializes a Hunter gives it a name and calls to initialize its evidencelist and gives it a specific evidence it can read 
//the parameters initHunter takes is an char* name to give the hunter a name, an EvidenceListType, RoomType, HunterType and EvidenceType to initialize the hunter
void initHunter(char *name, EvidenceType evType, EvidenceListType *evList, RoomType *room, HunterType *hunter) {
    strcpy(hunter->name, name);
    hunter->type = evType;
    hunter->collectedEvidence = evList;
    hunter->room = room;

    hunter->boredom = 0;
    hunter->fear = 0;

    hunterAddToRoom(room, hunter);
    l_hunterInit(hunter->name, hunter->type);
}

//initGhost initalizes a Ghost, gives it a ghost class and calls to initialize its roomList
//the parameters initGhist takes is an GhostClass class to give the ghost a class, RoomListType and GhostType to initialize the hunter
void initGhost(GhostClass class, GhostType *ghost, RoomListType *roomList) {
    ghost->boredom = 0;
    ghost->class = class;
    
    // start at the room after the van
    RoomNodeType* current = roomList->head->next;

    // traverse the room list and stop randomly
    while (current != NULL) {
        if (randInt(0, 101) < 50) {
            break;
        }

        // stop at the last node so current doesn't get set to NULL
        if (current->next == NULL) {
            break;
        }

        current = current->next;
    }

    ghost->room = current->data;

    l_ghostInit(ghost->class, ghost->room->name);
}