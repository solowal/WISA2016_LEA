
#include "NEON_ARM_128.h"
#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpu-features.h>
#include <arm_neon.h>


void armv7_LEA_EncryptBlk_NEON_ARM128(uint32_t key[4 * 24], uint32_t text[13 * 4]) {

	asm(

			"push         {r4-r9}                 	\n\t"
			"vpush        {q4-q7}                 	\n\t"

			//plain-text load
			//12 plain-text
			"vldmia %1!, {q0-q7} 					\n\t"
			"vldmia %1!, {q8-q11} 					\n\t"
			"ldmia %1!, {r6-r9} 					\n\t"

			//initialization
			"sub %1, %1, #208						\n\t"

			//pre-processing
			"vtrn.32 q0, q1 						\n\t"
			"vtrn.32 q2, q3 						\n\t"
			"vtrn.32 q4, q5 						\n\t"
			"vtrn.32 q6, q7 						\n\t"
			"vtrn.32 q8, q9 						\n\t"
			"vtrn.32 q10, q11						\n\t"

			"vswp         d1, d4  	                \n\t"
			"vswp         d3, d6  	                \n\t"
			"vswp         d9, d12  	                \n\t"
			"vswp         d11, d14 	                \n\t"
			"vswp         d17, d20 	                \n\t"
			"vswp         d19, d22 	                \n\t"

			//key address : r0
			//data address: r1
			//tmp		  : r2 r3 r4 r5
			//q12 		key
			//q13-q15 	temp

			//#1
			//1-1
			//first  : q0 q4 q8
			//second : q1 q5 q9
			//third  : q2 q6 q10
			//fourth : q3 q7 q11
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r9, r3, r9 						\n\t"

			"veor q3, q3, q12 						\n\t"
			"eor r10, r5, r8 						\n\t"

			"veor q7, q7, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q11, q11, q12 					\n\t"
			"eor r8, r3, r8 						\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r7 						\n\t"

			"veor q13, q2, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q14, q6, q12 						\n\t"
			"eor r7, r3, r7 						\n\t"

			"veor q15, q10, q12 					\n\t"
			"eor r10, r2, r6 						\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"add r7, r7, r10 						\n\t"

			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #29 					\n\t"
			"vshl.i32 q7, q14, #29 					\n\t"
			"vshl.i32 q11, q15, #29 				\n\t"

			"vsri.32  q3, q13, #3  					\n\t"
			"vsri.32  q7, q14, #3  					\n\t"
			"vsri.32  q11, q15, #3  				\n\t"

			//1-2
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #27 					\n\t"
			"vshl.i32 q6, q14, #27 					\n\t"
			"vshl.i32 q10, q15, #27 				\n\t"

			"vsri.32  q2, q13, #5  					\n\t"
			"vsri.32  q6, q14, #5  					\n\t"
			"vsri.32  q10, q15, #5  				\n\t"

			//1-3
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12 						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #9 					\n\t"
			"vshl.i32 q5, q14, #9 					\n\t"
			"vshl.i32 q9, q15, #9 					\n\t"

			"vsri.32  q1, q13, #23 					\n\t"
			"vsri.32  q5, q14, #23 					\n\t"
			"vsri.32  q9, q15, #23					\n\t"

			//2-1
			//first  : q1 q5 q9
			//second : q2 q6 q10
			//third  : q3 q7 q11
			//fourth : q0 q4 q8
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r6, r3, r6 						\n\t"

			"veor q0, q0, q12 						\n\t"
			"eor r10, r5, r9, ror#3					\n\t"

			"veor q4, q4, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q8, q8, q12	 					\n\t"
			"eor r9, r3, r9, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r8, ror#5					\n\t"

			"veor q13, q3, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q14, q7, q12 						\n\t"
			"eor r8, r3, r8, ror#5					\n\t"

			"veor q15, q11, q12 					\n\t"
			"eor r10, r2, r7, ror#23				\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"add r8, r8, r10 						\n\t"

			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #29 					\n\t"
			"vshl.i32 q4, q14, #29 					\n\t"
			"vshl.i32 q8, q15, #29 					\n\t"

			"vsri.32  q0, q13, #3  					\n\t"
			"vsri.32  q4, q14, #3  					\n\t"
			"vsri.32  q8, q15, #3	  				\n\t"

			//2-2
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12						\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #27 					\n\t"
			"vshl.i32 q7, q14, #27 					\n\t"
			"vshl.i32 q11, q15, #27 				\n\t"

			"vsri.32  q3, q13, #5  					\n\t"
			"vsri.32  q7, q14, #5  					\n\t"
			"vsri.32  q11, q15, #5  				\n\t"

			//2-3
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #9 					\n\t"
			"vshl.i32 q6, q14, #9 					\n\t"
			"vshl.i32 q10, q15, #9					\n\t"

			"vsri.32  q2, q13, #23  				\n\t"
			"vsri.32  q6, q14, #23  				\n\t"
			"vsri.32  q10, q15, #23 				\n\t"

			//3-1
			//first  : q2 q6 q10
			//second : q3 q7 q11
			//third  : q0 q4 q8
			//fourth : q1 q5 q9
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r7, r3, r7, ror#23					\n\t"

			"veor q1, q1, q12 						\n\t"
			"eor r10, r5, r6, ror#3					\n\t"

			"veor q5, q5, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q9, q9, q12	 					\n\t"
			"eor r6, r3, r6, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r9, ror#5					\n\t"

			"veor q13, q0, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q14, q4, q12 						\n\t"
			"eor r9, r3, r9, ror#5					\n\t"

			"veor q15, q8, q12	 					\n\t"
			"eor r10, r2, r8, ror#23				\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"add r9, r9, r10 						\n\t"

			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #29 					\n\t"
			"vshl.i32 q5, q14, #29 					\n\t"
			"vshl.i32 q9, q15, #29 					\n\t"

			"vsri.32  q1, q13, #3  					\n\t"
			"vsri.32  q5, q14, #3  					\n\t"
			"vsri.32  q9, q15, #3	  				\n\t"

			//3-2
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12						\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #27 					\n\t"
			"vshl.i32 q4, q14, #27 					\n\t"
			"vshl.i32 q8, q15, #27	 				\n\t"

			"vsri.32  q0, q13, #5  					\n\t"
			"vsri.32  q4, q14, #5  					\n\t"
			"vsri.32  q8, q15, #5  					\n\t"

			//3-3
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12 					\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #9 					\n\t"
			"vshl.i32 q7, q14, #9 					\n\t"
			"vshl.i32 q11, q15, #9					\n\t"

			"vsri.32  q3, q13, #23  				\n\t"
			"vsri.32  q7, q14, #23  				\n\t"
			"vsri.32  q11, q15, #23 				\n\t"

			//4-1
			//first  : q3 q7 q11
			//second : q0 q4 q8
			//third  : q1 q5 q9
			//fourth : q2 q6 q10
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r8, r3, r8, ror#23					\n\t"

			"veor q2, q2, q12 						\n\t"
			"eor r10, r5, r7, ror#3					\n\t"

			"veor q6, q6, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q10, q10, q12	 					\n\t"
			"eor r7, r3, r7, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r6, ror#5					\n\t"

			"veor q13, q1, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q14, q5, q12 						\n\t"
			"eor r6, r3, r6, ror#5					\n\t"

			"veor q15, q9, q12	 					\n\t"
			"eor r10, r2, r9, ror#23				\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"add r6, r6, r10 						\n\t"

			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #29 					\n\t"
			"vshl.i32 q6, q14, #29 					\n\t"
			"vshl.i32 q10, q15, #29					\n\t"

			"vsri.32  q2, q13, #3  					\n\t"
			"vsri.32  q6, q14, #3  					\n\t"
			"vsri.32  q10, q15, #3	  				\n\t"

			//4-2
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #27 					\n\t"
			"vshl.i32 q5, q14, #27 					\n\t"
			"vshl.i32 q9, q15, #27	 				\n\t"

			"vsri.32  q1, q13, #5  					\n\t"
			"vsri.32  q5, q14, #5  					\n\t"
			"vsri.32  q9, q15, #5  					\n\t"

			//4-3
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12 					\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #9 					\n\t"
			"vshl.i32 q4, q14, #9 					\n\t"
			"vshl.i32 q8, q15, #9					\n\t"

			"vsri.32  q0, q13, #23  				\n\t"
			"vsri.32  q4, q14, #23  				\n\t"
			"vsri.32  q8, q15, #23 					\n\t"

			//#2
			//#1
			//1-1
			//first  : q0 q4 q8
			//second : q1 q5 q9
			//third  : q2 q6 q10
			//fourth : q3 q7 q11
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r9, r3, r9, ror#23					\n\t"

			"veor q3, q3, q12 						\n\t"
			"eor r10, r5, r8, ror#3					\n\t"

			"veor q7, q7, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q11, q11, q12 					\n\t"
			"eor r8, r3, r8, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r7, ror#5					\n\t"

			"veor q13, q2, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q14, q6, q12 						\n\t"
			"eor r7, r3, r7, ror#5					\n\t"

			"veor q15, q10, q12 					\n\t"
			"eor r10, r2, r6, ror#23				\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"add r7, r7, r10 						\n\t"

			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #29 					\n\t"
			"vshl.i32 q7, q14, #29 					\n\t"
			"vshl.i32 q11, q15, #29 				\n\t"

			"vsri.32  q3, q13, #3  					\n\t"
			"vsri.32  q7, q14, #3  					\n\t"
			"vsri.32  q11, q15, #3  				\n\t"

			//1-2
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #27 					\n\t"
			"vshl.i32 q6, q14, #27 					\n\t"
			"vshl.i32 q10, q15, #27 				\n\t"

			"vsri.32  q2, q13, #5  					\n\t"
			"vsri.32  q6, q14, #5  					\n\t"
			"vsri.32  q10, q15, #5  				\n\t"

			//1-3
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12 						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #9 					\n\t"
			"vshl.i32 q5, q14, #9 					\n\t"
			"vshl.i32 q9, q15, #9 					\n\t"

			"vsri.32  q1, q13, #23 					\n\t"
			"vsri.32  q5, q14, #23 					\n\t"
			"vsri.32  q9, q15, #23					\n\t"

			//2-1
			//first  : q1 q5 q9
			//second : q2 q6 q10
			//third  : q3 q7 q11
			//fourth : q0 q4 q8
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r6, r3, r6, ror#23					\n\t"

			"veor q0, q0, q12 						\n\t"
			"eor r10, r5, r9, ror#3					\n\t"

			"veor q4, q4, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q8, q8, q12	 					\n\t"
			"eor r9, r3, r9, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r8, ror#5					\n\t"

			"veor q13, q3, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q14, q7, q12 						\n\t"
			"eor r8, r3, r8, ror#5					\n\t"

			"veor q15, q11, q12 					\n\t"
			"eor r10, r2, r7, ror#23				\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"add r8, r8, r10 						\n\t"

			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #29 					\n\t"
			"vshl.i32 q4, q14, #29 					\n\t"
			"vshl.i32 q8, q15, #29 					\n\t"

			"vsri.32  q0, q13, #3  					\n\t"
			"vsri.32  q4, q14, #3  					\n\t"
			"vsri.32  q8, q15, #3	  				\n\t"

			//2-2
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12						\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #27 					\n\t"
			"vshl.i32 q7, q14, #27 					\n\t"
			"vshl.i32 q11, q15, #27 				\n\t"

			"vsri.32  q3, q13, #5  					\n\t"
			"vsri.32  q7, q14, #5  					\n\t"
			"vsri.32  q11, q15, #5  				\n\t"

			//2-3
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #9 					\n\t"
			"vshl.i32 q6, q14, #9 					\n\t"
			"vshl.i32 q10, q15, #9					\n\t"

			"vsri.32  q2, q13, #23  				\n\t"
			"vsri.32  q6, q14, #23  				\n\t"
			"vsri.32  q10, q15, #23 				\n\t"

			//3-1
			//first  : q2 q6 q10
			//second : q3 q7 q11
			//third  : q0 q4 q8
			//fourth : q1 q5 q9
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r7, r3, r7, ror#23					\n\t"

			"veor q1, q1, q12 						\n\t"
			"eor r10, r5, r6, ror#3					\n\t"

			"veor q5, q5, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q9, q9, q12	 					\n\t"
			"eor r6, r3, r6, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r9, ror#5					\n\t"

			"veor q13, q0, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q14, q4, q12 						\n\t"
			"eor r9, r3, r9, ror#5					\n\t"

			"veor q15, q8, q12	 					\n\t"
			"eor r10, r2, r8, ror#23				\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"add r9, r9, r10 						\n\t"

			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #29 					\n\t"
			"vshl.i32 q5, q14, #29 					\n\t"
			"vshl.i32 q9, q15, #29 					\n\t"

			"vsri.32  q1, q13, #3  					\n\t"
			"vsri.32  q5, q14, #3  					\n\t"
			"vsri.32  q9, q15, #3	  				\n\t"

			//3-2
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12						\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #27 					\n\t"
			"vshl.i32 q4, q14, #27 					\n\t"
			"vshl.i32 q8, q15, #27	 				\n\t"

			"vsri.32  q0, q13, #5  					\n\t"
			"vsri.32  q4, q14, #5  					\n\t"
			"vsri.32  q8, q15, #5  					\n\t"

			//3-3
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12 					\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #9 					\n\t"
			"vshl.i32 q7, q14, #9 					\n\t"
			"vshl.i32 q11, q15, #9					\n\t"

			"vsri.32  q3, q13, #23  				\n\t"
			"vsri.32  q7, q14, #23  				\n\t"
			"vsri.32  q11, q15, #23 				\n\t"

			//4-1
			//first  : q3 q7 q11
			//second : q0 q4 q8
			//third  : q1 q5 q9
			//fourth : q2 q6 q10
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r8, r3, r8, ror#23					\n\t"

			"veor q2, q2, q12 						\n\t"
			"eor r10, r5, r7, ror#3					\n\t"

			"veor q6, q6, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q10, q10, q12	 					\n\t"
			"eor r7, r3, r7, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r6, ror#5					\n\t"

			"veor q13, q1, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q14, q5, q12 						\n\t"
			"eor r6, r3, r6, ror#5					\n\t"

			"veor q15, q9, q12	 					\n\t"
			"eor r10, r2, r9, ror#23				\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"add r6, r6, r10 						\n\t"

			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #29 					\n\t"
			"vshl.i32 q6, q14, #29 					\n\t"
			"vshl.i32 q10, q15, #29					\n\t"

			"vsri.32  q2, q13, #3  					\n\t"
			"vsri.32  q6, q14, #3  					\n\t"
			"vsri.32  q10, q15, #3	  				\n\t"

			//4-2
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #27 					\n\t"
			"vshl.i32 q5, q14, #27 					\n\t"
			"vshl.i32 q9, q15, #27	 				\n\t"

			"vsri.32  q1, q13, #5  					\n\t"
			"vsri.32  q5, q14, #5  					\n\t"
			"vsri.32  q9, q15, #5  					\n\t"

			//4-3
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12 					\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #9 					\n\t"
			"vshl.i32 q4, q14, #9 					\n\t"
			"vshl.i32 q8, q15, #9					\n\t"

			"vsri.32  q0, q13, #23  				\n\t"
			"vsri.32  q4, q14, #23  				\n\t"
			"vsri.32  q8, q15, #23 					\n\t"

			//#2
			//#1
			//1-1
			//first  : q0 q4 q8
			//second : q1 q5 q9
			//third  : q2 q6 q10
			//fourth : q3 q7 q11
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r9, r3, r9, ror#23					\n\t"

			"veor q3, q3, q12 						\n\t"
			"eor r10, r5, r8, ror#3					\n\t"

			"veor q7, q7, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q11, q11, q12 					\n\t"
			"eor r8, r3, r8, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r7, ror#5					\n\t"

			"veor q13, q2, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q14, q6, q12 						\n\t"
			"eor r7, r3, r7, ror#5					\n\t"

			"veor q15, q10, q12 					\n\t"
			"eor r10, r2, r6, ror#23				\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"add r7, r7, r10 						\n\t"

			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #29 					\n\t"
			"vshl.i32 q7, q14, #29 					\n\t"
			"vshl.i32 q11, q15, #29 				\n\t"

			"vsri.32  q3, q13, #3  					\n\t"
			"vsri.32  q7, q14, #3  					\n\t"
			"vsri.32  q11, q15, #3  				\n\t"

			//1-2
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #27 					\n\t"
			"vshl.i32 q6, q14, #27 					\n\t"
			"vshl.i32 q10, q15, #27 				\n\t"

			"vsri.32  q2, q13, #5  					\n\t"
			"vsri.32  q6, q14, #5  					\n\t"
			"vsri.32  q10, q15, #5  				\n\t"

			//1-3
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12 						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #9 					\n\t"
			"vshl.i32 q5, q14, #9 					\n\t"
			"vshl.i32 q9, q15, #9 					\n\t"

			"vsri.32  q1, q13, #23 					\n\t"
			"vsri.32  q5, q14, #23 					\n\t"
			"vsri.32  q9, q15, #23					\n\t"

			//2-1
			//first  : q1 q5 q9
			//second : q2 q6 q10
			//third  : q3 q7 q11
			//fourth : q0 q4 q8
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r6, r3, r6, ror#23					\n\t"

			"veor q0, q0, q12 						\n\t"
			"eor r10, r5, r9, ror#3					\n\t"

			"veor q4, q4, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q8, q8, q12	 					\n\t"
			"eor r9, r3, r9, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r8, ror#5					\n\t"

			"veor q13, q3, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q14, q7, q12 						\n\t"
			"eor r8, r3, r8, ror#5					\n\t"

			"veor q15, q11, q12 					\n\t"
			"eor r10, r2, r7, ror#23				\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"add r8, r8, r10 						\n\t"

			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #29 					\n\t"
			"vshl.i32 q4, q14, #29 					\n\t"
			"vshl.i32 q8, q15, #29 					\n\t"

			"vsri.32  q0, q13, #3  					\n\t"
			"vsri.32  q4, q14, #3  					\n\t"
			"vsri.32  q8, q15, #3	  				\n\t"

			//2-2
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12						\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #27 					\n\t"
			"vshl.i32 q7, q14, #27 					\n\t"
			"vshl.i32 q11, q15, #27 				\n\t"

			"vsri.32  q3, q13, #5  					\n\t"
			"vsri.32  q7, q14, #5  					\n\t"
			"vsri.32  q11, q15, #5  				\n\t"

			//2-3
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #9 					\n\t"
			"vshl.i32 q6, q14, #9 					\n\t"
			"vshl.i32 q10, q15, #9					\n\t"

			"vsri.32  q2, q13, #23  				\n\t"
			"vsri.32  q6, q14, #23  				\n\t"
			"vsri.32  q10, q15, #23 				\n\t"

			//3-1
			//first  : q2 q6 q10
			//second : q3 q7 q11
			//third  : q0 q4 q8
			//fourth : q1 q5 q9
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r7, r3, r7, ror#23					\n\t"

			"veor q1, q1, q12 						\n\t"
			"eor r10, r5, r6, ror#3					\n\t"

			"veor q5, q5, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q9, q9, q12	 					\n\t"
			"eor r6, r3, r6, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r9, ror#5					\n\t"

			"veor q13, q0, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q14, q4, q12 						\n\t"
			"eor r9, r3, r9, ror#5					\n\t"

			"veor q15, q8, q12	 					\n\t"
			"eor r10, r2, r8, ror#23				\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"add r9, r9, r10 						\n\t"

			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #29 					\n\t"
			"vshl.i32 q5, q14, #29 					\n\t"
			"vshl.i32 q9, q15, #29 					\n\t"

			"vsri.32  q1, q13, #3  					\n\t"
			"vsri.32  q5, q14, #3  					\n\t"
			"vsri.32  q9, q15, #3	  				\n\t"

			//3-2
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12						\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #27 					\n\t"
			"vshl.i32 q4, q14, #27 					\n\t"
			"vshl.i32 q8, q15, #27	 				\n\t"

			"vsri.32  q0, q13, #5  					\n\t"
			"vsri.32  q4, q14, #5  					\n\t"
			"vsri.32  q8, q15, #5  					\n\t"

			//3-3
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12 					\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #9 					\n\t"
			"vshl.i32 q7, q14, #9 					\n\t"
			"vshl.i32 q11, q15, #9					\n\t"

			"vsri.32  q3, q13, #23  				\n\t"
			"vsri.32  q7, q14, #23  				\n\t"
			"vsri.32  q11, q15, #23 				\n\t"

			//4-1
			//first  : q3 q7 q11
			//second : q0 q4 q8
			//third  : q1 q5 q9
			//fourth : q2 q6 q10
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r8, r3, r8, ror#23					\n\t"

			"veor q2, q2, q12 						\n\t"
			"eor r10, r5, r7, ror#3					\n\t"

			"veor q6, q6, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q10, q10, q12	 					\n\t"
			"eor r7, r3, r7, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r6, ror#5					\n\t"

			"veor q13, q1, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q14, q5, q12 						\n\t"
			"eor r6, r3, r6, ror#5					\n\t"

			"veor q15, q9, q12	 					\n\t"
			"eor r10, r2, r9, ror#23				\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"add r6, r6, r10 						\n\t"

			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #29 					\n\t"
			"vshl.i32 q6, q14, #29 					\n\t"
			"vshl.i32 q10, q15, #29					\n\t"

			"vsri.32  q2, q13, #3  					\n\t"
			"vsri.32  q6, q14, #3  					\n\t"
			"vsri.32  q10, q15, #3	  				\n\t"

			//4-2
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #27 					\n\t"
			"vshl.i32 q5, q14, #27 					\n\t"
			"vshl.i32 q9, q15, #27	 				\n\t"

			"vsri.32  q1, q13, #5  					\n\t"
			"vsri.32  q5, q14, #5  					\n\t"
			"vsri.32  q9, q15, #5  					\n\t"

			//4-3
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12 					\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #9 					\n\t"
			"vshl.i32 q4, q14, #9 					\n\t"
			"vshl.i32 q8, q15, #9					\n\t"

			"vsri.32  q0, q13, #23  				\n\t"
			"vsri.32  q4, q14, #23  				\n\t"
			"vsri.32  q8, q15, #23 					\n\t"
			//#2
			//#1
			//1-1
			//first  : q0 q4 q8
			//second : q1 q5 q9
			//third  : q2 q6 q10
			//fourth : q3 q7 q11
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r9, r3, r9, ror#23					\n\t"

			"veor q3, q3, q12 						\n\t"
			"eor r10, r5, r8, ror#3					\n\t"

			"veor q7, q7, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q11, q11, q12 					\n\t"
			"eor r8, r3, r8, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r7, ror#5					\n\t"

			"veor q13, q2, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q14, q6, q12 						\n\t"
			"eor r7, r3, r7, ror#5					\n\t"

			"veor q15, q10, q12 					\n\t"
			"eor r10, r2, r6, ror#23				\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"add r7, r7, r10 						\n\t"

			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #29 					\n\t"
			"vshl.i32 q7, q14, #29 					\n\t"
			"vshl.i32 q11, q15, #29 				\n\t"

			"vsri.32  q3, q13, #3  					\n\t"
			"vsri.32  q7, q14, #3  					\n\t"
			"vsri.32  q11, q15, #3  				\n\t"

			//1-2
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #27 					\n\t"
			"vshl.i32 q6, q14, #27 					\n\t"
			"vshl.i32 q10, q15, #27 				\n\t"

			"vsri.32  q2, q13, #5  					\n\t"
			"vsri.32  q6, q14, #5  					\n\t"
			"vsri.32  q10, q15, #5  				\n\t"

			//1-3
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12 						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #9 					\n\t"
			"vshl.i32 q5, q14, #9 					\n\t"
			"vshl.i32 q9, q15, #9 					\n\t"

			"vsri.32  q1, q13, #23 					\n\t"
			"vsri.32  q5, q14, #23 					\n\t"
			"vsri.32  q9, q15, #23					\n\t"

			//2-1
			//first  : q1 q5 q9
			//second : q2 q6 q10
			//third  : q3 q7 q11
			//fourth : q0 q4 q8
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r6, r3, r6, ror#23					\n\t"

			"veor q0, q0, q12 						\n\t"
			"eor r10, r5, r9, ror#3					\n\t"

			"veor q4, q4, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q8, q8, q12	 					\n\t"
			"eor r9, r3, r9, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r8, ror#5					\n\t"

			"veor q13, q3, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q14, q7, q12 						\n\t"
			"eor r8, r3, r8, ror#5					\n\t"

			"veor q15, q11, q12 					\n\t"
			"eor r10, r2, r7, ror#23				\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"add r8, r8, r10 						\n\t"

			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #29 					\n\t"
			"vshl.i32 q4, q14, #29 					\n\t"
			"vshl.i32 q8, q15, #29 					\n\t"

			"vsri.32  q0, q13, #3  					\n\t"
			"vsri.32  q4, q14, #3  					\n\t"
			"vsri.32  q8, q15, #3	  				\n\t"

			//2-2
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12						\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #27 					\n\t"
			"vshl.i32 q7, q14, #27 					\n\t"
			"vshl.i32 q11, q15, #27 				\n\t"

			"vsri.32  q3, q13, #5  					\n\t"
			"vsri.32  q7, q14, #5  					\n\t"
			"vsri.32  q11, q15, #5  				\n\t"

			//2-3
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #9 					\n\t"
			"vshl.i32 q6, q14, #9 					\n\t"
			"vshl.i32 q10, q15, #9					\n\t"

			"vsri.32  q2, q13, #23  				\n\t"
			"vsri.32  q6, q14, #23  				\n\t"
			"vsri.32  q10, q15, #23 				\n\t"

			//3-1
			//first  : q2 q6 q10
			//second : q3 q7 q11
			//third  : q0 q4 q8
			//fourth : q1 q5 q9
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r7, r3, r7, ror#23					\n\t"

			"veor q1, q1, q12 						\n\t"
			"eor r10, r5, r6, ror#3					\n\t"

			"veor q5, q5, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q9, q9, q12	 					\n\t"
			"eor r6, r3, r6, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r9, ror#5					\n\t"

			"veor q13, q0, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q14, q4, q12 						\n\t"
			"eor r9, r3, r9, ror#5					\n\t"

			"veor q15, q8, q12	 					\n\t"
			"eor r10, r2, r8, ror#23				\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"add r9, r9, r10 						\n\t"

			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #29 					\n\t"
			"vshl.i32 q5, q14, #29 					\n\t"
			"vshl.i32 q9, q15, #29 					\n\t"

			"vsri.32  q1, q13, #3  					\n\t"
			"vsri.32  q5, q14, #3  					\n\t"
			"vsri.32  q9, q15, #3	  				\n\t"

			//3-2
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12						\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #27 					\n\t"
			"vshl.i32 q4, q14, #27 					\n\t"
			"vshl.i32 q8, q15, #27	 				\n\t"

			"vsri.32  q0, q13, #5  					\n\t"
			"vsri.32  q4, q14, #5  					\n\t"
			"vsri.32  q8, q15, #5  					\n\t"

			//3-3
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12 					\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #9 					\n\t"
			"vshl.i32 q7, q14, #9 					\n\t"
			"vshl.i32 q11, q15, #9					\n\t"

			"vsri.32  q3, q13, #23  				\n\t"
			"vsri.32  q7, q14, #23  				\n\t"
			"vsri.32  q11, q15, #23 				\n\t"

			//4-1
			//first  : q3 q7 q11
			//second : q0 q4 q8
			//third  : q1 q5 q9
			//fourth : q2 q6 q10
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r8, r3, r8, ror#23					\n\t"

			"veor q2, q2, q12 						\n\t"
			"eor r10, r5, r7, ror#3					\n\t"

			"veor q6, q6, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q10, q10, q12	 					\n\t"
			"eor r7, r3, r7, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r6, ror#5					\n\t"

			"veor q13, q1, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q14, q5, q12 						\n\t"
			"eor r6, r3, r6, ror#5					\n\t"

			"veor q15, q9, q12	 					\n\t"
			"eor r10, r2, r9, ror#23				\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"add r6, r6, r10 						\n\t"

			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #29 					\n\t"
			"vshl.i32 q6, q14, #29 					\n\t"
			"vshl.i32 q10, q15, #29					\n\t"

			"vsri.32  q2, q13, #3  					\n\t"
			"vsri.32  q6, q14, #3  					\n\t"
			"vsri.32  q10, q15, #3	  				\n\t"

			//4-2
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #27 					\n\t"
			"vshl.i32 q5, q14, #27 					\n\t"
			"vshl.i32 q9, q15, #27	 				\n\t"

			"vsri.32  q1, q13, #5  					\n\t"
			"vsri.32  q5, q14, #5  					\n\t"
			"vsri.32  q9, q15, #5  					\n\t"

			//4-3
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12 					\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #9 					\n\t"
			"vshl.i32 q4, q14, #9 					\n\t"
			"vshl.i32 q8, q15, #9					\n\t"

			"vsri.32  q0, q13, #23  				\n\t"
			"vsri.32  q4, q14, #23  				\n\t"
			"vsri.32  q8, q15, #23 					\n\t"
			//#2
			//#1
			//1-1
			//first  : q0 q4 q8
			//second : q1 q5 q9
			//third  : q2 q6 q10
			//fourth : q3 q7 q11
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r9, r3, r9, ror#23					\n\t"

			"veor q3, q3, q12 						\n\t"
			"eor r10, r5, r8, ror#3					\n\t"

			"veor q7, q7, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q11, q11, q12 					\n\t"
			"eor r8, r3, r8, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r7, ror#5					\n\t"

			"veor q13, q2, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q14, q6, q12 						\n\t"
			"eor r7, r3, r7, ror#5					\n\t"

			"veor q15, q10, q12 					\n\t"
			"eor r10, r2, r6, ror#23				\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"add r7, r7, r10 						\n\t"

			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #29 					\n\t"
			"vshl.i32 q7, q14, #29 					\n\t"
			"vshl.i32 q11, q15, #29 				\n\t"

			"vsri.32  q3, q13, #3  					\n\t"
			"vsri.32  q7, q14, #3  					\n\t"
			"vsri.32  q11, q15, #3  				\n\t"

			//1-2
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #27 					\n\t"
			"vshl.i32 q6, q14, #27 					\n\t"
			"vshl.i32 q10, q15, #27 				\n\t"

			"vsri.32  q2, q13, #5  					\n\t"
			"vsri.32  q6, q14, #5  					\n\t"
			"vsri.32  q10, q15, #5  				\n\t"

			//1-3
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12 						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #9 					\n\t"
			"vshl.i32 q5, q14, #9 					\n\t"
			"vshl.i32 q9, q15, #9 					\n\t"

			"vsri.32  q1, q13, #23 					\n\t"
			"vsri.32  q5, q14, #23 					\n\t"
			"vsri.32  q9, q15, #23					\n\t"

			//2-1
			//first  : q1 q5 q9
			//second : q2 q6 q10
			//third  : q3 q7 q11
			//fourth : q0 q4 q8
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r6, r3, r6, ror#23					\n\t"

			"veor q0, q0, q12 						\n\t"
			"eor r10, r5, r9, ror#3					\n\t"

			"veor q4, q4, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q8, q8, q12	 					\n\t"
			"eor r9, r3, r9, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r8, ror#5					\n\t"

			"veor q13, q3, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q14, q7, q12 						\n\t"
			"eor r8, r3, r8, ror#5					\n\t"

			"veor q15, q11, q12 					\n\t"
			"eor r10, r2, r7, ror#23				\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"add r8, r8, r10 						\n\t"

			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #29 					\n\t"
			"vshl.i32 q4, q14, #29 					\n\t"
			"vshl.i32 q8, q15, #29 					\n\t"

			"vsri.32  q0, q13, #3  					\n\t"
			"vsri.32  q4, q14, #3  					\n\t"
			"vsri.32  q8, q15, #3	  				\n\t"

			//2-2
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12						\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #27 					\n\t"
			"vshl.i32 q7, q14, #27 					\n\t"
			"vshl.i32 q11, q15, #27 				\n\t"

			"vsri.32  q3, q13, #5  					\n\t"
			"vsri.32  q7, q14, #5  					\n\t"
			"vsri.32  q11, q15, #5  				\n\t"

			//2-3
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #9 					\n\t"
			"vshl.i32 q6, q14, #9 					\n\t"
			"vshl.i32 q10, q15, #9					\n\t"

			"vsri.32  q2, q13, #23  				\n\t"
			"vsri.32  q6, q14, #23  				\n\t"
			"vsri.32  q10, q15, #23 				\n\t"

			//3-1
			//first  : q2 q6 q10
			//second : q3 q7 q11
			//third  : q0 q4 q8
			//fourth : q1 q5 q9
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r7, r3, r7, ror#23					\n\t"

			"veor q1, q1, q12 						\n\t"
			"eor r10, r5, r6, ror#3					\n\t"

			"veor q5, q5, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q9, q9, q12	 					\n\t"
			"eor r6, r3, r6, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r9, ror#5					\n\t"

			"veor q13, q0, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q14, q4, q12 						\n\t"
			"eor r9, r3, r9, ror#5					\n\t"

			"veor q15, q8, q12	 					\n\t"
			"eor r10, r2, r8, ror#23				\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"add r9, r9, r10 						\n\t"

			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #29 					\n\t"
			"vshl.i32 q5, q14, #29 					\n\t"
			"vshl.i32 q9, q15, #29 					\n\t"

			"vsri.32  q1, q13, #3  					\n\t"
			"vsri.32  q5, q14, #3  					\n\t"
			"vsri.32  q9, q15, #3	  				\n\t"

			//3-2
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12						\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #27 					\n\t"
			"vshl.i32 q4, q14, #27 					\n\t"
			"vshl.i32 q8, q15, #27	 				\n\t"

			"vsri.32  q0, q13, #5  					\n\t"
			"vsri.32  q4, q14, #5  					\n\t"
			"vsri.32  q8, q15, #5  					\n\t"

			//3-3
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12 					\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #9 					\n\t"
			"vshl.i32 q7, q14, #9 					\n\t"
			"vshl.i32 q11, q15, #9					\n\t"

			"vsri.32  q3, q13, #23  				\n\t"
			"vsri.32  q7, q14, #23  				\n\t"
			"vsri.32  q11, q15, #23 				\n\t"

			//4-1
			//first  : q3 q7 q11
			//second : q0 q4 q8
			//third  : q1 q5 q9
			//fourth : q2 q6 q10
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r8, r3, r8, ror#23					\n\t"

			"veor q2, q2, q12 						\n\t"
			"eor r10, r5, r7, ror#3					\n\t"

			"veor q6, q6, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q10, q10, q12	 					\n\t"
			"eor r7, r3, r7, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r6, ror#5					\n\t"

			"veor q13, q1, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q14, q5, q12 						\n\t"
			"eor r6, r3, r6, ror#5					\n\t"

			"veor q15, q9, q12	 					\n\t"
			"eor r10, r2, r9, ror#23				\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"add r6, r6, r10 						\n\t"

			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #29 					\n\t"
			"vshl.i32 q6, q14, #29 					\n\t"
			"vshl.i32 q10, q15, #29					\n\t"

			"vsri.32  q2, q13, #3  					\n\t"
			"vsri.32  q6, q14, #3  					\n\t"
			"vsri.32  q10, q15, #3	  				\n\t"

			//4-2
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #27 					\n\t"
			"vshl.i32 q5, q14, #27 					\n\t"
			"vshl.i32 q9, q15, #27	 				\n\t"

			"vsri.32  q1, q13, #5  					\n\t"
			"vsri.32  q5, q14, #5  					\n\t"
			"vsri.32  q9, q15, #5  					\n\t"

			//4-3
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12 					\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #9 					\n\t"
			"vshl.i32 q4, q14, #9 					\n\t"
			"vshl.i32 q8, q15, #9					\n\t"

			"vsri.32  q0, q13, #23  				\n\t"
			"vsri.32  q4, q14, #23  				\n\t"
			"vsri.32  q8, q15, #23 					\n\t"
			//#2
			//#1
			//1-1
			//first  : q0 q4 q8
			//second : q1 q5 q9
			//third  : q2 q6 q10
			//fourth : q3 q7 q11
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r9, r3, r9, ror#23					\n\t"

			"veor q3, q3, q12 						\n\t"
			"eor r10, r5, r8, ror#3					\n\t"

			"veor q7, q7, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q11, q11, q12 					\n\t"
			"eor r8, r3, r8, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r7, ror#5					\n\t"

			"veor q13, q2, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q14, q6, q12 						\n\t"
			"eor r7, r3, r7, ror#5					\n\t"

			"veor q15, q10, q12 					\n\t"
			"eor r10, r2, r6, ror#23				\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"add r7, r7, r10 						\n\t"

			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #29 					\n\t"
			"vshl.i32 q7, q14, #29 					\n\t"
			"vshl.i32 q11, q15, #29 				\n\t"

			"vsri.32  q3, q13, #3  					\n\t"
			"vsri.32  q7, q14, #3  					\n\t"
			"vsri.32  q11, q15, #3  				\n\t"

			//1-2
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #27 					\n\t"
			"vshl.i32 q6, q14, #27 					\n\t"
			"vshl.i32 q10, q15, #27 				\n\t"

			"vsri.32  q2, q13, #5  					\n\t"
			"vsri.32  q6, q14, #5  					\n\t"
			"vsri.32  q10, q15, #5  				\n\t"

			//1-3
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12 						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #9 					\n\t"
			"vshl.i32 q5, q14, #9 					\n\t"
			"vshl.i32 q9, q15, #9 					\n\t"

			"vsri.32  q1, q13, #23 					\n\t"
			"vsri.32  q5, q14, #23 					\n\t"
			"vsri.32  q9, q15, #23					\n\t"

			//2-1
			//first  : q1 q5 q9
			//second : q2 q6 q10
			//third  : q3 q7 q11
			//fourth : q0 q4 q8
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r6, r3, r6, ror#23					\n\t"

			"veor q0, q0, q12 						\n\t"
			"eor r10, r5, r9, ror#3					\n\t"

			"veor q4, q4, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q8, q8, q12	 					\n\t"
			"eor r9, r3, r9, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r8, ror#5					\n\t"

			"veor q13, q3, q12 						\n\t"
			"add r9, r9, r10 						\n\t"

			"veor q14, q7, q12 						\n\t"
			"eor r8, r3, r8, ror#5					\n\t"

			"veor q15, q11, q12 					\n\t"
			"eor r10, r2, r7, ror#23				\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"add r8, r8, r10 						\n\t"

			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #29 					\n\t"
			"vshl.i32 q4, q14, #29 					\n\t"
			"vshl.i32 q8, q15, #29 					\n\t"

			"vsri.32  q0, q13, #3  					\n\t"
			"vsri.32  q4, q14, #3  					\n\t"
			"vsri.32  q8, q15, #3	  				\n\t"

			//2-2
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12 					\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12						\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #27 					\n\t"
			"vshl.i32 q7, q14, #27 					\n\t"
			"vshl.i32 q11, q15, #27 				\n\t"

			"vsri.32  q3, q13, #5  					\n\t"
			"vsri.32  q7, q14, #5  					\n\t"
			"vsri.32  q11, q15, #5  				\n\t"

			//2-3
			"vdup.32 q12, r3						\n\t"
			"veor q2, q2, q12 						\n\t"
			"veor q6, q6, q12 						\n\t"
			"veor q10, q10, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q1, q12 						\n\t"
			"veor q14, q5, q12 						\n\t"
			"veor q15, q9, q12 						\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #9 					\n\t"
			"vshl.i32 q6, q14, #9 					\n\t"
			"vshl.i32 q10, q15, #9					\n\t"

			"vsri.32  q2, q13, #23  				\n\t"
			"vsri.32  q6, q14, #23  				\n\t"
			"vsri.32  q10, q15, #23 				\n\t"

			//3-1
			//first  : q2 q6 q10
			//second : q3 q7 q11
			//third  : q0 q4 q8
			//fourth : q1 q5 q9
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r7, r3, r7, ror#23					\n\t"

			"veor q1, q1, q12 						\n\t"
			"eor r10, r5, r6, ror#3					\n\t"

			"veor q5, q5, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q9, q9, q12	 					\n\t"
			"eor r6, r3, r6, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r9, ror#5					\n\t"

			"veor q13, q0, q12 						\n\t"
			"add r6, r6, r10 						\n\t"

			"veor q14, q4, q12 						\n\t"
			"eor r9, r3, r9, ror#5					\n\t"

			"veor q15, q8, q12	 					\n\t"
			"eor r10, r2, r8, ror#23				\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"add r9, r9, r10 						\n\t"

			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #29 					\n\t"
			"vshl.i32 q5, q14, #29 					\n\t"
			"vshl.i32 q9, q15, #29 					\n\t"

			"vsri.32  q1, q13, #3  					\n\t"
			"vsri.32  q5, q14, #3  					\n\t"
			"vsri.32  q9, q15, #3	  				\n\t"

			//3-2
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12						\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #27 					\n\t"
			"vshl.i32 q4, q14, #27 					\n\t"
			"vshl.i32 q8, q15, #27	 				\n\t"

			"vsri.32  q0, q13, #5  					\n\t"
			"vsri.32  q4, q14, #5  					\n\t"
			"vsri.32  q8, q15, #5  					\n\t"

			//3-3
			"vdup.32 q12, r3						\n\t"
			"veor q3, q3, q12 						\n\t"
			"veor q7, q7, q12 						\n\t"
			"veor q11, q11, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q2, q12 						\n\t"
			"veor q14, q6, q12 						\n\t"
			"veor q15, q10, q12 					\n\t"

			"vadd.i32 q13, q3, q13 					\n\t"
			"vadd.i32 q14, q7, q14 					\n\t"
			"vadd.i32 q15, q11, q15					\n\t"

			"vshl.i32 q3, q13, #9 					\n\t"
			"vshl.i32 q7, q14, #9 					\n\t"
			"vshl.i32 q11, q15, #9					\n\t"

			"vsri.32  q3, q13, #23  				\n\t"
			"vsri.32  q7, q14, #23  				\n\t"
			"vsri.32  q11, q15, #23 				\n\t"

			//4-1
			//first  : q3 q7 q11
			//second : q0 q4 q8
			//third  : q1 q5 q9
			//fourth : q2 q6 q10
			"ldmia %0!, {r2-r5} 					\n\t"

			"vdup.32 q12, r3						\n\t"
			"eor r8, r3, r8, ror#23					\n\t"

			"veor q2, q2, q12 						\n\t"
			"eor r10, r5, r7, ror#3					\n\t"

			"veor q6, q6, q12 						\n\t"
			"add r8, r8, r10 						\n\t"

			"veor q10, q10, q12	 					\n\t"
			"eor r7, r3, r7, ror#3					\n\t"

			"vdup.32 q12, r5						\n\t"
			"eor r10, r4, r6, ror#5					\n\t"

			"veor q13, q1, q12 						\n\t"
			"add r7, r7, r10 						\n\t"

			"veor q14, q5, q12 						\n\t"
			"eor r6, r3, r6, ror#5					\n\t"

			"veor q15, q9, q12	 					\n\t"
			"eor r10, r2, r9, ror#23				\n\t"

			"vadd.i32 q13, q2, q13 					\n\t"
			"add r6, r6, r10 						\n\t"

			"vadd.i32 q14, q6, q14 					\n\t"
			"vadd.i32 q15, q10, q15					\n\t"

			"vshl.i32 q2, q13, #29 					\n\t"
			"vshl.i32 q6, q14, #29 					\n\t"
			"vshl.i32 q10, q15, #29					\n\t"

			"vsri.32  q2, q13, #3  					\n\t"
			"vsri.32  q6, q14, #3  					\n\t"
			"vsri.32  q10, q15, #3	  				\n\t"

			//4-2
			"vdup.32 q12, r3						\n\t"
			"veor q1, q1, q12 						\n\t"
			"veor q5, q5, q12 						\n\t"
			"veor q9, q9, q12 						\n\t"

			"vdup.32 q12, r4						\n\t"
			"veor q13, q0, q12 						\n\t"
			"veor q14, q4, q12 						\n\t"
			"veor q15, q8, q12						\n\t"

			"vadd.i32 q13, q1, q13 					\n\t"
			"vadd.i32 q14, q5, q14 					\n\t"
			"vadd.i32 q15, q9, q15					\n\t"

			"vshl.i32 q1, q13, #27 					\n\t"
			"vshl.i32 q5, q14, #27 					\n\t"
			"vshl.i32 q9, q15, #27	 				\n\t"

			"vsri.32  q1, q13, #5  					\n\t"
			"vsri.32  q5, q14, #5  					\n\t"
			"vsri.32  q9, q15, #5  					\n\t"

			//4-3
			"vdup.32 q12, r3						\n\t"
			"veor q0, q0, q12 						\n\t"
			"veor q4, q4, q12 						\n\t"
			"veor q8, q8, q12						\n\t"

			"vdup.32 q12, r2						\n\t"
			"veor q13, q3, q12 						\n\t"
			"veor q14, q7, q12 						\n\t"
			"veor q15, q11, q12 					\n\t"

			"vadd.i32 q13, q0, q13 					\n\t"
			"vadd.i32 q14, q4, q14 					\n\t"
			"vadd.i32 q15, q8, q15					\n\t"

			"vshl.i32 q0, q13, #9 					\n\t"
			"vshl.i32 q4, q14, #9 					\n\t"
			"vshl.i32 q8, q15, #9					\n\t"

			"vsri.32  q0, q13, #23  				\n\t"
			"vsri.32  q4, q14, #23  				\n\t"
			"vsri.32  q8, q15, #23 					\n\t"

			//test
			//"vstmia %1!, {q0-q3} 					\n\t"

			//post-processing

			"vswp         d1, d4  	                \n\t"
			"mov r9, r9, ror#23						\n\t"

			"vswp         d3, d6  	                \n\t"
			"mov r8, r8, ror#3						\n\t"

			"vswp         d9, d12  	            \n\t"
			"mov r7, r7, ror#5						\n\t"

			"vswp         d11, d14 	            \n\t"
			"mov r6, r6, ror#23						\n\t"

			"vswp         d17, d20 	            \n\t"
			"vswp         d19, d22 	            \n\t"

			"vtrn.32 q0, q1 						\n\t"
			"vtrn.32 q2, q3 						\n\t"
			"vtrn.32 q4, q5 						\n\t"
			"vtrn.32 q6, q7 						\n\t"
			"vtrn.32 q8, q9 						\n\t"
			"vtrn.32 q10, q11						\n\t"

			//plain-text store
			//12 plain-text
			"vstmia %1!, {q0-q7} 					\n\t"
			"vstmia %1!, {q8-q11} 					\n\t"
			"stmia %1, {r6-r9} 						\n\t"
			/* */
			"vpop         {q4-q7}                 \n\t"
			"pop          {r4-r9}                 \n\t"
			:
			:"r"(&key[0]),"r"(&text[0])
			:
	);
	return;

}

