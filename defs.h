#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef struct Room RoomType;
typedef struct RoomNode RoomNodeType;
typedef struct RoomList RoomListType;
typedef struct House HouseType;
typedef struct Ghost GhostType;
typedef struct Hunter HunterType;
typedef struct EvidenceNode EvidenceNodeType;
typedef struct EvidenceList EvidenceListType;

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

struct EvidenceList {
    EvidenceNodeType* head;
    EvidenceNodeType* tail;
    sem_t semaphore;
};

struct Hunter{
    char name[MAX_STR];
    RoomType* room;
    EvidenceType type;
    EvidenceListType* collectedEvidence;
    int fear;
    int boredom;

};

struct RoomNode{
    RoomType* data;
    RoomNodeType* next;

};

struct RoomList{
    RoomNodeType* head;
    RoomNodeType* tail;
};

struct Room{
    char name[MAX_STR];
    RoomListType rooms;
    EvidenceListType* evidenceList;
    HunterType* hunters[NUM_HUNTERS];
    int numOfHunters;
    GhostType* ghost;
    sem_t semaphore;

};

struct House {
    HunterType* hunters[NUM_HUNTERS];
    RoomListType rooms;
    EvidenceListType* collectedEvidence;
};

struct Ghost {
    GhostClass class;
    RoomType* room;
    int boredom;
};


struct EvidenceNode {
    EvidenceType evidence;
    EvidenceNodeType* next;
};

// house.c
void populateRooms(HouseType* house);
void cleanupHouse(HouseType *house);

// Helper Utilies / utils.c
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter
enum EvidenceType randomEvidenceType(enum GhostClass ghost); // return a randomly selected EvidenceType based on the provided GhostType
int printStatus(HunterType* hunter);
enum GhostClass determineGhost(int emf, int temp, int fingerprint);

// Logging Utilities / logger.c
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);

//init.c
void initHouse(HouseType *house, EvidenceListType *sharedEvidence);
void initEvidenceList(EvidenceListType *evList);
void initRoomList(RoomListType *list);
RoomType* createRoom(char *name);
void initHunter(char *name, EvidenceType evType, EvidenceListType *evList, RoomType *room, HunterType *hunter);
void initGhost(GhostClass class, GhostType *ghost, RoomListType *roomList);

//room.c
void populateRooms(HouseType* house);
void connectRooms(RoomType* room, RoomType *room1);
void addRoom(RoomListType *list, RoomType *room);
void hunterAddToRoom(RoomType* room, HunterType* hunter);
void hunterRemoveFromRoom(RoomType* room, HunterType* hunter);
void ghostAddToRoom(RoomType* room, GhostType* ghost);
void ghostRemoveFromRoom(RoomType* room);
void cleanupRooms(RoomListType* roomList);
void cleanupConnectedRooms(RoomListType* connectedRooms);

//evidence.c
void addEvidence(EvidenceListType* evList, EvidenceType evidence);
void removeEvidence(EvidenceListType* evList, EvidenceType evidence);
void cleanupEvidence(EvidenceListType* evList);

//action.c
void leaveEvidence(GhostType* ghost);
void ghostMove(GhostType* ghost);
void hunterMove(HunterType* hunter);
void hunterCollect(HunterType* hunter);
int reviewEvidence(HunterType* hunter);
void* hunterMain(void* ptr);
void* ghostMain(void* ptr);