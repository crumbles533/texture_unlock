#pragma once
//----------------------header------------------//
#include<iostream>
#include<iomanip>
#include "detours.h"
#include <fstream> 
#include <chrono>
#include <format>
#include<time.h>
#ifndef WINBOOL 
#define WINBOOL bool
#endif
#include "detours.h"
#include "dxgi/dxgi.h"
#include "dxgi/dxgi1_3.h"
#include "dxgi/dxgi1_4.h"
#include "dxgi/dxgi1_6.h"
#include <combaseapi.h> 
#include "amd_ags_x64_wrapper.h"

//------------------------libraries---------------------------//
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "detours.lib")

//----------------------function pointers-------------------------//
HMODULE dxgiDLL = LoadLibraryA("C:\\Windows\\System32\\dxgi.dll");



typedef HRESULT(*PFN_DXGIDeclareAdapterRemovalSupport)();
PFN_DXGIDeclareAdapterRemovalSupport orgDXGIDeclareAdapterRemovalSupport = (PFN_DXGIDeclareAdapterRemovalSupport)(GetProcAddress(dxgiDLL,"DXGIDeclareAdapterRemovalSupport"));
typedef HRESULT(*PFN_DXGIGetDebugInterface1)(UINT, REFIID, void**);
PFN_DXGIGetDebugInterface1 orgDXGIGetDebugInterface1=(PFN_DXGIGetDebugInterface1)(GetProcAddress(dxgiDLL,"DXGIGetDebugInterface1"));
typedef HRESULT(*PFN_CreateDXGIFactory)(REFIID, void**);
PFN_CreateDXGIFactory orgCreateDXGIFactory= (PFN_CreateDXGIFactory)(GetProcAddress(dxgiDLL,"CreateDXGIFactory"));
typedef HRESULT(*PFN_CreateDXGIFactory1)(REFIID, void**);
PFN_CreateDXGIFactory1 orgCreateDXGIFactory1 = (PFN_CreateDXGIFactory1)(GetProcAddress(dxgiDLL, "CreateDXGIFactory1"));
typedef HRESULT(*PFN_CreateDXGIFactory2)(UINT,REFIID,void**);
PFN_CreateDXGIFactory2 orgCreateDXGIFactory2 = (PFN_CreateDXGIFactory2)(GetProcAddress(dxgiDLL, "CreateDXGIFactory2"));


//------------------log current time---------------------------//
auto now = std::chrono::system_clock::now();
auto in_time = std::chrono::system_clock::to_time_t(now);
struct tm timeInfo;
auto temp = localtime_s(&timeInfo, &in_time);
auto in_time_t = std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S");

