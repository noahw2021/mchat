//
//  main.c
//  prove
//
//  Created by Noah Wooten on 2/6/24.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int __attribute__((noinline)) IsUppercase(char Char) {
    return (Char & 0x20) ? 1 : 0;
}

int main(int argc, const char * argv[]) {

    /*
     only uppercase
     every 2nd letter
     vowels replaced by underscore
     vowel count
     */
    
    
    /*
     My method: 716 bytes instruction
     Stack: 40 bytes
     Heap: 179 bytes
     Total: 935 bytes
     */
    
    char* Input = malloc(128);
    printf("Please input a string: ");
    fgets(Input, 128, stdin);
    
    // my method
    char* OutputUpper = malloc(strlen(Input) + 1);
    char* Output2ndLtr = malloc(strlen(Input) + 1);
    char* OutputVowels = malloc(strlen(Input) + 1);
    strcpy(OutputVowels, Input);
    int Output_NumVowels = 0;
    int* Output_VowelLocations = malloc(strlen(Input) * sizeof(int));
    unsigned long InputLength = strlen(Input);
    char TempStr[2] = {0, 0};
    
    for (int i = 0; i < InputLength; i++) {
        char ThisChar = Input[i];
        
        if (ThisChar & 0x20) {
            TempStr[0] = ThisChar;
            strcat(OutputUpper, TempStr);
        }
        
        if ((i & 1)) {
            TempStr[0] = ThisChar;
            strcat(Output2ndLtr, TempStr);
        }
        
        if (Input[i] == 'a' || Input[i] == 'e' || Input[i] == 'i' || Input[i] == 'o' || Input[i] == 'u') {
            OutputVowels[i] = '_';
            Output_VowelLocations[Output_NumVowels] = i;
            Output_NumVowels++;
        }
    }
    
    printf("Only Upper: %s\n", OutputUpper);
    printf("Only 2nd Ltr: %s\n", Output2ndLtr);
    printf("Only Vowels: %s\n", OutputVowels);
    printf("Number Vowels: %i\n", Output_NumVowels);
    for (int i = 0; i < Output_NumVowels; i++)
        printf("%i ", Output_VowelLocations[i]);
    
    /* Your Method: 1,192 bytes
       Stack: 28 bytes
       Heap: 128 bytes
       Total: 1,348 bytes
     */
    
    // the method
    unsigned long StringLength = strlen(Input);
    
    // only uppercase
    printf("Only Uppercase: ");
    for (int i = 0; i < StringLength; i++) {
        if (IsUppercase(Input[i]))
            fputc(Input[i], stdout);
    }
    
    printf("\nEvery 2nd letter: ");
    // every 2nd letter
    for (int i = 0; i < StringLength; i++) {
        if (i & 1)
            fputc(Input[i], stdout);
    }
    
    printf("\nVowels replaced with underscore: ");
    // vowels replaced by underscore
    for (int i = 0; i < StringLength; i++) {
        if (Input[i] == 'a' || Input[i] == 'e' || Input[i] == 'i' || Input[i] == 'o' || Input[i] == 'u')
            fputc(Input[i], stdout);
    }
    
    // vowel count
    int VowelCount = 0;
    for (int i = 0; i < StringLength; i++) {
        if (Input[i] == 'a' || Input[i] == 'e' || Input[i] == 'i' || Input[i] == 'o' || Input[i] == 'u')
            VowelCount++;
    }
    printf("\nVowel count: %i\n", VowelCount);
    
    // vowel location
    for (int i = 0; i < StringLength; i++) {
        if (Input[i] == 'a' || Input[i] == 'e' || Input[i] == 'i' || Input[i] == 'o' || Input[i] == 'u') {
            printf("%i ", i);
        }
    }
    printf("\n");
    
    return 0;
}
