#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MD5_NOMINAL_MODE 15

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

void MD5Transform(uint8_t* md, const uint8_t* xpPlaintext, uint32_t len, uint32_t xOpt)
{
    uint32_t state[4];
    MD5_digesttostate(state, md);

    for (uint32_t i = 0; i < len; i++)
    {
        uint32_t X[16];
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];

        for (uint32_t j = 0; j < 16; j++)
        {
            MD5_chartoint(&X[j], xpPlaintext + i * 64 + j * 4);
        }

        /* Round 1 */
        if (xOpt & 1)
        {
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
        }
        
       /* Round 2 */
        if (xOpt & 2)
        {
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
        }

        /* Round 3 */
        if (xOpt & 4)
        {
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
        }
        /* Round 4 */
        if (xOpt & 8)
        {
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
        }
        
        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    }
    MD5_statetodigest(md, state, 16);
}

uint32_t NumberOfOnes(uint8_t* xDigest, uint32_t xLength)
{
    uint32_t nOnes = 0;
    for (int i = 0; i < xLength; i++)
    {
        uint8_t value = xDigest[i];
        for (int j = 0; j < 8; j++)
        {
            nOnes += value & 1;
            value >>= 1;
        }
    }
    return nOnes;
}

/*
 *  FREQUENCY TEST:
 *  Simple test of the percentage of 1's in the message digest.
 *  The expected nOnes ~ nZeroes, which means the percentage of 
 *  1's ~ 50%. The range of values from 0.4 - 0.6 are accepted.
 */
double Frequency_Test(uint8_t *xDigest, uint32_t xLength)
{
    uint32_t nOnes = NumberOfOnes(xDigest, xLength);
    
    return ((double)nOnes / (xLength * 8));
}

/*
 *  CHI SQUARE TEST:
 *  Since we're dealing with binary data, the number of outcomes
 *  is 2, the outcome we're observing is either the occurence of
 *  bit 1 or the bit 0, which means the degrees of freedom = 2-1 = 1.
 *  A Chi Square distribution with 1 degree of freedom with 95%
 *  certainity has a critical value of  3.841. If the Chi Square
 *  value obtained > 3.841, the data fails the statistical test for
 *  randomness.
 *  Reference: Handbook of Applied Cryptography, �5.4.4
 */
double ChiSquare_Test(uint8_t *xDigest, uint32_t xLength)
{
    uint32_t nOnes = NumberOfOnes(xDigest, xLength);
    uint32_t nZeroes = (xLength * 8) - nOnes;

    return (pow((abs(nZeroes - nOnes)), 2)) / (xLength*8);
}

/*
 *  MONOBIT TEST:
 *  The number of 1's and 0's are expected to be equal for a random
 *  sequence. The Monobit test check if the P-value obtained is >= 0.01.
 *  Reference: https://nvlpubs.nist.gov/nistpubs/legacy/sp/nistspecialpublication800-22r1a.pdf
 */
double Monobit_Test(uint8_t *xDigest, uint32_t xLength)
{
    int32_t count = 0;
    double sobs = 0.0, pVal = 0.0;
    for (int i = 0; i < xLength; i++)
    {
        uint8_t value = xDigest[i];
        for (int j = 0; j < 8; j++)
        {
            (value & 1) ? (count += 1) : (count -= 1);
            value >>= 1;
        }
    }

    sobs = ((double)abs(count) / (sqrt(xLength * 8)));
    pVal = erfc(fabs(sobs) / sqrt(2));

    return pVal;
}

uint32_t BitwiseDifference(uint8_t* xMD1, uint8_t* xMD2, uint32_t xLength)
{
    uint32_t count = 0;
    for (int i = 0; i < xLength; i++)
    {
        uint8_t diff = xMD1[i] ^ xMD2[i];
        for (int j = 0; j < 8; j++)
        {
            if ((diff >> j) & 1)
            {
                count++;
            }
        }
    }
    return count;
}

void MD5Sum(uint8_t* digest, uint8_t* messageString, uint32_t xOpt)
{
    uint32_t unpaddedLength = strlen(messageString);
    MD5CreateLookUpTable();

    /*Initialize message digest*/
    memcpy(digest, MD5REGISTERS, sizeof(MD5REGISTERS));

    /* MD5Transform full blocks of message */
    MD5Transform(digest, messageString, unpaddedLength / 64, xOpt);

    /* MD5Transform remaining bits */
    /* Create the Pad */
    uint8_t paddedString[128] = { 0 };
    uint32_t mod = unpaddedLength % 64;
    memcpy(paddedString, messageString + (64 * (unpaddedLength / 64)), mod);

    uint32_t nPadding = (mod > 56) ? (56 + (64 - mod)) : (56 - mod);
    memcpy(paddedString + mod, PADDING, nPadding);

    /* Append the length in *bits* */
    uint64_t len = strlen(messageString) * 8;
    uint8_t lenBuf[8];
    MD5_statetodigest(lenBuf, &len, 8);
    memcpy(paddedString + mod + nPadding, lenBuf, sizeof(uint64_t));
    
    MD5Transform(digest, paddedString, (mod + nPadding + 8) / 64, xOpt);
}

void MD5_TestForStatisticalRandomness(uint8_t* digest)
{
    PRINT_MD(digest);

    double result_freqTest = Frequency_Test(digest, 16);
    printf("Frequency Test: %lf Status: %s\n", result_freqTest, ((result_freqTest > 0.40) && (result_freqTest < 0.60)) ? "Passed" : "Failed");

    double result_ChiSquareTest = ChiSquare_Test(digest, 16);
    printf("ChiSquare Test: %lf Status: %s\n", result_ChiSquareTest, (result_ChiSquareTest < 3.841) ? "Passed" : "Failed");

    double result_MonobitTest = Monobit_Test(digest, 16);
    printf("Monobit Test: %lf Status: %s\n", result_MonobitTest, (result_MonobitTest >= 0.01) ? "Passed" : "Failed");
}

void MD5_TestOptimizations(uint8_t* digest, uint8_t* messageString)
{
    for (uint32_t i = 0; i < 16; i++)
    {
        uint8_t digestOpt[16];
        printf("---------------\n");
        printf("Optimization %d\n", i);
        printf("---------------\n");
        MD5Sum(digestOpt, messageString, i);
        printf("MD5[%s]:", messageString);
        MD5_TestForStatisticalRandomness(digestOpt);
        printf("MD and MDOpt[%d] differ by %d bits.\n", i, BitwiseDifference(digest, digestOpt, 16));
        printf("\n");
    }
}

void MD5_TestAvalancheEffect(uint8_t* messageString, uint32_t length, uint32_t xOpt)
{
    uint8_t ref_digest[16] = { 0 };
    double avgBitwiseDifference = 0.0;
    uint32_t count = 0;
    
    uint8_t* msg = (uint8_t*)malloc(length);
    memset(msg, 0, length);

    MD5Sum(ref_digest, messageString, MD5_NOMINAL_MODE);

    for (uint32_t i = 0; i < length; i++)
    {
        for (uint32_t j = 0; j < 8; j++)
        {
            uint8_t test_digest[16] = { 0 };
            memcpy(msg, messageString, length);
            msg[i] = msg[i] ^ (1 << j);
            MD5Sum(test_digest, msg, xOpt);
            avgBitwiseDifference = (double)BitwiseDifference(ref_digest, test_digest, 16)/128;
            if (avgBitwiseDifference >= 0.50)
            {
                count++;
            }
            memset(msg, 0, length);
        }
    }

    if (count >= ((length * 8) / 2))
    {
        printf("Avalanche Criterion PASSED for Opt %d.\n", xOpt);
    }
    else
    {
        printf("Avalanche Criterion FAILED for Opt %d, only %lf bits vary on average\n", xOpt, (double)count/(length*8));
    }

    free(msg);
}

int main(int argc, char** argv)
{
    uint8_t digest[16] = { 0 };
    uint8_t* messageString;
    if (1 < argc)
    {
        messageString = argv[1];
    }
    else
    {
        messageString = gPlaintext;
    }
    MD5Sum(digest, messageString, MD5_NOMINAL_MODE);
    printf("MD5[%s]: ", messageString);
    PRINT_MD(digest);
    printf("\n");

    printf("--------------------------------\n");
    printf("Tests for Statistical Randomness\n");
    printf("--------------------------------\n");
    MD5_TestForStatisticalRandomness(digest);
    printf("\n");

    printf("-----------------------------\n");
    printf("Tests for Avalanche Criterion\n");
    printf("-----------------------------\n");
    for (uint32_t i = 1; i < 16; i++)
    {
        MD5_TestAvalancheEffect(messageString, strlen(messageString), i);
    }
    
    //MD5_TestOptimizations(digest, messageString);
    return 0;
}