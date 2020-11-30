#include <stdio.h>
#include <math.h>

#define MASK 2047		// 5 bits with the highest weight at "0" and the 11 significant bits at "1" (000001 ... 1) at 16 bits
#define VAL_BITS 11		// Define to set the limit of significant bits at 11 bits
#define VAL_32 2147483648		// Bit with the highest weight at "1" and the rest at "0" (1000 ... 0) at 32 bits
#define VAL_NEG 1024      	// Define to check if the number with 11 significant bits is negative (1000 ... 0)
#define VAL_MAXBITS 32		// Define to set the bit limit to 32 bits
#define LOW_MASK 1 		// Lowest weight bit at "1"

/*
 * 	Nane: encode
 * 	Return: (unsigned short) returns the number in a 32-bit parameter encoded in 16 bits, 
 *				if it is possible to do so (verification using the valid), in which the 
 *				5 bits with the greatest weight represent the shift to be made to the 
 *				left and the remaining 11 bits are the ones significant and contains the 
 *				encoded number
 */
unsigned short encode( int data, int *valid ) {
	int shift = 0;
	int highBits;
					
	while((highBits = data >> (VAL_BITS - 1)) != 0 && highBits != ~0) { 
		if((data & LOW_MASK) == 0) {                                      
			*valid = 1;
			data >>= 1;
			++shift;
		}
		
		else {
			*valid = 0;
			break;
		}
	}
	
	return (shift << VAL_BITS) | (data & ~( ~0 << VAL_BITS ));
}

/*
 *	Auxiliar Function
 * 	Name: findZeros
 * 	Return: (int) returns the number of zeros to the left of a number that has gone
 *				from 11 significant bits to 32 bits in order to fill everything with 
 *				"1" (the function will only be called if the number in 11 significant
 *				bits is, in fact, negative) 
 */
int findZeros(int num) {
    int mask = VAL_32;            
    int count = 0;
    
	while((~num & mask) == mask){
        mask = mask >> 1;
        count++;
    }
    
    return count;
}

/*
 * 	Name: decode
 * 	Return: (int) returns the number that was encoded in 16 bits (5 of the largest
 *				weight represent the shift to be made to the left and the remaining
 *				11 bits are the significant ones and contains the encoded number) 
 *				to 32 bits, with sign
 */
int decode(unsigned short code) {
	int numShifts = code >> VAL_BITS;		// Obtain the number of offsets present in the 5 highest weight bits
	
	int num = code & MASK;		// Get only the value present in the 11 significant bits
	
	int finalCode = num << numShifts;		// Positioning the significant part of the 11-bit number to 32 bits
	
	// Verification of whether the value is negative or not in order to be properly displayed in 32 bits, with a sign
	if((num & VAL_NEG) == VAL_NEG) { 
		int zeros = findZeros(finalCode); 
        unsigned long fill = 0;
        
        while (zeros > 0) {
            fill += pow(2 , (VAL_MAXBITS - zeros));
            zeros--;
        }
        
        return finalCode | fill;
	}
	
	return finalCode;
}

/*
 * 	Main test
 */
int main(int argc, char **argv){
	int num, valid;
	unsigned short code;
	
	// Entering a number via the console
	printf("Insira o número: ");
	scanf("%d", &num);
	
	// Number encoding
	code = encode(num, &valid);
	
	// Number verification (whether or not it is possible to encode it)
	if(valid == 0) {
		printf("Erro: O seguinte valor não é codificável -> %d\n", num);
		return -1;
	}
	
	// If valid, write the encoded number and then decrypt it
	printf("The number encoded in 16 bits, in hexadecimal, is: 0x%x\n", code);
	printf("The decoded number, 32 bits and signed, is: 0x%x\n", decode(code));
	
	return 0;
}

