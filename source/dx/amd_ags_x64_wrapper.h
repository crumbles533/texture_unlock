#pragma once
//----------------------header------------------//

#include<iostream>
#include<iomanip>
#include <fstream> 
#include <chrono>
#include <format>
#include<time.h>
#ifndef WINBOOL 
#define WINBOOL bool
#endif
#include <combaseapi.h> 
#include "winver.h"
#include "amd_ags_x64.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "Version.lib")



//----------------------function pointers-------------------------//
#ifndef AMD_AGS_X64_WRAPPER_H
#define AMD_AGS_X64_WRAPPER_H
extern HMODULE agsDLL;

typedef AMD_AGS_API AGSReturnCode(*funcptr_ags)(int, const AGSConfiguration*, AGSContext**, AGSGPUInfo*);
extern funcptr_ags OriginalAGSInitialize;
typedef AMD_AGS_API AGSReturnCode(*funcptr_ags1)(AGSContext*);
extern funcptr_ags1 OriginalAGSDeInitialize;
typedef AMD_AGS_API AGSReturnCode(*funcptr_ags2)(AGSContext**, const AGSConfiguration*, AGSGPUInfo*);
extern funcptr_ags2 OriginalAGSInit;
typedef AMD_AGS_API AGSReturnCode(*funcptr_ags3)(AGSContext*);
extern funcptr_ags3 OriginalAGSDeInit;
extern BOOL check_ags_version();

#endif
//-------------------function definitions-------------------------//

extern "C" AMD_AGS_API extern AGSReturnCode ProxyAGSInitialize(int, const AGSConfiguration *, AGSContext * *, AGSGPUInfo * );
extern "C" AMD_AGS_API extern AGSReturnCode ProxyAGSDeInitialize(AGSContext * );
extern "C" AMD_AGS_API extern AGSReturnCode ProxyAGSInit(AGSContext * *, const AGSConfiguration *, AGSGPUInfo *);
extern "C" AMD_AGS_API extern AGSReturnCode ProxyAGSDeInit(AGSContext *);

