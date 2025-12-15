#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 3
#define MAX_RESOURCES 3
#define MAX_NAME_LEN 20

typedef enum{ 
    READ = 1, 
    WRITE = 2,  
    EXECUTE = 4 
}Permission;

typedef struct{
    char name[MAX_NAME_LEN];
}User;

typedef struct{
    char name[MAX_NAME_LEN];
}Resource;

typedef struct{
    char userName[MAX_NAME_LEN];
    int permissions; 
}ACLEntry;

typedef struct{
    Resource resource;
    ACLEntry entries[MAX_USERS];
    int entryCount;
}ACLControlledResource;

typedef struct{
    char resourceName[MAX_NAME_LEN];
    int permissions;
}Capability;

typedef struct{
    char userName[MAX_NAME_LEN];
    Capability capabilities[MAX_RESOURCES];
    int capabilityCount;
}CapabilityUser;

void printPermissions(int perm){
    if (perm & READ) {
        printf("READ ");
    }
    if (perm & WRITE) {
        printf("WRITE ");
    }
    if (perm & EXECUTE) {
        printf("EXECUTE ");
    }
}

int hasPermission(int userPerm, int requiredPerm){
    return (userPerm & requiredPerm) == requiredPerm;
}

void checkACLAccess(ACLControlledResource *res, const char *userName, int perm){
    for(int i=0; i < res->entryCount; i++){
        if(strcmp(res->entries[i].userName, userName) == 0){
            if(hasPermission(res->entries[i].permissions, perm)){
                printf("ACL Check: User %s requests ", userName);
                printPermissions(perm);
                printf("on %s: Access GRANTED\n", res->resource.name);
            } else {
                printf("ACL Check: User %s requests ", userName);
                printPermissions(perm);
                printf("on %s: Access DENIED\n", res->resource.name);
            }
            return;
        }
    }
    printf("ACL Check: User %s has NO entry for resource %s: Access DENIED\n", userName, res->resource.name);
}

void checkCapabilityAccess(CapabilityUser *user, const char *resourceName, int perm){
    for(int i=0; i < user->capabilityCount; i++){
        if(strcmp(user->capabilities[i].resourceName, resourceName) == 0){
            if(hasPermission(user->capabilities[i].permissions, perm)){
                printf("Capability Check: User %s requests ", user->userName);
                printPermissions(perm);
                printf("on %s: Access GRANTED\n", resourceName);
            } else {
                printf("Capability Check: User %s requests ", user->userName);
                printPermissions(perm);
                printf("on %s: Access DENIED\n", resourceName);
            }
            return;
        }
    }
    printf("Capability Check: User %s has NO capability for %s: Access DENIED\n", user->userName, resourceName);
}


int main(){
    User users[MAX_USERS] = {{"Alice"}, {"Bob"}, {"Charlie"}};
    Resource resources[MAX_RESOURCES] = {{"File1"}, {"File2"}, {"File3"}};
    ACLControlledResource aclResources[MAX_RESOURCES];
    for(int i=0; i < MAX_RESOURCES; i++){
        aclResources[i].resource = resources[i];
        aclResources[i].entryCount = 0;
    }
    aclResources[0].entries[0] = (ACLEntry){"Alice", READ | WRITE};
    aclResources[0].entries[1] = (ACLEntry){"Bob", READ};
    aclResources[0].entryCount = 2;
    aclResources[1].entries[0] = (ACLEntry){"Charlie", READ};
    aclResources[1].entryCount = 1;
    aclResources[2].entryCount = 0;
    CapabilityUser capUsers[MAX_USERS];

    strcpy(capUsers[0].userName, "Alice");
    capUsers[0].capabilities[0] = (Capability){"File1", READ | WRITE};
    capUsers[0].capabilityCount = 1;

    strcpy(capUsers[1].userName, "Bob");
    capUsers[1].capabilities[0] = (Capability){"File1", READ};
    capUsers[1].capabilityCount = 1;

    strcpy(capUsers[2].userName, "Charlie");
    capUsers[2].capabilityCount = 0; 

    checkACLAccess(&aclResources[0], "Alice", READ);
    checkACLAccess(&aclResources[0], "Bob", WRITE);
    checkACLAccess(&aclResources[0], "Charlie", READ);

    checkCapabilityAccess(&capUsers[0], "File1", WRITE);
    checkCapabilityAccess(&capUsers[1], "File1", WRITE);
    checkCapabilityAccess(&capUsers[2], "File2", READ);

    return 0;
}
