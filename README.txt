When using 32-bit signed integers, values that do not need to occupy the 32 bits are frequent because they
have a bit sequence at zero at the bottom or a bit sequence with the sign value at the bottom high. 
Taking advantage of this observation, a compact 16-bit representation is proposed, with the following format:
- bits 0 to 10: encode the significant part of the value, at 11 bits, including the signal;
- bits 11 to 15: encode the shift, indicating the number of bits to shift to the left, from in order to position 
  the significant part in the 32-bit word.

There are values that can have several equivalent representations, as they have less than 11 significant bits.
There are also non-encodable values in this format, as they have more than 11 significant bits.

The encode function checks whether the variable data can be represented in 11 bits and, if it can,
performs the encoding of the data parameter and returns the respective code in 16 bits.

The decode function returns the signed 32-bit value, reproduced from the code passed in the code parameter.
