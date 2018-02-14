#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>

#define SAFE_DELETE(x) do{\
    if(NULL != x){ free(x); x = NULL; }\
}while(0)

#define F(X,Y,Z) ((X & Y) | (~(X) & Z))
#define G(X,Y,Z) ((X & Z) | (Y & ~(Z)))
#define H(X,Y,Z) (X ^ Y ^ Z)
#define I(X,Y,Z) (Y ^ (X | ~(Z)))

#define ROTATE_LEFT(x,n) do{\
    ((x << n) | (x >> (32-n)));\
}while(0)

#define FF(a,b,c,d,x,t,s) do{\
    a += F(b,c,d) + x + t;\
    ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define GG(a,b,c,d,x,t,s) do{\
    a += G(b,c,d) + x + t;\
    ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define HH(a,b,c,d,x,t,s) do{\
    a += H(b,c,d) + x + t;\
    ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define II(a,b,c,d,x,t,s) do{\
    a += I(b,c,d) + x + t;\
    ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

const char gPlaintext[] = "Implement the message digest algorithm MD5 and test how random the output appears.\
For example, test the percentage of 1 bits in the output, or test how many bits of output\
change with minor changes in the input. Also, design various simplifications of the\
message digest functions (such as reducing the number of rounds) and see how these\
change things.";

typedef struct
{
    uint8_t* data;
    uint64_t length;
}buffer_t;

int gMD5Registers[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

buffer_t* MD5Init(const uint8_t* xInputData)
{
    buffer_t* pBuffer = NULL;
    if (xInputData)
    {
        uint32_t nPaddingBits = 0;
        pBuffer = (buffer_t *)malloc(sizeof(buffer_t));
        pBuffer->length = strlen(xInputData);
        uint32_t mod = ((strlen(xInputData) * 8) % 512);
        if (mod > 448)
        {
            nPaddingBits = 448 + (512 - mod);
        }
        else
        {
            nPaddingBits = 448 - mod;
        }

        if (0 < nPaddingBits)
        {
            printf("Creating the pad\n");
            pBuffer->data = (uint8_t *)malloc(strlen(xInputData) + (nPaddingBits / 8) + 8);
            if (NULL != pBuffer->data)
            {
                pBuffer->length = strlen(xInputData) + (nPaddingBits / 8) + 8;
            }
            memcpy(pBuffer->data, xInputData, strlen(xInputData));
            pBuffer->data[strlen(xInputData)] = (char)'0x01';
            memset(pBuffer->data + strlen(xInputData) + 1, '\0', (nPaddingBits / 8) - 1);
            uint64_t nInputLength = strlen(xInputData);
            memcpy(pBuffer->data + pBuffer->length - 8, &nInputLength, sizeof(nInputLength));
        }
    }
    
    return pBuffer;
}

int main(int argc, char** argv)
{
    buffer_t* pBuffer = MD5Init(gPlaintext);
    if (pBuffer)
    {
        printf("Pre-processed input:\n");
        for (uint32_t i = 0; i < pBuffer->length; i++)
        {
            printf("%d", pBuffer->data[i]);
        }
        printf("\n");
    }
    
    SAFE_DELETE(pBuffer);
    return 0;
}