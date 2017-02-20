#ifndef PCEAS_SHA_256_H
#define PCEAS_SHA_256_H

#include <stdint.h>
#include <stdlib.h>

/**
 * SHA 256 context.
 */
struct sha256_t
{
    /** Message buffer. **/
    uint8_t buffer[64];
    /** SHA 256 state. **/
    uint32_t state[8];
    /** Number of bytes in the message buffer. **/
    size_t len;
    /** Total number of bytes processed. **/
    uint64_t total;
};

/** Initializes SHA 256 context. **/
void sha256_init(struct sha256_t* context);
/** Updates context. **/
void sha256_add(struct sha256_t* context, const void* data, size_t len);
/** Compute SHA 256 digest. **/
void sha256_finalize(struct sha256_t* context, uint8_t *digest);
/** All in one SHA 256 digest computation.**/
void sha256_digest(const void* data, size_t len, uint8_t* digest);

#endif /* PCEAS_SHA_256_H */
