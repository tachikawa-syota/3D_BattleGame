/**
* @file   rdtsc.h
* @brief  CPUÉNÉçÉbÉN
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