/* KW26 Assignment - Homework 2
 * I Derek McCrae (de842158) affirm that this program is entirely my own work
 * and that I have neither developed my code together with any another person, nor copied any code from any other person,
 * nor permitted my code to be copied or otherwise used by any other
 * person, nor have I copied, modified, or otherwise used program code that I have found in any external source,
 * including but not limited to, online sources.
 * I acknowledge that any violation of the above terms will be treated as academic dishonesty.
 */

//pre-processor directives
#include <stdio.h>
#include <stdlib.h>
#include "kw26.h"
#include <string.h>

//start functions
int checkString(char *str, int num){
    //declare variables
    int final;
    
    //make appropriate adjustments
    if(str[num] == 'a' || str[num] =='A')
        return 10;
    else if(str[num] == 'b' || str[num] =='B')
        return 11;
    else if(str[num] == 'c' || str[num] =='C')
        return 12;
    else if(str[num] == 'd' || str[num] =='D')
        return 13;
    else if(str[num] == 'e' || str[num] =='E')
        return 14;
    else if(str[num] == 'f' || str[num] =='F')
        return 15;
    else{
        final = (int) str[num] - 48;
        return final;
    }

}

Int40 *kw26Add(Int40 *p, Int40 *q){
    //declare variables
    int carry = 0, sum = 0, i;
    Int40 *add = malloc(sizeof(struct Int40));
    add->digits = malloc(40 * sizeof(int));
    //make sure space given
    if(add == NULL || add->digits == NULL){
        return NULL;
    }
    //make sure both structures are filled
    if(p == NULL || q == NULL)
        return NULL;
    
    //add
    for(i=0; i<40; i++){
        sum = p->digits[i] + q->digits[i] + carry;
        //once sum obtained
        if(sum == 16){
            add->digits[i] = 0;
            carry = 1;
        }
        else if(sum > 9 && sum <= 15){
            carry = 0;
            add->digits[i] = sum;
        }
        else if(sum > 16){
            carry = 1;
            add->digits[i] = sum - 16;
        }
        else{
            carry = 0;
            add->digits[i] = sum;
        }
    }
    
    //verify number created
    if(add == NULL)
        return NULL;
    else
        return add;
}

Int40 *kw26Destroyer(Int40 *p){
    //make sure p had contents
    if (p == NULL){
      return NULL;
    }
    else{
        p->digits = NULL;
        return NULL;
    }
}

Int40 *parseString(char *str){
    //verify str filled
    if(str == NULL)
        return NULL;
    //declare variables
    int i, length, j;
    Int40 *parse = malloc(sizeof(struct Int40));
    parse->digits = malloc(40 * sizeof(int));
    //make sure space given
    if(parse == NULL || parse->digits == NULL){
        return NULL;
    }
    
    if(strlen(str)==0){
        for(i=0; i<40; i++){
            parse->digits[i] = 0;
        }
    }
    else{
        length = strlen(str) - 1;
        j=0;
        if(length < 40){
            for(i=39; i>=length; i--){
                parse->digits[i] = 0;
                i--;
            }
            for(i=length; i>=0; i--){
                parse->digits[i] = checkString(str, j);
                j++;
            }
        }
        else{
            for(i=39; i>=0; i--){
                parse->digits[i] = checkString(str, j);
                j++;
            }
        }
    }
    
    //verify number created
    if(parse == NULL)
        return NULL;
    else
        return parse;
}

Int40 *fibKw26(int n, Int40 *first, Int40 *second){
    //verify first and second filled
    if(first == NULL || second == NULL)
        return NULL;
    //declare variables
    Int40 * fib = malloc(sizeof(struct Int40));
    int i;
    
    //make sure malloc successful
    if(fib == NULL)
        return NULL;
    
    //fib math
    if(n == 0)
        return first;
    else if(n == 1)
        return second;
    for(i=2; i<=n; i++){
        fib = kw26Add(first, second);
        first = second;
        second = fib;
        if(second == NULL)
            return NULL;
    }
    
    //make sure fib achieved
    if(second == NULL)
        return NULL;
    return second;
}

Int40 *encrypt(Int40 *key, Int40 *inputText){
    //verify key & inputText filled
    if(key == NULL || inputText == NULL)
        return NULL;
    //declare variables
    int i;
    Int40 * incrypt = malloc(sizeof(struct Int40));
    incrypt->digits = malloc(40 * sizeof(int));
    //make sure mallocs successful
    if(incrypt == NULL || incrypt->digits == NULL)
        return NULL;
    
    for(i=0; i<40; i++){
        incrypt->digits[i] = key->digits[i] ^ inputText->digits[i];
    }
    
    if(incrypt == NULL)
        return NULL;
    return incrypt;
}

void kw26Rating(void){
    kw26RatingStruct * user = malloc(sizeof(struct kw26RatingStruct));
    user->NID = malloc(10 * sizeof(char));
    if(user == NULL){
        return;
    }
    
    strcpy(user->NID, "de842158");
    user->degreeOfDifficulty = 4.002;
    user->duration = 25;
    
    //print to STDERR
    fprintf(stderr, "%s;", user->NID);
    fprintf(stderr, "%f;", user->degreeOfDifficulty);
    fprintf(stderr, "%f;", user->duration);
    fprintf(stderr, "\n");
    
    return;
}

Int40 *loadHWConfigVariable(int doSeed){
    //declare variables
    int i, j = 0, random[5];
    Int40 * compute = malloc(sizeof(struct Int40));
    compute->digits = malloc(40 * sizeof(int));
    //make sure space given
    if(compute == NULL || compute->digits == NULL)
        return NULL;
    
    if(doSeed == 0){
        for(i=0; i<40; i++){
            compute->digits[i] = 1;
        }
    }
    else{
        srand(doSeed);
        for(i=0; i<5; i++){
            random[i] = rand() % 16; //only hexadecimal numbers created
        }
        for(i=39; i>=0; i--){
            if(j == 5)
                j=0;
            compute->digits[i] = random[j];
            j++;
        }
    }
    
    //verify number created
    if(compute == NULL)
        return NULL;
    else
        return compute;
}

Int40 *loadCryptoVariable(char *inputFilename){
    //verify filename given
    if(inputFilename == NULL)
        return NULL;
    //declare variables & check memory
    Int40 * user = malloc(sizeof(struct Int40));
    user->digits = malloc(sizeof(int)*40);
    if(user == NULL)
        return NULL;
    FILE *ifp = NULL;
    int length, i, j = 0;
    char *inBuf;
    ifp = fopen(inputFilename, "r");
    if(ifp == NULL)
        return NULL;
    inBuf = malloc(100 * sizeof(char));
    if (inBuf == NULL)
        return NULL;
    
    //get info from file
    fscanf(ifp, "%s", inBuf);
    length = strlen(inBuf);
    if(length < 40){
        for(i=39; i>length-1; i--){
            user->digits[i] = 0;
        }
        for(i=length-1; i>=0; i--){
            user->digits[i] = checkString(inBuf, j);
            j++;
        }
    }
    else{
        for(i = 39; i >= 0; i--){
            user->digits[i] = checkString(inBuf, j);
            j++;
        }
    }
    
    //verify number created
    if(user == NULL){
        return NULL;
    }
    else{
        return user;
    }
}

Int40 *loadPlainText(char *inputFilename){
    //verify filename given
    if(inputFilename == NULL)
        return NULL;
    //declare variables
    Int40 * text = malloc(sizeof(struct Int40));
    text->digits = malloc(40 * sizeof(int));
    char *inBuf = malloc(100 * sizeof(char));
    int i, length, scancount = 0, j, indexcount, hold;
    FILE * ifp = NULL;
    ifp = fopen(inputFilename, "r");
    //make sure malloc text successful
    if(text == NULL || inBuf == NULL || ifp == NULL || text->digits == NULL)
        return NULL;
    
    indexcount = 39;
    while(fscanf(ifp, "%s", inBuf) != EOF){
        j=0;
        length = strlen(inBuf);
        if(length >= 40){
            for(i=indexcount; i>=0; i--){
                text->digits[i] = (int) inBuf[j];
                j++;
            }
            //make sure Plain text successful
            if(text == NULL){
                return NULL;
            }
            else{
                return text;
            }
        }
        else{
            for(i=indexcount; i>indexcount-length; i--){
                text->digits[i] = (int) inBuf[j];
                j++;
                hold = i;
                //make sure not to leave array bounds
                if(i == 0){
                    //make sure Plain text successful
                    if(text == NULL){
                        return NULL;
                    }
                    return text;
                }
            }
            indexcount = hold;
        }
    }
    if(indexcount != 0){ //Int40 array not completely filled so need to shift entries to allow zeros to be added to front
        hold = 39 - indexcount;
        for(i=0; i<=hold; i++){
            text->digits[i] = text->digits[indexcount+1];
            indexcount++;
        }
        for(i=hold+1; i<40; i++){
            text->digits[i] = 0;
        }
    }
    
    //make sure Plain text successful
    if(text == NULL){
        return NULL;
    }
    return text;
}


