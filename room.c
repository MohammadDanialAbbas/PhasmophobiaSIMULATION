#include "defs.h"

//connectRooms creates a link between two rooms through updating each rooms' linked list of connected rooms
//The parameters connectRooms takes in are two different RoomType* rooms to add each to each other's list
void connectRooms(RoomType* room1, RoomType* room2) {
    RoomNodeType* newNode1 = (RoomNodeType*) malloc(sizeof(RoomNodeType));
    newNode1->data = room2;
    newNode1->next = NULL; 

    // if the room isn't connected to any other RoomTypes
    if (room1->rooms.head == NULL) {
        // the room will point to the room1 parameter
        room1->rooms.head = newNode1;
        room1->rooms.tail = newNode1;
    }

    // room already points to 1 RoomType (which possibly points to another)
    else {
        room1->rooms.tail->next = newNode1;
        room1->rooms.tail = newNode1;
    }

    
    // room1 should also be in the room2 linkedlist of rooms
    RoomNodeType* newNode2 = (RoomNodeType*) malloc(sizeof(RoomNodeType));
    newNode2->data = room1;
    newNode2->next = NULL;

    // if room2 isn't conncted to any other rooms
    if (room2->rooms.head == NULL) {
        // the room will point to the room1 parameter
        room2->rooms.head = newNode2;
        room2->rooms.tail = newNode2;
    }

    // room2 alraedy has atleast 1 room it is conncted to
    else {
        room2->rooms.tail->next = newNode2;
        room2->rooms.tail = newNode2;
    }
    
}

//addRooms takes a specific room and adds it to a list of rooms
//the parameters addRoom takes is a RoomListType* list and a RoomType* room to add a room to a list
void addRoom(RoomListType *list, RoomType *room) {
    RoomNodeType* newNode = (RoomNodeType*) malloc(sizeof(RoomNodeType));
    newNode->data = room;
    newNode->next = NULL; 

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    }

    else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

//hunterAddToRoom inserts a specific Hunter into a room, increases the number of hunters into that room 
//the parameters hunterAddToRoom takes is a RoomType* room and a HunterType* hunter to add a hunter to a room
void hunterAddToRoom(RoomType* room, HunterType* hunter) {
    // set the room of the hunter to the room we are adding it to
    hunter->room = room;

    room->hunters[room->numOfHunters] = hunter;

    room->numOfHunters++;

}

//hunterRemoveFromRoom removes a specific Hunter from a room, decreases the number of hunters in that room 
//the parameters hunterRemoveFromRoom takes is a RoomType* room and a HunterType* hunter to remove a hunter from a room
void hunterRemoveFromRoom(RoomType* room, HunterType* hunter) {
    
    for (int i = 0; i < room->numOfHunters - 1; i++) {
        // find the index at which the hunter is in in the hunter array in the room
        if (strcmp(room->hunters[i]->name, hunter->name) == 0 && room->hunters[i]->name != NULL) {
            
            
            // shift all the hunters back by 1
            while (i < NUM_HUNTERS - 1) {
                room->hunters[i] = room->hunters[i + 1];
                i++;
            }
            
            room->hunters[room->numOfHunters - 1] = NULL;
            break;
        }
    }
    

    room->numOfHunters--;
}

//ghostAddToRoom inserts the ghost into a room 
//the parameters hunterAddToRoom takes is a RoomType* room and a GhostType* ghost to add the ghost to a room
void ghostAddToRoom(RoomType* room, GhostType* ghost) {
    room->ghost = ghost;
    ghost->room = room;
}

//ghostRemoveFromRoom removes the ghost from a room
//the parameters ghostRemoveFromRoom takes is a RoomType* room and a GhostType* ghost to remove the ghost from a room
void ghostRemoveFromRoom(RoomType* room) {
    room->ghost = NULL;
}

// this function frees all data within a roomlist which includes nodes, rooms, evidence lists, etc
// the only parameter is the roomlist to free
void cleanupRooms(RoomListType* roomList) {
    RoomNodeType* current = roomList->head;
    RoomNodeType* next = NULL;

    while (current != NULL) {
        next = current->next;

        // clean up each rooms evidence list
        cleanupEvidence(current->data->evidenceList);
        free(current->data->evidenceList);

        // clean up each rooms list of connected rooms
        cleanupConnectedRooms(&current->data->rooms);

        // free the room
        free(current->data);

        // then free the node
        free(current);

        current = next;
    }

    roomList->head = NULL;
    roomList->tail = NULL;
}

// this function frees all the nodes for a given roomlist
// the only parameter is the roomlisttype to free
void cleanupConnectedRooms(RoomListType* connectedRooms) {
    RoomNodeType* current = connectedRooms->head;
    RoomNodeType* next = NULL;

    // traverse the linked list
    while (current != NULL) {
        next = current->next;

        free(current);

        current = next;
    }

    connectedRooms->head = NULL;
    connectedRooms->tail = NULL;
}
