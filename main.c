#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));

    //User-inputted Values
    char name1[MAX_STR];
    char name2[MAX_STR];
    char name3[MAX_STR];
    char name4[MAX_STR];
    int device1;
    int device2;
    int device3;
    int device4;

    printf("Enter the name for Hunter 1: ");
    scanf("%s", name1);
    printf("Enter the name for Hunter 2: ");
    scanf("%s", name2);
    printf("Enter the name for Hunter 3: ");
    scanf("%s", name3);
    printf("Enter the name for Hunter 4: ");
    scanf("%s", name4);

    printf("Enter the device for hunter 1 (0. EMF, 1. TEMPERATURE, 2. FINGERPRINTS, 3.SOUND,): ");
    scanf("%d", &device1);
    printf("Enter the device for hunter 2 (0. EMF, 1. TEMPERATURE, 2. FINGERPRINTS, 3.SOUND,): ");
    scanf("%d", &device2);
    printf("Enter the device for hunter 3 (0. EMF, 1. TEMPERATURE, 2. FINGERPRINTS, 3.SOUND,): ");
    scanf("%d", &device3);
    printf("Enter the device for hunter 4 (0. EMF, 1. TEMPERATURE, 2. FINGERPRINTS, 3.SOUND,): ");
    scanf("%d", &device4);

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;
    
    // !! this Evidence List will be shared among all the hunters and the house's evidence list
    EvidenceListType* sharedEvidenceList = (EvidenceListType*) malloc(sizeof(EvidenceListType));
    initEvidenceList(sharedEvidenceList);

    initHouse(&house, sharedEvidenceList);
    populateRooms(&house);

    GhostType* ghost = (GhostType*) malloc(sizeof(GhostType));
    initGhost(POLTERGEIST, ghost, &house.rooms);
    
    HunterType* hunter1 = (HunterType*) malloc(sizeof(HunterType));
    initHunter(name1, device1, sharedEvidenceList, house.rooms.head->data, hunter1);
    HunterType* hunter2 = (HunterType*) malloc(sizeof(HunterType));
    initHunter(name2, device2, sharedEvidenceList, house.rooms.head->data, hunter2);
    HunterType* hunter3 = (HunterType*) malloc(sizeof(HunterType));
    initHunter(name3, device3, sharedEvidenceList, house.rooms.head->data, hunter3);
    HunterType* hunter4 = (HunterType*) malloc(sizeof(HunterType));
    initHunter(name4, device4, sharedEvidenceList, house.rooms.head->data, hunter4);
    
    pthread_t t_hunter1, t_hunter2, t_hunter3, t_hunter4;
    pthread_t t_ghost;

    // create the threads
    pthread_create(&t_ghost, NULL, ghostMain, (void *) ghost);
    pthread_create(&t_hunter1, NULL, hunterMain, (void *) hunter1);
    pthread_create(&t_hunter2, NULL, hunterMain, (void *) hunter2);
    pthread_create(&t_hunter3, NULL, hunterMain, (void *) hunter3);
    pthread_create(&t_hunter4, NULL, hunterMain, (void *) hunter4);
    
    // join the threads
    pthread_join(t_ghost, NULL);
    pthread_join(t_hunter1, NULL);
    pthread_join(t_hunter2, NULL);
    pthread_join(t_hunter3, NULL);
    pthread_join(t_hunter4, NULL);
    
    int counter = 0;
    counter += printStatus(hunter1);
    counter += printStatus(hunter2);
    counter += printStatus(hunter3);
    counter += printStatus(hunter4);

    if (counter >= 4) {
        printf("The Ghost has won!\n");
    }

    EvidenceNodeType* current = hunter1->collectedEvidence->head;
    counter = 0;
    int emfFound = 0;
    int tempFound = 0;
    int fingerprintFound = 0;
    printf("The types of evidence that were found were: \n");
    // loop through and count the types of evidence
    while (current != NULL) {
        if (current->evidence == 0) {
            printf(" - EMF\n");
            counter++;
            emfFound = 1;
        }

        else if (current->evidence == 1) {
            printf(" - TEMPERATURE\n");
            counter++;
            tempFound = 1;
        }

        else if (current->evidence == 2) {
            printf(" - FINGERPRINTS\n");
            counter++;
            fingerprintFound = 1;
        }

        else if (current->evidence == 3) {
            printf(" - SOUND\n");
            counter++;
        }

        current = current->next;
    }
    printf("\n");
    // if 3 or more unique pieces of evidence were found
    if (counter >= 3) {
        // determine the ghost type given the types of evidence they found
        GhostClass ghostClass = determineGhost(emfFound, tempFound, fingerprintFound);
        if (ghostClass == POLTERGEIST) {
            printf("The ghost was found to be a POLTEGEIST.\n");
        }
        else if (ghostClass == BANSHEE) {
            printf("The ghost was found to be a BANSHEE.\n");
        }
        else if (ghostClass == BULLIES) {
            printf("The ghost was found to be a BULLIES.\n");
        }
        else {
            printf("The ghost was found to be a PHANTOM.\n");
        }

    }

    // free all the data
    cleanupHouse(&house);
    free(sharedEvidenceList);

    free(ghost);
    free(hunter1);
    free(hunter2);
    free(hunter3);
    free(hunter4);

    return 0;
}

