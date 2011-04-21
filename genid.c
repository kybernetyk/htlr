//
//  main.c
//  superID
//
//  Created by Daniel on 03.08.10.
//  Copyright (c) 2010 __MyCompanyName__. All rights reserved.
//

// mapping:  6 9 31 8 27 18 11 25 29 16 30 15 14 20 28 7 10 2 23 17 4 0 24 5 3 12 22 19 1 13 21 26

#include <stdio.h>
#include <stdlib.h>

int mapping[32] =
        { 6, 9, 31, 8, 27, 18, 11, 25, 29, 16, 30, 15, 14, 20, 28, 7, 10, 2, 23 ,17, 4, 0, 24, 5, 3, 12, 22, 19, 1, 13, 26, 21 };

char baseX[36] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
   // 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
   // '$', '-', '_', '.', '!', '*', '\'', '(', ')', ','
};

unsigned int parameter;
unsigned int output = 0;


int main (int argc, const char * argv[]) {
    
    if (argc != 2) {
        printf("bai");
        return -1;
    }

    parameter = atoi(argv[1]);
    
    // remap bits. this is bit 6 in output is set to bit 31 in input and so on
	int i = 0;
    for (i = 0; i < 32; i++) {
        if (parameter & (1 << i)) {
            output |= 1 << mapping[i];
//            printf("%c", '1');
        } //else
//            printf("%c", '0');
    }
    
//    printf("\r\n\r\n");

/*    
    for (int i = 0; i < 32; i++) {
        if (output & (1 << i)) {
            printf("%c", '1');
        } else
            printf("%c", '0');
    }
    
    printf("\r\n\r\n");
 */

	int size = 32;
	
	while (((output & 63) == 0) && size > 6) {
		output = output >> 6;
		size-=6;
	}
    
    while (size > 0) {
        int tmp = output & 63;
        int schubs = tmp < 36;

        size -= schubs ? 6 : 5;
        
        printf("%c", schubs ? baseX[tmp] : baseX[tmp & 31]);
        
        output = schubs ? output >> 6 : output >> 5;
    }

//    printf("\r\n\r\n");
    
    return 0;
}
