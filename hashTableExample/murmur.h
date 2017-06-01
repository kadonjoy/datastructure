//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

#include <stdint.h>

typedef void (MURMUR_FUNC)(const void *key, int len, uint32_t seed, void *out);

MURMUR_FUNC MurmurHash3_x86_32;
MURMUR_FUNC MurmurHash3_x86_128;
MURMUR_FUNC MurmurHash3_x64_128;

#endif // _MURMURHASH3_H_
