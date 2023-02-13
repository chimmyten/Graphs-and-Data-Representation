#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

void convertWhole(int toConvert, int* converted){
    int arrCounter = 0;
    int largestPow = -1;
    if (toConvert == 0){
        converted[0] = 0;
        return;
    }
    if (toConvert == 1){
        converted[0] = 1;
        return;
    }
    if (toConvert == 2){
        converted[0] = 1;
        converted[1] = 0;
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
       
    return;
}
    
void convertFract(double toConvert, int* converted, int size){
    int power = -1;
    int arrCount = 0;
    for (int i = 0; i < size; i++){
        if (toConvert >= pow(2, power)){
            converted[arrCount] = 1;
            toConvert = toConvert - pow(2, power);
            if (toConvert == 0){
                break;
            }
            power--;
            arrCount++;
        }
        else{
            converted[arrCount] = 0;
            power--;
            arrCount++;
        }
    }
    return;
}


int main(int argc, char** argv){

    FILE* fp = fopen(argv[1], "r");
    double num;
    int decPlaces;
    int whole;
    while (fscanf(fp, "%lf ", &num) != EOF){
        fscanf(fp, "%d\n", &decPlaces);
        whole = num;
        double fraction = num - whole;
        int size = whole;
        if (whole == 0){
            size = 1;
        }
        int wholeBin[size];
        int fractBin[decPlaces];
        for (int i = 0; i < size; i++){
            wholeBin[i] = -1;
        }
        for (int i = 0; i < decPlaces; i++){
            fractBin[i] = -1;
        }
        //convert whole and fraction to binary
        convertWhole(whole, wholeBin);
        convertFract(fraction, fractBin, decPlaces);
        
        int finArrayCounter = 0;
        int decimalPlaceCount = 0;
        int finArray[decPlaces + 1];

        //combine whole and fract
        if (wholeBin[0] != 0){
            for (int i = 0; i < size; i++){
                if (wholeBin[i] == -1){
                    break;
                }
                if (i >= (decPlaces + 1)){
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
                else decimalPlaceCount++;
            }
            decimalPlaceCount = decimalPlaceCount - 1;
            int fractCount = 0;
            for (int i = finArrayCounter; i < decPlaces + 1; i++){
                if (fractBin[fractCount] == -1){
                    break;
                }
                finArray[i] = fractBin[fractCount];
                finArrayCounter++;
                fractCount++;
            }
            if (finArrayCounter < decPlaces + 1){
                for (int i = finArrayCounter; i < decPlaces + 1; i++){
                    finArray[i] = 0;
                }
            }
        }
        //if whole is 0 find where the first 1 is
        else if (wholeBin[0] == 0){
            int fractCount = 0;
            for (int i = 0; i < decPlaces; i++){
                if (fractBin[i] == 1){
                    break;
                }
                else {
                    fractCount++;
                }
            }
            decimalPlaceCount = decimalPlaceCount - fractCount - 1;
        for (int i = finArrayCounter; i < decPlaces; i++){
            if (fractCount >= decPlaces || fractBin[fractCount] == -1){
                break;
            }
            finArray[i] = fractBin[fractCount];
            finArrayCounter++;
            fractCount++;
        }
        if (finArrayCounter < decPlaces + 1){
            for (int i = finArrayCounter; i < decPlaces + 1; i++){
                finArray[i] = 0;
            }
        }
        }
        //print final number
        printf("%d.", finArray[0]);
        for (int i = 1; i < decPlaces + 1; i++){
            printf("%d", finArray[i]);
        }
        printf(" %d\n", decimalPlaceCount);
    }
   
    return 0;
}