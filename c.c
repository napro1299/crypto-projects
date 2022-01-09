#include <immintrin.h>
#include <stdio.h>
#include "sha256.h"

int main() {
    // int d[8];
    // _mm256_storeu_si256((__m256i *)d, _mm256_set1_epi8(0xbb));
    // __m256i val = _mm256_loadu_si256((__m256i *)d);
    // printf("%llx\n%llx\n%llx\n%llx\n",_mm256_extract_epi64(val, 0),
    //                           _mm256_extract_epi64(val, 1),
    //                           _mm256_extract_epi64(val, 2),
    //                          _mm256_extract_epi64(val, 3));
    
#define WORDS(x) x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]

    const char *data[] = {
        "abc",
        "egypt",
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
        "nuts"
    };
    const unsigned int expected[3][8] = {
        {0xba7816bf, 0x8f01cfea, 0x414140de, 0x5dae2223, 0xb00361a3, 0x96177a9c, 0xb410ff61, 0xf20015ad},
        {0xb116c99a, 0x2c11258b, 0xdc6b121b, 0x63d190a3, 0xb86f90bb, 0xb1116dec, 0xe0b70ef7, 0x576938f2},
        {0x248d6a61, 0xd20638b8, 0xe5c02693, 0x0c3e6039, 0xa33ce459, 0x64ff2167, 0xf6ecedd4, 0x19db06c1},
        {0x9093f2bb, 0x8d45a4d7, 0x0a44fdf5, 0x6e0b62f7, 0xd6c9d7cb, 0x0748093b, 0x0cf3ff9d, 0x35b3ebbe}
    };

    unsigned int out[8];

    sha256(data[0], strlen(data[0]), out);
    printf("%s\n", data[0]);
    printf("expected: %x%x%x%x%x%x%x%x\n", WORDS(expected[0]));
    printf("got:      %x%x%x%x%x%x%x%x\n", WORDS(out));

printf("==========================================================================\n");

    sha256(data[1], strlen(data[1]), out);
    printf("%s\n", data[1]);
    printf("expected: %x%x%x%x%x%x%x%x\n", WORDS(expected[1]));
    printf("got:      %x%x%x%x%x%x%x%x\n", WORDS(out));

printf("==========================================================================\n");

    sha256(data[2], strlen(data[2]), out);
    printf("%s\n", data[2]);
    printf("expected: %x%x%x%x%x%x%x%x\n", WORDS(expected[2]));
    printf("got:      %x%x%x%x%x%x%x%x\n", WORDS(out));
    printf("size: %i", strlen(data[2]));

printf("==========================================================================\n");

    sha256(data[3], strlen(data[3]), out);
    printf("%s\n", data[3]);
    printf("expected: %x%x%x%x%x%x%x%x\n", WORDS(expected[3]));
    printf("got:      %x%x%x%x%x%x%x%x\n", WORDS(out));
}
