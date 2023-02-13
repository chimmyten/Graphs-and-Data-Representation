#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

void convertE(int toConvert, int* converted, int size){
    int arrCounter = 0;
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
    
    
    for (int i = size - 1; i >=0; i--){ //loop through and try to divide by the largest power of 2 available
        if (toConvert % (1 << i) != toConvert && toConvert != 0){ //if successful put a 1 in the next position
            toConvert = toConvert - (1 << i);
            converted[arrCounter] = 1;
            arrCounter++;
        }
        else {
            converted[arrCounter] = 0;
            arrCounter++;
        }
    }
    /*for (int i = arrCounter; i < size; i++){
        converted[i] = 0;
    }*/
    
       
    return;
}


void convertWhole(int toConvert, int* converted, int size){
    if (toConvert < 0){
        toConvert = -1 * toConvert;
    }
    int arrCounter = 0;
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
        if (toConvert % (1 << i) == toConvert){
            largestPow = i-1;
            break;
        }
    }

    for (int i = largestPow; i >=0; i--){ //loop through and try to divide by the largest power of 2 available
        if (toConvert % (1 << i) != toConvert && toConvert != 0){ //if successful put a 1 in the next position
            toConvert = toConvert - (1 << i);
            converted[arrCounter] = 1;
            arrCounter++;
        }
        else {
            converted[arrCounter] = 0;
            arrCounter++;
        }
    }
    /*for (int i = arrCounter; i < size; i++){
        converted[i] = 0;
    }*/
    
       
    return;
}
    
void convertFract(double toConvert, int* converted, int size){
    int power = -1;
    int arrCount = 0;
    for (int i = 0; i < size; i++){
        if (toConvert >= pow(2, power)){
            converted[arrCount] = 1;
            toConvert = toConvert - pow(2, power);
            power--;
            arrCount++;
            if (toConvert == 0){
                break;
            }
            
        }
        else{
            converted[arrCount] = 0;
            power--;
            arrCount++;
        }
    }
    /*for (int i = arrCount; i < size; i++){
        converted[i] = 0;
    }*/
    return;
}

void canonicalize(int* decimalPlaceCount, int* finArray, int* wholeBin, int* fractBin, int numBits, int size){
    int finArrayCounter = 0;
    //combine whole and fract
        if (wholeBin[0] != 0){
            for (int i = 0; i < size; i++){
                if (wholeBin[i] == -1){
                    break;
                }
                if (i >= (numBits)){
                    break;
                }
                else{
                    finArray[i] = wholeBin[i];
                    finArrayCounter++;
                }
            }

            //find where the decimal is
            for (int i = 0; i < size; i++){
                if (wholeBin[i] == -1){
                    break;
                }
                else (*decimalPlaceCount)++;
            }
            *decimalPlaceCount = *decimalPlaceCount - 1;
            int fractCount = 0;
            for (int i = finArrayCounter; i < numBits; i++){
                if (fractBin[fractCount] == -1){
                    break;
                }
                finArray[i] = fractBin[fractCount];
                finArrayCounter++;
                fractCount++;
            }
            if (finArrayCounter < numBits){
                for (int i = finArrayCounter; i < numBits; i++){
                    finArray[i] = 0;
                }
            }
        }
        //if whole is 0 find where the first 1 is
        else if (wholeBin[0] == 0){
            int fractCount = 0;
            for (int i = 0; i < numBits; i++){
                if (fractBin[i] == 1){
                    break;
                }
                else {
                    fractCount++;
                }
            }
            *decimalPlaceCount = *decimalPlaceCount - fractCount - 1;
        for (int i = finArrayCounter; i < numBits; i++){
            if (fractBin[fractCount] == -1){
                break;
            }
            finArray[i] = fractBin[fractCount];
            finArrayCounter++;
            fractCount++;
        }
        if (finArrayCounter < numBits){
            for (int i = finArrayCounter; i < numBits; i++){
                finArray[i] = 0;
            }
        }
    }
    
}

void roundFract(int* finArray, int fbits, int* rounded, int subNorm){
    //extract fraction bits of canoncalized num
    int fractBits[fbits+2]; //must be one more than the midpt to compare
    int fractCounter = 0;
    if (subNorm == 0){
        for (int i = 1; i < fbits + 3; i++){
            if (finArray[i] == -1){
                fractBits[fractCounter] = 0;
                fractCounter++;
            }
            else {
                fractBits[fractCounter] = finArray[i];
                fractCounter++;
            }
        }
    }
    else if (subNorm == 1){
        for (int i = 0; i < fbits + 2; i++){
            fractBits[fractCounter] = finArray[i];
            fractCounter++;
        }
    }
    //get v1 by truncating fractBits
    int v1[fbits];
    for (int i = 0; i < fbits; i++){
        v1[i] = fractBits[i];
    }
    //get v2 by copying v1 and adding 1 to it
    int v2[fbits];
    for (int i = 0; i < fbits; i++){
        v2[i] = v1[i];
    }
    v2[fbits-1] = v2[fbits-1] + 1;
    for (int i = fbits-1; i > 0; i--){
        if (v2[i] > 1){
            v2[i] = 0;
            v2[i-1] = v2[i-1] + 1;
        }
    }
    //get midpt by copying v1 and adding an additional 1 to the end
    int midpt[fbits + 1];
    for (int i = 0; i < fbits; i++){
        midpt[i] = v1[i];
    }
    midpt[fbits] = 1;

    //if fractBits is less than the midpt
    if (fractBits[fbits] < midpt[fbits]){
        for (int i = 0; i < fbits; i++){
            rounded[i] = v1[i];
        }
    }
    //if fractBits is greater than or equal to midpt
    else if (fractBits[fbits] == midpt[fbits]){
        //greater than the midpt
        if (fractBits[fbits+1] == 1){
            for (int i = 0; i < fbits; i++){
                rounded[i] = v2[i];
            }
        }
        //equal to midpt
        else if (fractBits[fbits+1] == 0){
            //find which one ends with 0 and set rounded to that value
            if (v1[fbits-1] == 0){
                for (int i = 0; i < fbits; i++){
                    rounded[i] = v1[i];
                }
            }
            else{
                for (int i = 0; i < fbits; i++){
                    rounded[i] = v2[i];
                }
            }
        }
    }
}

int main(int argc, char** argv){

    FILE* fp = fopen(argv[1], "r");
    double num;
    int fbits;
    int whole;
    int numBits;
    int expBits;
    while (fscanf(fp, "%lf ", &num) != EOF){
        fscanf(fp, "%d ", &numBits);
        fscanf(fp, "%d ", &expBits);
        fscanf(fp, "%d\n", &fbits);
        whole = num;
        double fraction = num - whole;
        int size = whole;
        if (whole == 0){
            size = 1;
        }
        if (whole < 0){
            size = whole * -1;
        }
        if (fraction < 0){
            fraction = fraction * -1;
        }
        int wholeBin[size];
        int fractBin[100];
        for (int i = 0; i < size; i++){
            wholeBin[i] = -1;
        }
        for (int i = 0; i < 100; i++){
            fractBin[i] = -1;
        }
        //convert whole and fraction to binary
        convertWhole(whole, wholeBin, size);
        convertFract(fraction, fractBin, 100);
        
        int decimalPlaceCount = 0;
        int finArray[100];

        canonicalize(&decimalPlaceCount, finArray, wholeBin, fractBin, 100, size);
        //convert to IEEE format
        int ieee[numBits];
        int iCounter = 0;
        int bias = (1 << (expBits-1)) - 1;
        int E = bias + decimalPlaceCount;
        int eBin[expBits];
        for (int i = 0; i < expBits; i++){
            eBin[i] = 0;
        }
        if (num > 0){
            ieee[0] = 0;
            iCounter++;
        }
        else{
            ieee[0] = 1;
            iCounter++;
        }
        convertE(E, eBin, expBits);
        
        //copy the exponent bits into final array
        for (int i = 0; i < expBits; i++){
            ieee[iCounter] = eBin[i];
            iCounter++;
        }
        int subnormalCheck = 1; //1 means it is subnormal
        for (int i = 1; i < expBits + 1; i++){
            if (ieee[i]  == 1){ //if 1 is detected in exponent field, it is not subnormal
                subnormalCheck = 0;
            }
        }
        int rounded[fbits];
        roundFract(finArray, fbits, rounded, subnormalCheck);
        //copy fraction bits into final array
        for (int i = 0; i < fbits; i++){
            ieee[iCounter] = rounded[i];
            iCounter++;
        }
        //check if there are any carries
        for (int i = numBits-1; i > 0; i--){
        if (ieee[i] > 1){
            ieee[i] = 0;
            ieee[i-1] = ieee[i-1] + 1;
        }
    }
        //print out final array
        for (int i = 0; i < numBits; i++){
            printf("%d", ieee[i]);
        }
        printf("\n");
    }
   
    return 0;
}