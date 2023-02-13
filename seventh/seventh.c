#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

void convert(int toConvert, int numBits, int min){
    int output[numBits];
    int arrCounter = 0;
    if (toConvert == 0){
        for (int i = 0; i < numBits; i++){
            output[i] = 0;
        }
    }
    //if the number is positive
    else if (toConvert > 0){
        output[0] = 0;
        arrCounter++;
        for (int i = numBits - 2; i > 0; i--){ //loop through and try to divide by the largest power of 2 available
            if (toConvert % (1 << i) != toConvert && toConvert != 0){ //if successful put a 1 in the next position
                toConvert = toConvert - (1 << i);
                output[arrCounter] = 1;
                arrCounter++;
            }
            else {
                output[arrCounter] = 0;
                arrCounter++;
            }
        }
       //check if 2^0 is left at the end
        if (toConvert == 1){
            output[arrCounter] = 1;
        }
        else {
            output[arrCounter] = 0;
        }
    }
    else if (toConvert == min){
        output[0] = 1;
        for (int i = 1; i < numBits; i++){
            output[i] = 0;
        }
    }
    else if (toConvert < 0){
        //convert number to positive binary
        toConvert = toConvert * -1;
        output[0] = 0;
        arrCounter++;
        for (int i = numBits - 2; i > 0; i--){ //loop through and try to divide by the largest power of 2 available
            if (toConvert % (1 << i) != toConvert && toConvert != 0){ //if successful put a 1 in the next position
                toConvert = toConvert - (1 << i);
                output[arrCounter] = 1;
                arrCounter++;
            }
            else {
                output[arrCounter] = 0;
                arrCounter++;
            }
        }
       //check if 2^0 is left at the end
        if (toConvert == 1){
            output[arrCounter] = 1;
        }
        else {
            output[arrCounter] = 0;
        }
        //flip all of the bits
       for (int i = 0; i < numBits; i++){
            if (output[i] == 0){
                output[i] = 1;
            }
            else if (output[i] == 1){
                output[i] = 0;
            }
        }
        //add 1
        output[arrCounter] = output[arrCounter] + 1;
        for (int i = arrCounter; i > 0; i--){
            if (output[i] > 1){
                output[i] = 0;
                output[i-1] = output[i-1] + 1;
            }
        }
    }

    for (int i = 0; i < numBits; i++){
        printf("%d", output[i]);
    }
    printf("\n");
}

int main(int argc, char** argv){

    FILE* fp = fopen(argv[1], "r");
    double num;
    int bits;
    while (fscanf(fp, "%lf ", &num) != EOF){
        fscanf(fp, "%d\n", &bits);
        int max = (1 << (bits-1)) - 1;
        int min = -(1 << (bits-1));
        if (num > max || num > INT_MAX){
            convert(max, bits, min);
        }
        else if (num < min){
            convert(min, bits, min);
        }
        else{
            int intNum = num;
            convert(intNum, bits, min);
        }
    }
    
    return 0;
}