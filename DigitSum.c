//Dependencies
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
/*

Features of Implementation:

- int data type has been used in all variables, this is to reduce memory usage as values greater than 2147483647 are too large for this code to run efficiently therefore it isnt expected
  that users will wish to process numbers of this size.
- The code is implented to take arguments when the file DigitSum.c is called, this is because it is the most convenient way for input to be taken when executing this program in A Unix terminal
  as the user will only be required to enter one input when they use the program.
*/
int f(int n){
    //Declares an array to hold digits for each number and determines the largest value whos digits add to
    int *CurrentNum = malloc(n*sizeof(int));
    int solutionNum = pow(2,n-1)-1;
    int foundsolutions = 0;
    //Sum variable holds the result of the function
    int Sum = 0;
    int i = 0;

    while(foundsolutions < solutionNum){//for every number below the largest acceepted number
        //printf("%d\n", i);
        int nonzeroIn = 0;
        //determines if a 0 digit is present in the current number
        int zeroIn = 0;
        for(int j = n-1; j > -1 ; j--){
            if(CurrentNum[j] != 0){
                nonzeroIn =1;
            }
            if(CurrentNum[j] == 0 && nonzeroIn == 1){
                zeroIn = 1;
            }
        }

        if(zeroIn == 0){//If no zero digit is present
            
            int digitSum = 0;//Check if all digits add up to n
            for(int j = 0 ; j < n ; j++){
                digitSum += CurrentNum[j];
            }

            if(digitSum == n){//if digits add up to n
                Sum += i;//add this value to the result
                foundsolutions++;
            }
        }

        CurrentNum[0]++;//increment the unit column
        for(int j = 0 ; j < n-1 ; j++){//for every digit
            if(CurrentNum[j] == 10){//if digit == 10
                CurrentNum[j+1]++;//increment the next digit
                CurrentNum[j] = 0;//set this digits value to 0
            }
        }
        i++;
    }
    free(CurrentNum);
    int maxValue = 0;
    for(int i = 0; i < n ; i++){
        maxValue += 1*pow(10,i);
    }
    
    return Sum+maxValue;
}

int main(int argc, const char * argv[]){
    int n = atoi(argv[1]);
    printf("%d \n",f(n));
}