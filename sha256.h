#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <string.h>
#include <immintrin.h>
#include <x86intrin.h>

typedef uint16_t u64;
typedef uint32_t u32;
typedef uint8_t u8;

#define ROTR(x, n) _lrotr(x, n)

#define CH(x, y, z) (((x) & (y)) ^ ((x) & (z)))
#define MAJ(x,y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define SUM0(x) (ROTR(x, 2)  ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SUM1(x) (ROTR(x, 6)  ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SIG0(x) (ROTR(x, 7)  ^ ROTR(x, 18) ^ (x >> 3))
#define SIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))

static const u32 k[] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
#define SUM0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define SUM1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))

void sha256(const char *data, size_t size, void *__out) {
	u32 a, b, c, d, e, f, g, h, i, j, t1, t2, W[64], H[8] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};
	u32 *out = (u32 *)__out;
	__m256i m[2]; // current message

    for (i = 0; i <= size/64; i++) {
        if (i == size/64) {
			memset(m, 0, 64);
			memcpy((char *)m, data + i * 64, size - i * 64);
			((u8 *)m)[size - i * 64] = (u8)0x80;
			((u32 *)m)[15] = (((u32)size - i * 64) * 8) << 24;
			goto transform;
		}
		_mm256_storeu_si256(m, _mm256_loadu_si256((__m256i *)(data + i * 64)));
		_mm256_storeu_si256( &m[1],
		size-(i*64) > 32 ?
		_mm256_loadu_si256((__m256i *)(data+(i*64) + 32)) :
		_mm256_setzero_si256() );

transform:
		/* optimize to 2 128-bit loads */
		a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		e = H[4];
		f = H[5];
		g = H[6];
		h = H[7];

        int jj;
        u8 *tdata = (u8 *)m;
#ifdef SHA_DEBUG
        memset(W, 0, sizeof(W)); /* Ease of investigation in debugger memory view */
#endif
		//memcpy(W, (char *)m, 16);
		for (j = 0, jj = 0; j < 16; ++j, jj += 4)
            W[j] = (tdata[jj] << 24) | (tdata[jj + 1] << 16) | (tdata[jj + 2] << 8) | (tdata[jj + 3]);
		for (j = 16; j < 64; j++) {
			W[j] = SIG1(W[j-2]) + W[j-7] + SIG0(W[j-15]) + W[j-16];
		}

		for (j = 0; j < 64; j++) {
			t1 = h + SUM1(e) + CH(e, f, g) + k[j] + W[j];
			t2 = SUM0(a) + MAJ(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + t1;
			d = c;
			c = b;
			b = a;
			a = t1 + t2;
		}

		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
		H[5] += f;
		H[6] += g;
		H[7] += h;
	}
/* Pseudo:
	out[0:7] = H[0:7]
*/
	_mm256_storeu_si256((__m256i *)out, _mm256_loadu_si256((__m256i *)H));
}

#endif 