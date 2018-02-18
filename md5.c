#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define SAFE_DELETE(x) do{\
    if(NULL != x){ free(x); x = NULL; }\
}while(0)

#define F(X,Y,Z) ((X & Y) | ((~X) & Z))
#define G(X,Y,Z) ((X & Z) | (Y & (~Z)))
#define H(X,Y,Z) (X ^ Y ^ Z)
#define I(X,Y,Z) (Y ^ (X | (~Z)))

#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))

#define FF(a,b,c,d,x,s,t) do{\
    a += F(b,c,d) + x + t;\
    a = ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define GG(a,b,c,d,x,s,t) do{\
    a += G(b,c,d) + x + t;\
    a = ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define HH(a,b,c,d,x,s,t) do{\
    a += H(b,c,d) + x + t;\
    a = ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define II(a,b,c,d,x,s,t) do{\
    a += I(b,c,d) + x + t;\
    a = ROTATE_LEFT(a,s);\
    a += b;\
}while(0)

#define PRINT_MD(x) do{\
    for (uint32_t k = 0; k < 16; k++) { printf("%02X", x[k]); } printf("\n"); \
}while(0)

const char gPlaintext[] = "";

const char gPlaintext2[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijkl";

const char gPlaintext3[] = "Implement the message digest algorithm MD5 and test how random the output appears. \
For example, test the percentage of 1 bits in the output, or test how many bits of output \
change with minor changes in the input. Also, design various simplifications of the \
message digest functions (such as reducing the number of rounds) and see how these \
change things.";

const uint8_t PADDING[64] = { 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const uint8_t MD5REGISTERS[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                                0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

uint32_t gLookupTable[64];

void MD5CreateLookUpTable()
{
    for (int i = 1; i <= 64; i++)
    {
        gLookupTable[i - 1] = (uint32_t)(fabs(sin(i))*pow(2, 32));
    }
}

void MD5_digesttostate(uint32_t* dst, uint8_t* src)
{
    for (int i = 0; i < 4; i++)
    {
        dst[i] = ((src[i*4 + 3] & 0xff) << 24) | ((src[i*4 + 2] & 0xff) << 16) | ((src[i*4 + 1] & 0xff) << 8) | (src[i*4 + 0] & 0xff);
    }
}

void MD5_statetodigest(uint8_t* dst, uint32_t* src, uint32_t len)
{
    for (int i = 0, j = 0; j < len; i++, j=i*4)
    {
        dst[j + 3] = ((src[i] >> 24) & 0xff);
        dst[j + 2] = ((src[i] >> 16) & 0xff);
        dst[j + 1] = ((src[i] >> 8) & 0xff);
        dst[j + 0] = (src[i] & 0xff);
    }
}

void MD5_chartoint(uint32_t* dst, uint8_t* src)
{
    *dst = ((src[3] & 0xff) << 24) | ((src[2] & 0xff) << 16) | ((src[1] & 0xff) << 8) | (src[0] & 0xff);
}

void MD5Transform(uint8_t* md, const uint8_t* xpPlaintext, uint32_t len)
{
    uint32_t state[4];
    MD5_digesttostate(state, md);

    for (int i = 0; i < len; i++)
    {
        uint32_t X[16];
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];

        for (int j = 0; j < 16; j++)
        {
            MD5_chartoint(&X[j], xpPlaintext + i * 64 + j * 4);
        }
        /* Round 1 */
        FF(a, b, c, d, X[0], 7, gLookupTable[0]);
        FF(d, a, b, c, X[1], 12, gLookupTable[1]);
        FF(c, d, a, b, X[2], 17, gLookupTable[2]);
        FF(b, c, d, a, X[3], 22, gLookupTable[3]);
        FF(a, b, c, d, X[4], 7, gLookupTable[4]);
        FF(d, a, b, c, X[5], 12, gLookupTable[5]);
        FF(c, d, a, b, X[6], 17, gLookupTable[6]);
        FF(b, c, d, a, X[7], 22, gLookupTable[7]);
        FF(a, b, c, d, X[8], 7, gLookupTable[8]);
        FF(d, a, b, c, X[9], 12, gLookupTable[9]);
        FF(c, d, a, b, X[10], 17, gLookupTable[10]);
        FF(b, c, d, a, X[11], 22, gLookupTable[11]);
        FF(a, b, c, d, X[12], 7, gLookupTable[12]);
        FF(d, a, b, c, X[13], 12, gLookupTable[13]);
        FF(c, d, a, b, X[14], 17, gLookupTable[14]);
        FF(b, c, d, a, X[15], 22, gLookupTable[15]);

        /* Round 2 */
        GG(a, b, c, d, X[1], 5, gLookupTable[16]);
        GG(d, a, b, c, X[6], 9, gLookupTable[17]);
        GG(c, d, a, b, X[11], 14, gLookupTable[18]);
        GG(b, c, d, a, X[0], 20, gLookupTable[19]);
        GG(a, b, c, d, X[5], 5, gLookupTable[20]);
        GG(d, a, b, c, X[10], 9, gLookupTable[21]);
        GG(c, d, a, b, X[15], 14, gLookupTable[22]);
        GG(b, c, d, a, X[4], 20, gLookupTable[23]);
        GG(a, b, c, d, X[9], 5, gLookupTable[24]);
        GG(d, a, b, c, X[14], 9, gLookupTable[25]);
        GG(c, d, a, b, X[3], 14, gLookupTable[26]);
        GG(b, c, d, a, X[8], 20, gLookupTable[27]);
        GG(a, b, c, d, X[13], 5, gLookupTable[28]);
        GG(d, a, b, c, X[2], 9, gLookupTable[29]);
        GG(c, d, a, b, X[7], 14, gLookupTable[30]);
        GG(b, c, d, a, X[12], 20, gLookupTable[31]);

        /* Round 3 */
        HH(a, b, c, d, X[5], 4, gLookupTable[32]);
        HH(d, a, b, c, X[8], 11, gLookupTable[33]);
        HH(c, d, a, b, X[11], 16, gLookupTable[34]);
        HH(b, c, d, a, X[14], 23, gLookupTable[35]);
        HH(a, b, c, d, X[1], 4, gLookupTable[36]);
        HH(d, a, b, c, X[4], 11, gLookupTable[37]);
        HH(c, d, a, b, X[7], 16, gLookupTable[38]);
        HH(b, c, d, a, X[10], 23, gLookupTable[39]);
        HH(a, b, c, d, X[13], 4, gLookupTable[40]);
        HH(d, a, b, c, X[0], 11, gLookupTable[41]);
        HH(c, d, a, b, X[3], 16, gLookupTable[42]);
        HH(b, c, d, a, X[6], 23, gLookupTable[43]);
        HH(a, b, c, d, X[9], 4, gLookupTable[44]);
        HH(d, a, b, c, X[12], 11, gLookupTable[45]);
        HH(c, d, a, b, X[15], 16, gLookupTable[46]);
        HH(b, c, d, a, X[2], 23, gLookupTable[47]);

        /* Round 4 */
        II(a, b, c, d, X[0], 6, gLookupTable[48]);
        II(d, a, b, c, X[7], 10, gLookupTable[49]);
        II(c, d, a, b, X[14], 15, gLookupTable[50]);
        II(b, c, d, a, X[5], 21, gLookupTable[51]);
        II(a, b, c, d, X[12], 6, gLookupTable[52]);
        II(d, a, b, c, X[3], 10, gLookupTable[53]);
        II(c, d, a, b, X[10], 15, gLookupTable[54]);
        II(b, c, d, a, X[1], 21, gLookupTable[55]);
        II(a, b, c, d, X[8], 6, gLookupTable[56]);
        II(d, a, b, c, X[15], 10, gLookupTable[57]);
        II(c, d, a, b, X[6], 15, gLookupTable[58]);
        II(b, c, d, a, X[13], 21, gLookupTable[59]);
        II(a, b, c, d, X[4], 6, gLookupTable[60]);
        II(d, a, b, c, X[11], 10, gLookupTable[61]);
        II(c, d, a, b, X[2], 15, gLookupTable[62]);
        II(b, c, d, a, X[9], 21, gLookupTable[63]);

        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    }
    MD5_statetodigest(md, state, 16);
}

int main(int argc, char** argv)
{
    uint8_t digest[16];
    uint8_t* messageString = gPlaintext3;
    uint32_t unpaddedLength = strlen(messageString);
    MD5CreateLookUpTable();
    
    /*Initialize message digest*/
    memcpy(digest, MD5REGISTERS, sizeof(MD5REGISTERS));

    /* MD5Transform full blocks of message */
    MD5Transform(digest, messageString, unpaddedLength / 64);

    /* MD5Transform remaining bits */
    /* Create the Pad */
    uint8_t paddedString[128];
    uint32_t mod = unpaddedLength % 64;
    memcpy(paddedString, messageString + (64 * (unpaddedLength / 64)), mod);

    uint32_t nPadding = (mod > 64) ? (56 + (64 - mod)) : (56 - mod);
    memcpy(paddedString + mod, PADDING, nPadding);

    /* Append the length in *bits* */
    uint64_t len = strlen(messageString) * 8;
    uint8_t lenBuf[8];
    MD5_statetodigest(lenBuf, &len, 8);

    memcpy(paddedString + mod + nPadding, lenBuf, sizeof(uint64_t));

    MD5Transform(digest, paddedString, (mod + nPadding + 8) / 64);
    printf("MD5[%s]:\n", messageString);
    PRINT_MD(digest);
    printf("\n");

    return 0;
}