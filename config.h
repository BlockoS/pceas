#ifndef PCEAS_CONFIG_H
#define PCEAS_CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __GNUC__
#define FORCE_INLINE __attribute__((always_inline)) inline
#else
#define FORCE_INLINE inline
#endif /* __GNUC__ */

#endif /* PCEAS_CONFIG_H */
