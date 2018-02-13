#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>

const char gPlaintext[] = "Implement the message digest algorithm MD5 and test how random the output appears.\
For example, test the percentage of 1 bits in the output, or test how many bits of output\
change with minor changes in the input. Also, design various simplifications of the\
message digest functions (such as reducing the number of rounds) and see how these\
change things.";

int main(int argc, char** argv)
{
    printf("Length: %d \nLength mod 512 : %d\n", strlen(gPlaintext), ((strlen(gPlaintext) * 8) % 512));
    uint8_t* pInputBlock = NULL;
    uint32_t nPaddingBits = 448 - ((strlen(gPlaintext) * 8) % 512);

    printf("nPaddingBits: %d pInputBlock length: %d\n", nPaddingBits, ((strlen(gPlaintext) + (nPaddingBits / 8))*8) % 512);
    if (0 < nPaddingBits)
    {
        printf("Creating the pad\n");
        pInputBlock = (uint8_t *)malloc(strlen(gPlaintext) + (nPaddingBits / 8) + 8);
        memcpy(pInputBlock, gPlaintext, strlen(gPlaintext));
        pInputBlock[strlen(gPlaintext)] = (char)'0x01';
        memset(pInputBlock + strlen(gPlaintext) + 1, '\0', (nPaddingBits / 8) - 1);
        uint64_t nInputBlock = strlen(gPlaintext);
        memcpy(pInputBlock + strlen(gPlaintext) + (nPaddingBits / 8), &nInputBlock, sizeof(nInputBlock));
    }
    printf("Pre-processed input:\n");
    for (uint32_t i = 0; i < (strlen(gPlaintext) + (nPaddingBits / 8) + 8); i++)
    {
        printf("%d", pInputBlock[i]);
    }
    printf("\n");
    free(pInputBlock);
    return 0;
}