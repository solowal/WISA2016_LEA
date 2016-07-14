
#ifndef NEON_ARM_TMP128
#define NEON_ARM_TMP128
#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpu-features.h>

void armv7_LEA_EncryptBlk_NEON_ARM128(uint32_t key[4 * 24], uint32_t text[13 * 4]);

#endif
