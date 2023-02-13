#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>

void convertToBin(long toConvert, int* converted, int size, int startPos){
    if (toConvert < 0){
        toConvert = -1 * toConvert;
    }
    int arrCounter = startPos;
    int largestPow = -1;
    if (toConvert == 0){
        for (int i = 0; i < size; i++){
            converted[i] = 0;
        }
        return;
    }
    if (toConvert == 1){
        for (int i = 0; i < size - 1; i++){
            converted[i] = 0;
        }
        converted[size-1] = 1;
        return;
    }
    if (toConvert == 2){
        for (int i = 0; i < size - 2; i++){
            converted[i] = 0;
        }
        converted[size-2] = 1;
        converted[size-1] = 0;
        return;
    }
    //find largest power of 2 that divides the whole number
    for (int i = 0; i < toConvert; i++){
        long twoPower = (1 << i);
        //if twoPower exceeds INT_MAX
        if (twoPower < 0){
            largestPow = 31;
            break;
        }
        if (toConvert % twoPower == toConvert){
            largestPow = i-1;
            break;
        }
    }

    for (int i = largestPow; i >=0; i--){ //loop through and try to divide by the largest power of 2 available
        if (toConvert >= (1 << i) && toConvert != 0){ //if successful put a 1 in the next position
            toConvert = toConvert - (1 << i);
            converted[arrCounter] = 1;
            arrCounter++;
        }
        else {
            converted[arrCounter] = 0;
            arrCounter++;
        }
    }
    //pad the array with leading 0s
    int lastPosition = arrCounter - 1;
    int shiftAmount = (size-1) - lastPosition;
    for (int i = lastPosition; i >=0 ; i--){
        converted[i+shiftAmount] = converted[i];
    }
    for (int i = 0; i < shiftAmount; i++){
        converted[i] = 0;
    }
    
       
    return;
}
    
void convertBig(char* num, int* converted, int size){
    int bigArrayCounter = 0;
    for (int i = 2; i < strlen(num); i++){
            char hexdigit[4] = "0x";
            hexdigit[2] = num[i];
            int digit = strtol(hexdigit, NULL, 16);
            //special cases
            if (digit == 0){
                for (int i = bigArrayCounter; i < bigArrayCounter + 4; i++){
                    converted[i] = 0;
                }
                bigArrayCounter = bigArrayCounter+ 4;
                continue;
            }
            if (digit == 1){
                for (int i = bigArrayCounter; i < bigArrayCounter + 4; i++){
                    converted[i] = 0;
                }
                bigArrayCounter = bigArrayCounter+ 4;
                converted[bigArrayCounter-1] = 1;
                continue;
            }
            if (digit == 0){
                for (int i = bigArrayCounter; i < bigArrayCounter + 4; i++){
                    converted[i] = 0;
                }
                bigArrayCounter = bigArrayCounter+ 4;
                converted[bigArrayCounter-2] = 1;
                continue;
            }
            //convert digit to binary
            for (int i = 3; i >=0; i--){
                if (digit >= (1 << i)){
                    digit = digit - (1<<i);
                    converted[bigArrayCounter] = 1;
                    bigArrayCounter++;
                }
                else{
                    converted[bigArrayCounter] = 0;
                    bigArrayCounter++;
                }
            }
        }
    }


int main(int argc, char** argv){
    int numBits;
    int expBits;
    int fbits;
    char num[100];
    for (int i = 0; i < 100; i++){
        num[i] = 't'; //initialize all values to t so the end of the actual number can be seen
    }
    int decPlaces;
    
    FILE* fp = fopen(argv[1], "r");

    while (fscanf(fp, "%d ", &numBits) != EOF){
        fscanf(fp, "%d ", &expBits);
        fscanf(fp, "%d ", &fbits);
        fscanf(fp, "%s ", num);
        fscanf(fp, "%d\n", &decPlaces);
        int binSize = (strlen(num)-2) * 4;
        int bin_of_hex[binSize];
        long number = strtol(num, NULL, 16);
        if (number > INT_MAX){
            convertBig(num, bin_of_hex, binSize);
        }
        else{
            convertToBin(number, bin_of_hex, binSize, 0);
        }
        /*for (int i = 0; i < binSize; i++){
            printf("%d\n", bin_of_hex[i]);
        }*/
        //copy the specified number of bits into ieee array
        int ieee[numBits];
        int ieeeCount = 0;
        int startingPt = binSize - numBits;
        for (int i = startingPt; i < binSize; i++){
            ieee[ieeeCount] = bin_of_hex[i];
            ieeeCount++;
        }
        
        //check the sign bit
        int S = 0;
        if (ieee[0] == 0){
            S = 1;
        }
        else if (ieee[0] == 1){
            S = -1;
        }
        //convert exponent section to decimal
        int E = 0;
        int bias = (1 << (expBits-1))-1;
        int power = expBits - 1;
        int subNormal = 1; //assume value is subnormal
        for (int i = 1; i < expBits+1; i++){
            E += (ieee[i] << power);
            if (ieee[i] == 1){
                subNormal = 0;//if 1 is detected the value is not subNormal
            }
            power--;
        }

        //convert fraction section to decimal
        int negPower = -1;
        double MPart = 1;
        for (int i = expBits + 1; i < numBits; i++){
            MPart += ieee[i] * pow(2, negPower);
            negPower--;
        }
        if (subNormal == 0){
            double final = S * MPart * pow(2, E-bias);
            printf("%.*lf\n", decPlaces, final);
        }
        else if (subNormal == 1){
            MPart = MPart - 1;
            double final = S * MPart * pow(2, 1-bias);
            printf("%.*lf\n", decPlaces, final);
        }
    }
    return 0;
}