/*I Derek McCrae (de842158) affirm that this program is entirely my own work
 * and that I have neither developed my code together with any another person, nor copied any code from any other person,
 * nor permitted my code to be copied or otherwise used by any other
 * person, nor have I copied, modified, or otherwise used program code that I have found in any external source,
 * including but not limited to, online sources.
 * I acknowledge that any violation of the above terms will be treated as academic dishonesty.
 */

//pre=processor directives
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "airPdata.h"

//prototypes
void printData(airPdata * airport);

//main function
int main(int argc, char **argv){
    //declare varibles
    FILE *ifp = NULL;
    char buf[500];
    char *fname = argv[1];
    ifp = fopen(fname, "r");
    airPdata * airport = (airPdata *)malloc(sizeof(airPdata));

    if (ifp == NULL){
        fprintf(stderr, "etl ERROR: File '%s' not found.\n", fname);
        exit(0);
    }

    //header
    printf("%-12s %-11s %-42s %-34s %-3s %-15s %-16s Tower\n","FAA Site", "Short Name", "Airport Name", "City", "ST","Latitude", "Longitude");
    printf("%-12s %-11s %-42s %-34s %-3s %-15s %-16s =====\n","========", "==========", "============", "====", "==","========", "=========");

    while (fgets(buf, sizeof(buf), ifp) != NULL){
        //declare variables
        char *p, *b;
        p = malloc(51 * sizeof(char));
        b = buf;
        int index = 1;

        //malloc
        airport->siteNumber = malloc(51 * sizeof(char));
        airport->LocID = malloc(4 * sizeof(char));
        airport->fieldName = malloc(51 * sizeof(char));
        airport->city = malloc(51 * sizeof(char));
        airport->state = malloc(3 * sizeof(char));
        airport->latitude = malloc(51 * sizeof(char));
        airport->longitude = malloc(51 * sizeof(char));

        while ((p = strsep(&b, ",")) != NULL){
            if(index == 1){
                strcpy(airport->siteNumber, p);
            }//end index 1
            else if(index == 2){
                strcpy(airport->LocID, p);
            }//end index 2
            else if(index == 3){
                strcpy(airport->fieldName, p);
            }//end index 3
            else if(index == 4){
                strcpy(airport->city, p);
            }//end index 4
            else if(index == 5){
                strcpy(airport->state, p);
            }//end index 5
            else if(index == 9){
                strcpy(airport->latitude, p);
            }//end index 9
            else if(index == 10){
                strcpy(airport->longitude, p);
            }//end index 10
            else if(index == 15){
                airport->controlTower = *p;
            }//end index 15
            index ++;

        }//end while category
        printData(airport);
        
        airPdata *airport = (airPdata *)malloc(sizeof(airPdata));
        free(airport->longitude);
        free(airport->latitude);
        free(airport->state);
        free(airport->city);
        free(airport->fieldName);
        free(airport->LocID);
        free(airport->siteNumber);

        if (p != NULL){
            free(p);
        }
    }//end while airport

    //close files and free struct
    free(airport);
    fclose(ifp);

    return 0;
}

void printData(airPdata * airport){

    if(airport->siteNumber == NULL || airport->LocID == NULL || airport->fieldName == NULL || airport->city == NULL || airport->state == NULL || airport->latitude == NULL || airport->longitude == NULL){
            fprintf(stderr, "NULL pointer was passed.\n");
            return;
        }//end if
    else{
        printf("%-12s %-11s %-42s %-34s %-3s %-15s %-16s %c\n", airport->siteNumber, airport->LocID, airport->fieldName, airport->city, airport->state, airport->latitude, airport->longitude, airport->controlTower);
    }//end else

    return;
}


