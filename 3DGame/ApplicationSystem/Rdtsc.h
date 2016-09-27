/**
* @file   rdtsc.h
* @brief  CPU�N���b�N
* @author Syota Tachikawa
*/

#ifndef ___RDTSC_H
#define ___RDTSC_H

inline __int64 __fastcall rdtsc()
{
	__asm {
		cpuid
		rdtsc
	}
}

#endif