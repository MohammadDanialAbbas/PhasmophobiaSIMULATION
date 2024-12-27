#include "defs.h"

/*
    Returns a pseudo randomly generated number, in the range min to (max - 1), inclusively
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated integer in the range [0, max-1) 
*/
int randInt(int min, int max)
{
    return (int) randFloat(min, max);
}

/*
    Returns a pseudo randomly generated floating point number.
    A few tricks to make this thread safe, just to reduce any chance of issues using random
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated floating point number in the range [min, max)
*/
float randFloat(float min, float max) {
    static __thread unsigned int seed = 0;
    if (seed == 0) {
        seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    }

    float random = ((float) rand_r(&seed)) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

/* 
    Returns a random enum GhostClass.
*/
enum GhostClass randomGhost() {
    return (enum GhostClass) randInt(0, GHOST_COUNT);
}

/*
    Returns the string representation of the given enum EvidenceType.
        in: type - the enum EvidenceType to convert
        out: str - the string representation of the given enum EvidenceType, minimum 16 characters
*/
void evidenceToString(enum EvidenceType type, char* str) {
    switch (type) {
        case EMF:
            strcpy(str, "EMF");
            break;
        case TEMPERATURE:
            strcpy(str, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            strcpy(str, "FINGERPRINTS");
            break;
        case SOUND:
            strcpy(str, "SOUND");
            break;
        default:
            strcpy(str, "UNKNOWN");
            break;
    }
}

/* 
    Returns the string representation of the given enum GhostClass.
        in: ghost - the enum GhostClass to convert
        out: buffer - the string representation of the given enum GhostClass, minimum 16 characters
*/
void ghostToString(enum GhostClass ghost, char* buffer) {
    switch(ghost) {
        case BANSHEE:
            strcpy(buffer, "Banshee");
            break;
        case BULLIES:
            strcpy(buffer, "Bullies");
            break;
        case PHANTOM:
            strcpy(buffer, "Phantom");
            break;
        case POLTERGEIST:
            strcpy(buffer, "Poltergeist");
            break;
        default:
            strcpy(buffer, "Unknown");
            break;
        
    }
}

// this function returns 1 evidence type based on the given ghost class
// the only parameter is the ghost class to get the evidencetype from
enum EvidenceType randomEvidenceType(enum GhostClass ghost) {
    int randomNum = randInt(0,3);
    
    // POLTERGEIST
    if (ghost == 0) {
        
    }

    // BANSHEE
    else if (ghost == 1) {
        // BANSHEE does not leave FINGERPRINTS, replace it with SOUND
        if (randomNum == 2) {
            randomNum = 3;
        }
    }

    // BULLIES
    else if (ghost == 2) {
        // BULLY cannot leave TEMPERATURE, replace it with SOUND
        if (randomNum == 1) {
            randomNum = 3;
        }
    }

    //
    else if (ghost == 3) {
        if (randomNum == 0) {
            randomNum = 3;
        }
    }

    // this shouldn't be called but just in case to check for errors;
    else {
        printf("Invalid GhostClass\n");
    }

    return (enum EvidenceType) randomNum; 
}

// this is a helper function to print whether the hunter was too bored or scared
// the only parameter is the hunter to check its boredom and fear level
int printStatus(HunterType* hunter) {
    if (hunter->boredom >= BOREDOM_MAX) {
        printf("%s was too bored! \n", hunter->name);
        return 1;
    }

    else if (hunter->fear >= FEAR_MAX) {
        printf("%s was scared away! \n", hunter->name);
        return 1;
    }

    return 0;
}

// this function returns a ghost class based on the evidence types given
// the three parameters are "booleans" which tell us what type of evidence type was found
enum GhostClass determineGhost(int emf, int temp, int fingerprint) {
    // poltergeist, banshee, bully
    if (emf == 1) {
        // poltergeist or banshee
        if (temp == 1) {
            // poltegeist
            if (fingerprint == 1) {
                return POLTERGEIST;
            }
            // banshee
            else {
                return BANSHEE;
            }
        }
        // BULLIES
        else {
            return BULLIES;
        }
    }
    // PHANTOM
    else {
        return PHANTOM;
    }
}