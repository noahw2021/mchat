//
//  main.c
//  test
//
//  Created by Noah Wooten on 1/23/24.
//

#include <stdio.h>
#include <wchar.h>

int main(int argc, const char * argv[]) {
    long InputVariable = 99;
        
    long InputCents_Quarters = InputVariable / 25;
    long InputCents_Dimes = (InputVariable %= 25) / 10;
    long InputCents_Nickels = (InputVariable %= 10) / 5;
    long InputCents_Pennies = (InputVariable %= 5) / 1;;
        
    wprintf(L"Quarters: %02i - %.02fc\n", InputCents_Quarters,
        InputCents_Quarters * 0.25f);
    wprintf(L"Dimes:    %02i - %.02fc\n", InputCents_Dimes,
        InputCents_Dimes * 0.10f);
    wprintf(L"Nickels:  %02i - %.02fc\n", InputCents_Nickels,
        InputCents_Nickels * 0.05f);
    wprintf(L"Pennies:  %02i - %.02fc\n", InputCents_Pennies,
        InputCents_Pennies * 0.01f);
        
    return 0;
}
