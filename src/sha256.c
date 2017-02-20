#include <string.h>
#include "sha256.h"

static const uint32_t sha256_k[64] =
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static const uint32_t sha256_initial[8] = 
{
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

static const uint8_t sha256_pad[64] = 
{
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static inline uint32_t ror(uint32_t x, uint32_t n)
{
    return (x >> n) | (x << (32-n));
}

static inline uint32_t rol(uint32_t x, uint32_t n)
{
    return (x << n) | (x >> (32-n));
}

static inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ ((~x) & z);
}

static inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

static inline uint32_t sum0(uint32_t x)
{
    return ror(x, 2) ^ ror(x, 13) ^ ror(x, 22);
}

static inline uint32_t sum1(uint32_t x)
{
    return ror(x, 6) ^ ror(x, 11) ^ ror(x, 25);
}

static inline uint32_t sigma0(uint32_t x)
{
    return ror(x, 7) ^ ror(x, 18) ^ (x >> 3);
}

static inline uint32_t sigma1(uint32_t x)
{
    return ror(x, 17) ^ ror(x, 19) ^ (x >> 10);
}

/** Initializes SHA 256 context. **/
void sha256_init(struct sha256_t* context)
{
    context->len = 0;
    context->total = 0;
    memcpy(context->state, sha256_initial, 8*sizeof(uint32_t));
}
/** Process current message block. **/
static inline void sha256_transform(struct sha256_t* context)
{
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t w[64];
    int i;
    for(i=0; i<16; i++)
    {
        w[i] = (context->buffer[(4*i)  ] << 24) | (context->buffer[(4*i)+1] << 16)
             | (context->buffer[(4*i)+2] <<  8) | (context->buffer[(4*i)+3]      );
    }
    for( ; i<64; i++)
    {
        w[i] = sigma1(w[i-2]) + w[i-7] + sigma0(w[i-15]) + w[i-16];
    }
    a = context->state[0];
    b = context->state[1];
    c = context->state[2];
    d = context->state[3];
    e = context->state[4];
    f = context->state[5];
    g = context->state[6];
    h = context->state[7];
    for(i=0; i<64; i++)
    {
        uint32_t t1 = h + sum1(e) + ch(e, f, g) + sha256_k[i] + w[i];
        uint32_t t2 = sum0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;
    context->state[4] += e;
    context->state[5] += f;
    context->state[6] += g;
    context->state[7] += h;
}
/** Updates context. **/
void sha256_add(struct sha256_t* context, const void *data, size_t len)
{
    const uint8_t* ptr = (const uint8_t*)data;
    while(len)
    {
        size_t remaining = 64 - context->len;
        size_t n = (len < remaining) ? len : remaining;

        memcpy(context->buffer+context->len, ptr, n);
        
        ptr += n;
        len -= n;
        context->len += n;
        context->total += n;
        if(64 == context->len)
        {
            sha256_transform(context);
            context->len = 0;
        }
    }
}
/** Compute SHA 256 digest. **/
void sha256_finalize(struct sha256_t* context, uint8_t* digest)
{
    uint64_t bits = context->total * 8;
    /* padding */
    size_t pad_len = ((context->len < 56) ? 0 : 64) + 56 - context->len;
    sha256_add(context, sha256_pad, pad_len);
    /* message length */
    context->buffer[63] = bits;
    context->buffer[62] = bits >>  8;
    context->buffer[61] = bits >> 16;
    context->buffer[60] = bits >> 24;
    context->buffer[59] = bits >> 32;
    context->buffer[58] = bits >> 40;
    context->buffer[57] = bits >> 48;
    context->buffer[56] = bits >> 56;
    sha256_transform(context);
    /* store digest */
    for(int i=0; i<8; i++)
    {
        *digest++ = (context->state[i] >> 24) & 0xff;
        *digest++ = (context->state[i] >> 16) & 0xff;
        *digest++ = (context->state[i] >>  8) & 0xff;
        *digest++ = (context->state[i] >>  0) & 0xff;
    }
}
/** All in one SHA 256 digest computation.**/
void sha256_digest(const void* data, size_t len, uint8_t* digest)
{
    struct sha256_t ctx;
    sha256_init(&ctx);
    sha256_add(&ctx, data, len);
    sha256_finalize(&ctx, digest);
}
