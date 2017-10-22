
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include <stdbool.h>

/* Function for extracting set index */
int getSetIndex(char binaryPtr[], int numberOfTagBits, int numberOfSetIndexBits){
    
    int setIndex[numberOfSetIndexBits];

    // get the set index
    int y = 0;
    for (; y < numberOfSetIndexBits; y++){
        //printf("%d\n", binaryPtr[y+numberOfTagBits]);
        setIndex[y] = binaryPtr[y+numberOfTagBits]-48;
    }

    // convert set index from binary to decimal
    int index = 0;
    int i = 0;
    for (; i < numberOfSetIndexBits; i++){
        index = index * 2 + setIndex[i];
    }

    return index;
}

/* Function for extracting set index in TYPE B */
int getSetIndexForTypeB(char binaryPtr[], int numberOfSetIndexBits){
    
    int setIndex[numberOfSetIndexBits];

    // get the set index
    int y = 0;
    for (; y < numberOfSetIndexBits; y++){
        setIndex[y] = binaryPtr[y]-48;
    }

    // convert set index from binary to decimal
    int index = 0;
    int i = 0;
    for (; i < numberOfSetIndexBits; i++){
        index = index * 2 + setIndex[i];
    }
    return index;
}

/* Helper function for converting hex to binary */
char* hexToBinaryHelper(char hexChar){
    char * hex[] = {"0000", "0001", "0010", "0011", "0100", "0101",
                     "0110", "0111", "1000", "1001", "1010", "1011",
                     "1100", "1101", "1110", "1111"};
    if (hexChar >= '0' && hexChar <= '9'){
        return hex[hexChar - '0'];
    }
    if (hexChar >= 'a' && hexChar <= 'f') {
        return hex[10 + hexChar - 'a'];
    }
    return "-1";
}

/* Main Function */
int main(int argc, char ** argv){


    int cacheSize; // holds input cache size
    int blockSize; // holds input block size

    // check for input
    if (argc != 5){
        printf("error\n");
        exit(0);
    }

    char* cache_size = argv[1];
    char* inputAssoc = argv[2];
    char* block_size = argv[3];
    char* trace_file = argv[4];

    // this is cache size
    sscanf(cache_size, "%d", &cacheSize);

    // block size
    sscanf(block_size, "%d", &blockSize);

    // get the associative
    int associative;
    if (strcmp(inputAssoc, "direct") == 0){ // direct
        associative = 1;
    } else if (strcmp(inputAssoc, "assoc") == 0){   // fully
        associative = cacheSize/blockSize;
    } else {    // assoc:n
        // get the number of associative
        sscanf(inputAssoc, "assoc:%d", &associative);
    }

    // get the number of tag bits
    int numberOfSet = cacheSize/(blockSize*associative);
    int numberOfSetIndexBits = log(numberOfSet)/log(2);
    int blockOffest = log(blockSize)/log(2);
    int numberOfTagBits = 48 - numberOfSetIndexBits - blockOffest;

    // read trace file
    FILE *traceFile;
    traceFile = fopen(trace_file, "r");
    char firstItem[20];
    char operation;
    char memAddress[40];

    /*** 2d array for holding cache info ***/
    char* cache[numberOfSet][associative];  // type A
    char* cacheB[numberOfSet][associative]; // type B

    // initialize cache array A
    int initCounter1 = 0;
    int initCounter2 = 0;
    for (; initCounter1 < numberOfSet; initCounter1++){
        for (; initCounter2 < associative; initCounter2++){
            cache[initCounter1][initCounter2] = malloc(46);
            cache[initCounter1][initCounter2][0] = '\0';
        }
        initCounter2 = 0;
    }

    // initialize cache array B
    int initCounter1B = 0;
    int initCounter2B = 0;
    for (; initCounter1B < numberOfSet; initCounter1B++){
        for (; initCounter2B < associative; initCounter2B++){
            cacheB[initCounter1B][initCounter2B] = malloc(46);
            cacheB[initCounter1B][initCounter2B][0] = '\0';
        }
        initCounter2B = 0;
    }
   
    /* Output value holders for TYPE A */
    int numberOfHit = 0;    // number of hit
    int numberOfMiss = 0;        // number of miss
    int read = 0;       
    int write = 0;
    /***********************/
    /* Output value holders for TYPE B */
    int numberOfHitB = 0;    // number of hit
    int numberOfMissB = 0;        // number of miss
    int readB = 0;       
    int writeB = 0;

    // main while loop to read file line by line
    while(fscanf(traceFile, "%s %c %s", firstItem, &operation, memAddress) != EOF){
        if (strcmp(firstItem, "#eof") == 0){    // end of the file
            break;
        }

        // get the binary string
        int length = (strlen(memAddress)-2)*4;
        char binary[length+1];
        int xcount = 0;


        for (xcount = 0; xcount < length+1; xcount++){
            binary[xcount] = '\0';
        }

        int icount;
        int countp = 0;
        int tmpCounterp = 0;
        for (icount = 2; icount < strlen(memAddress); icount++){
            char* tmpBinary = hexToBinaryHelper(memAddress[icount]);
            //printf("%s\n", tmpBinary);
            //snprintf(binary, sizeof(binary), "%s%s", binary, tmpBinary);
            //strcat(binary, tmpBinary);
            //tmpCounterp
            for (; tmpCounterp < 4; tmpCounterp++){
                binary[countp] = tmpBinary[tmpCounterp];
                countp++;
            }
            tmpCounterp = 0;
        }

        // move binary string to a fixed size array
        char finalBinary[49];
        finalBinary[48] = '\0';
        int x;
        for (x = 0; x < 48; x++){
            finalBinary[x] = '0';
        }
    
        int t = 47;
        //int y = strlen(binary)-1;  
        int y = (strlen(memAddress)-2)*4;

        for (; t >= 0; t--){
            if (y == -1){break;}
            finalBinary[t] = binary[y];
            y--;
        }

        /***************** implement Type A *******************/
        // call function to get the tag
        //char* tag = getTag(finalBinary, numberOfTagBits);

        char tag[numberOfTagBits];   // holds the tag info

        // initialize tag array
        
        int tagCount = 0;
        for (; tagCount < numberOfTagBits; tagCount++){
            tag[tagCount] = '\0';
        }
        
        // get the tag
        int xx = 0;
        for (; xx < numberOfTagBits-1; xx++){
            if (xx < numberOfTagBits-1){
                tag[xx] = finalBinary[xx];
            } else {
                break;
            }
        }

        // call function to get the set index
        int setIndex = getSetIndex(finalBinary, numberOfTagBits-1, numberOfSetIndexBits);

        /* put tag into the cache */
        // go to the index in cache
        char* tagPtr = tag;
        int countHit = 0;
        int countHit2 = 0;
        bool isMatch = true;
        for (; countHit < associative; countHit++){
            for (; countHit2 < numberOfTagBits-1; countHit2++){
                if (cache[setIndex][countHit][countHit2] != tagPtr[countHit2]){
                    isMatch = false;
                    break;
                }
            }
            if (isMatch == true){   // ** CAHCE HIT **
                numberOfHit++;
                if (operation == 'W') { write++; }
                break;
            }
            countHit2 = 0;
            if (countHit == associative-1){break;}
            isMatch = true;
        }

        if (isMatch == false){
            numberOfMiss++;
            if (operation == 'W'){read++; write++;}
            if (operation == 'R'){read++;}

            // check for empty spot
            int emptyCount = 0;
            bool hasEmptySpot = false;
            for (; emptyCount < associative; emptyCount++){
                if (cache[setIndex][emptyCount][0] == '\0'){    // has empty spot, copy address into the cache
                    memcpy(cache[setIndex][emptyCount], &tagPtr[0], numberOfTagBits-1);
                    cache[setIndex][emptyCount][numberOfTagBits-1] = '\0';
                    hasEmptySpot = true;
                    break;
                }
            }

            if (hasEmptySpot == false){ // no empty spot, need to replace
                int count = 1;
                for (; count < associative; count++){
                    memcpy(cache[setIndex][count-1], &cache[setIndex][count][0], numberOfTagBits-1);
                    cache[setIndex][count-1][numberOfTagBits-1] = '\0';
                }
                memcpy(cache[setIndex][associative-1], &tagPtr[0], numberOfTagBits-1);
                cache[setIndex][associative-1][numberOfTagBits-1] = '\0';
            }
        }

        /**************** implement Type  B *****************/

        // tag for Type B
        char tagB[numberOfTagBits];   // holds the tag info

        // initialize tag array
        int tagCountB = 0;
        for (; tagCountB < numberOfTagBits; tagCountB++){
            tagB[tagCountB] = '\0';
        }
        
        // get the tag B
        int xxB = 0;
        int tagBCounter = numberOfSetIndexBits;
        for (; xxB < numberOfTagBits-1; xxB++){
            if (xxB < numberOfTagBits-1){
                tagB[xxB] = finalBinary[tagBCounter];
                tagBCounter++;
            } else {
                break;
            }
        }

        // call function to get the set index
        int setIndexB = getSetIndexForTypeB(finalBinary, numberOfSetIndexBits);

        /* put tag into the cache TYPE B*/
        // go to the index in cache
        char* tagPtrB = tagB;
        countHit = 0;
        countHit2 = 0;
        isMatch = true;
        for (; countHit < associative; countHit++){
            for (; countHit2 < numberOfTagBits-1; countHit2++){
                if (cacheB[setIndexB][countHit][countHit2] != tagPtrB[countHit2]){
                    isMatch = false;
                    break;
                }
            }
            if (isMatch == true){   // ** CAHCE HIT **
                numberOfHitB++;
                if (operation == 'W') { writeB++; }
                break;
            }
            countHit2 = 0;
            if (countHit == associative-1){break;}
            isMatch = true;
        }

        if (isMatch == false){
            numberOfMissB++;
            if (operation == 'W'){readB++; writeB++;}
            if (operation == 'R'){readB++;}

            // check for empty spot
            int emptyCount = 0;
            bool hasEmptySpot = false;
            for (; emptyCount < associative; emptyCount++){
                if (cacheB[setIndexB][emptyCount][0] == '\0'){    // has empty spot, copy address into the cache
                    memcpy(cacheB[setIndexB][emptyCount], &tagPtrB[0], numberOfTagBits-1);
                    cacheB[setIndexB][emptyCount][numberOfTagBits-1] = '\0';
                    hasEmptySpot = true;
                    break;
                }
            }


            if (hasEmptySpot == false){ // no empty spot, need to replace
                int count = 1;
                for (; count < associative; count++){
                    memcpy(cacheB[setIndexB][count-1], &cacheB[setIndexB][count][0], numberOfTagBits-1);
                    cacheB[setIndexB][count-1][numberOfTagBits-1] = '\0';
                }
                memcpy(cacheB[setIndexB][associative-1], &tagPtrB[0], numberOfTagBits-1);
                cacheB[setIndexB][associative-1][numberOfTagBits-1] = '\0';
            }
        }
    }

    // free Memory
    int free1 = 0;
    int free2 = 0;
    for (; free1 < numberOfSet; free1++){
        for (; free2 < associative; free2++){
            free(cache[free1][free2]);
            free(cacheB[free1][free2]);
        }
        free2 = 0;
    }

    printf("cache A\n");
    printf("Memory reads: %d\n", read);
    printf("Memory writes: %d\n", write);
    printf("Cache hits: %d\n", numberOfHit);
    printf("Cache misses: %d\n", numberOfMiss);

    printf("cache B\n");
    printf("Memory reads: %d\n", readB);
    printf("Memory writes: %d\n", writeB);
    printf("Cache hits: %d\n", numberOfHitB);
    printf("Cache misses: %d\n", numberOfMissB);

    fclose(traceFile);
    return 0;
}
