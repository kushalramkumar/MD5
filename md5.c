#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

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

#define FF(a,b,c,d,x,s,t) do{\
    a += F(b,c,d) + x + t;\
    ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define GG(a,b,c,d,x,s,t) do{\
    a += G(b,c,d) + x + t;\
    ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define HH(a,b,c,d,x,s,t) do{\
    a += H(b,c,d) + x + t;\
    ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define II(a,b,c,d,x,s,t) do{\
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
uint32_t gLookupTable[64];
void MD5CreateLookUpTable()
{
    for (int i = 1; i <= 64; i++)
    {
        gLookupTable[i - 1] = (uint32_t)(fabs(sin(i))*pow(2, 32));
    }
}

buffer_t* MD5Init(const uint8_t* xInputData)
{
    buffer_t* pBuffer = NULL;
    MD5CreateLookUpTable();
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

void MD5Transform(buffer_t* xpPlaintext)
{
    unsigned char MD[16];
    uint32_t a = gMD5Registers[0], b = gMD5Registers[1], c = gMD5Registers[2], d = gMD5Registers[3];

    for (int i = 0; i < ((xpPlaintext->length) / 16) - 1; i++)
    {
        uint32_t X[16];
        uint32_t aa = a, bb = b, cc = c, dd = d;

        for (int j = 0; j < 16; j++)
        {
            memcpy(&X[j], xpPlaintext->data + i * 16, 4);
        }
        /* Round 1 */
        FF(a, b, c, d, 0, 7, gLookupTable[0]);
        FF(d, a, b, c, 1, 12, gLookupTable[1]);
        FF(c, d, a, b, 2, 17, gLookupTable[2]);
        FF(b, c, d, a, 3, 22, gLookupTable[3]);
        FF(a, b, c, d, 4, 7, gLookupTable[4]);
        FF(d, a, b, c, 5, 12, gLookupTable[5]);
        FF(c, d, a, b, 6, 17, gLookupTable[6]);
        FF(b, c, d, a, 7, 22, gLookupTable[7]);
        FF(a, b, c, d, 8, 7, gLookupTable[8]);
        FF(d, a, b, c, 9, 12, gLookupTable[9]);
        FF(c, d, a, b, 10, 17, gLookupTable[10]);
        FF(b, c, d, a, 11, 22, gLookupTable[11]);
        FF(a, b, c, d, 12, 7, gLookupTable[12]);
        FF(d, a, b, c, 13, 12, gLookupTable[13]);
        FF(c, d, a, b, 14, 17, gLookupTable[14]);
        FF(b, c, d, a, 15, 22, gLookupTable[15]);

        /* Round 2 */
        GG(a, b, c, d, 1, 5, gLookupTable[16]);
        GG(d, a, b, c, 6, 9, gLookupTable[17]);
        GG(c, d, a, b, 11, 14, gLookupTable[18]);
        GG(b, c, d, a, 0, 20, gLookupTable[19]);
        GG(a, b, c, d, 5, 5, gLookupTable[20]);
        GG(d, a, b, c, 10, 9, gLookupTable[21]);
        GG(c, d, a, b, 15, 14, gLookupTable[22]);
        GG(b, c, d, a, 4, 20, gLookupTable[23]);
        GG(a, b, c, d, 9, 5, gLookupTable[24]);
        GG(d, a, b, c, 14, 9, gLookupTable[25]);
        GG(c, d, a, b, 3, 14, gLookupTable[26]);
        GG(b, c, d, a, 8, 20, gLookupTable[27]);
        GG(a, b, c, d, 13, 5, gLookupTable[28]);
        GG(d, a, b, c, 2, 9, gLookupTable[29]);
        GG(c, d, a, b, 7, 14, gLookupTable[30]);
        GG(b, c, d, a, 12, 20, gLookupTable[31]);

        /* Round 3 */
        HH(a, b, c, d, 5, 4, gLookupTable[32]);
        HH(d, a, b, c, 8, 11, gLookupTable[33]);
        HH(c, d, a, b, 11, 16, gLookupTable[34]);
        HH(b, c, d, a, 14, 23, gLookupTable[35]);
        HH(a, b, c, d, 1, 4, gLookupTable[36]);
        HH(d, a, b, c, 4, 11, gLookupTable[37]);
        HH(c, d, a, b, 7, 16, gLookupTable[38]);
        HH(b, c, d, a, 10, 23, gLookupTable[39]);
        HH(a, b, c, d, 13, 4, gLookupTable[40]);
        HH(d, a, b, c, 0, 11, gLookupTable[41]);
        HH(c, d, a, b, 3, 16, gLookupTable[42]);
        HH(b, c, d, a, 6, 23, gLookupTable[43]);
        HH(a, b, c, d, 9, 4, gLookupTable[44]);
        HH(d, a, b, c, 12, 11, gLookupTable[45]);
        HH(c, d, a, b, 15, 16, gLookupTable[46]);
        HH(b, c, d, a, 2, 23, gLookupTable[47]);

        /* Round 4 */
        II(a, b, c, d, 0, 6, gLookupTable[48]);
        II(d, a, b, c, 7, 10, gLookupTable[49]);
        II(c, d, a, b, 14, 15, gLookupTable[50]);
        II(b, c, d, a, 5, 21, gLookupTable[51]);
        II(a, b, c, d, 12, 6, gLookupTable[52]);
        II(d, a, b, c, 3, 10, gLookupTable[53]);
        II(c, d, a, b, 10, 15, gLookupTable[54]);
        II(b, c, d, a, 1, 21, gLookupTable[55]);
        II(a, b, c, d, 8, 6, gLookupTable[56]);
        II(d, a, b, c, 15, 10, gLookupTable[57]);
        II(c, d, a, b, 6, 15, gLookupTable[58]);
        II(b, c, d, a, 13, 21, gLookupTable[59]);
        II(a, b, c, d, 4, 6, gLookupTable[60]);
        II(d, a, b, c, 11, 10, gLookupTable[61]);
        II(c, d, a, b, 2, 15, gLookupTable[62]);
        II(b, c, d, a, 9, 21, gLookupTable[63]);

        a += aa; b += bb; c += cc; d += dd;
    }
    memcpy(MD, &a, 4); memcpy(MD+4, &b, 4); memcpy(MD+8, &c, 4); memcpy(MD+12, &d, 4);
    printf("a: %04X\n", a);
    printf("b: %04X\n", b);
    printf("c: %04X\n", c);
    printf("d: %04X\n", d);
    for (uint32_t k = 0; k < 16; k++)
    {
        printf("%02X", MD[k]);
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    buffer_t* pBuffer = MD5Init(gPlaintext);
    MD5Transform(pBuffer);
    SAFE_DELETE(pBuffer);
    return 0;
}