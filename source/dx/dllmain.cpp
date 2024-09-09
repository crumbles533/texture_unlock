// dllmain.cpp : Defines the entry point for the DLL application.

//----------------------library---------------------------//
#include "pch.h"
#include "Header.h"


//-------------------------------memory---------------------------//

unsigned long long assignedVideoMemory = 8589934592*2 ;
unsigned long long assignedSharedMemory = 8589934592 *2 ;
unsigned long long assignedRAMMemory = 8589934592* 1;
SIZE_T BtoGB(1024 * 1024 * 1024);

//------FOR NVIDIA-------//
//std::wstring gpuName = L"NVIDIA GeForce RTX(TM) 4050";
//UINT vendorID = 0x10DE;
// UINT deviceID = 0x2684
//-----FOR AMD-----//1f00005ddb03f4
std::wstring gpuName = L"AMD Radeon(TM) RX 7900 XTX";
UINT vendorID = 0X1002;
UINT deviceId = 0x744c;    //AMD
//UINT deviceId = 0x15bf;    //AMD



HRESULT createAdapter(IUnknown*);
template <typename u>
int logValue(u);
typedef HRESULT (STDMETHODCALLTYPE *OriginalGetDesc)(IDXGIAdapter* , DXGI_ADAPTER_DESC* );
typedef HRESULT  (STDMETHODCALLTYPE *OriginalGetDesc1)(IDXGIAdapter4*, DXGI_ADAPTER_DESC1*);
typedef HRESULT(STDMETHODCALLTYPE *OriginalGetDesc2)(IDXGIAdapter4*, DXGI_ADAPTER_DESC2*);
typedef HRESULT(STDMETHODCALLTYPE *OriginalGetDesc3)(IDXGIAdapter4*, DXGI_ADAPTER_DESC3*);
typedef HRESULT(STDMETHODCALLTYPE *OriginalQueryVideoMemoryInfo)(IDXGIAdapter3*, UINT,DXGI_MEMORY_SEGMENT_GROUP,DXGI_QUERY_VIDEO_MEMORY_INFO* );
typedef HRESULT(STDMETHODCALLTYPE* OriginalSetVideoMemoryReservation)(IDXGIAdapter3*, UINT, DXGI_MEMORY_SEGMENT_GROUP, UINT);
typedef HRESULT(STDMETHODCALLTYPE* OriginalCheckInterfaceSupport)(REFGUID, LARGE_INTEGER*);

HRESULT proxyGetDesc1(IDXGIAdapter4* pAdapter1, DXGI_ADAPTER_DESC1* desc1);

void DetoursInit();
void DetoursDeInit();
//----------------------------------------------------proxyGetDesc----------------------------------------------//
OriginalGetDesc originalGetDesc; 
OriginalGetDesc1 originalGetDesc1; 
OriginalGetDesc2 originalGetDesc2;
OriginalGetDesc3 originalGetDesc3;
OriginalQueryVideoMemoryInfo orgQueryVideoMemoryInfo;
OriginalSetVideoMemoryReservation orgSetVideoMemoryReservation;
OriginalCheckInterfaceSupport orgCheckInterfaceSupport;

static std::ofstream logFile("AMD_APU_FIX.txt", std::ios_base::out | std::ios_base::app);

 HRESULT __stdcall proxyGetDesc(IDXGIAdapter* This, DXGI_ADAPTER_DESC* desc) 
 {
	 logFile << in_time_t << " LOGGING GPU INFO suceded" << std::endl; logFile.flush();

	auto HR= originalGetDesc(This, desc);
	
	if (logValue(*desc)!=0)
	{
		logFile << in_time_t << " LOGGING GPU INFO FAILED" << std::endl; logFile.flush();
	}
	if (desc->VendorId != 0x14144)
	{
		wcscpy_s(desc->Description, gpuName.c_str());
		desc->DedicatedVideoMemory = assignedVideoMemory;
		desc->SharedSystemMemory = assignedSharedMemory;
		desc->DedicatedSystemMemory = assignedRAMMemory;
		desc->VendorId = vendorID;
		desc->DeviceId = deviceId;

		
	}
	logValue(*desc);
	 return HR;

}

 HRESULT __stdcall proxyGetDesc1(IDXGIAdapter4* This,DXGI_ADAPTER_DESC1* desc1) {
	 logFile << in_time_t << " LOGGING GPU INFO suceeded" << std::endl; logFile.flush();

	auto HR=originalGetDesc1(This, desc1);
	if (logValue(*desc1)!=0)
	{
		logFile << in_time_t << " LOGGING GPU INFO FAILED" << std::endl; logFile.flush();
	}
	if (desc1->VendorId != 0x14144)
	{
		wcscpy_s(desc1->Description, gpuName.c_str());
		desc1->DedicatedVideoMemory = assignedVideoMemory;
		desc1->SharedSystemMemory = assignedSharedMemory;
		desc1->DedicatedSystemMemory = assignedRAMMemory;
		desc1->VendorId = vendorID;
		desc1->DeviceId = deviceId;
	}
	logValue(*desc1);

	return HR;

}


 HRESULT __stdcall proxyGetDesc2(IDXGIAdapter4* This, DXGI_ADAPTER_DESC2* desc2) {
	 logFile << in_time_t << " LOGGING GPU INFO suceded" << std::endl; logFile.flush();

	auto HR = originalGetDesc2(This, desc2);
	if (logValue(*desc2)!=0)
	{
		logFile << in_time_t << " LOGGING GPU INFO FAILED" << std::endl; logFile.flush();
	}
	if (desc2->VendorId != 0x14144)
	{
		wcscpy_s(desc2->Description, gpuName.c_str());
		desc2->DedicatedVideoMemory = assignedVideoMemory;
		desc2->SharedSystemMemory = assignedSharedMemory;
		desc2->DedicatedSystemMemory = assignedRAMMemory;
		desc2->VendorId = vendorID;
		desc2->DeviceId = deviceId;
	}
	logValue(*desc2);

	return HR;

} 

 HRESULT __stdcall proxyGetDesc3(IDXGIAdapter4* This, DXGI_ADAPTER_DESC3* desc3) {
	 logFile << in_time_t << " LOGGING GPU INFO suceded" << std::endl; logFile.flush();

	 auto HR = originalGetDesc3(This, desc3);
	 if (logValue(*desc3) != 0)
	 {
		 logFile << in_time_t << " LOGGING GPU INFO FAILED" << std::endl; logFile.flush();
	 }
	 if (desc3->VendorId != 0x14144)
	 {
	wcscpy_s(desc3->Description, gpuName.c_str());
	 desc3->DedicatedVideoMemory = assignedVideoMemory;
	 desc3->SharedSystemMemory = assignedSharedMemory;
	 desc3->DedicatedSystemMemory = assignedRAMMemory;
	 desc3->VendorId = vendorID;
	 desc3->DeviceId = deviceId;
 }
	logValue(*desc3);

	return HR;
}

 HRESULT __stdcall proxySetVideoMemoryReservation(IDXGIAdapter3* This, UINT NodeIndex, DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup, UINT Reservation)
 {
	 return orgSetVideoMemoryReservation(This, NodeIndex, MemorySegmentGroup, Reservation);
 }
 HRESULT __stdcall proxyQueryVMI(IDXGIAdapter3* This,UINT NodeIndex, DXGI_MEMORY_SEGMENT_GROUP MemorySegmentGroup, DXGI_QUERY_VIDEO_MEMORY_INFO* pVideoMemoryInfo)
 {

	 auto HR = orgQueryVideoMemoryInfo(This,NodeIndex, MemorySegmentGroup, pVideoMemoryInfo);


	 logFile << "\n Budget: " << pVideoMemoryInfo->Budget/BtoGB \
		 << "\n CurrentUsage: " << pVideoMemoryInfo->CurrentUsage/BtoGB \
		 << "\n AvailableForReservation: " << pVideoMemoryInfo->AvailableForReservation/BtoGB\
		 << "\n CurrentReservation: " << pVideoMemoryInfo->CurrentReservation/BtoGB  << std::endl; logFile.flush();
	 
	 //pVideoMemoryInfo->Budget = assignedVideoMemory;
	
	 return HR;
 }


HRESULT HookGetDesc(IDXGIAdapter* This) {

	void** vtable = *(void***)(This);

	const int indexGetDesc = 8;	
	const int indexQueryVMI = 14;
	const int indexSetVideoMemoryReservation = 15;

	DWORD oldProtect;

	VirtualProtect(&(vtable[indexGetDesc]), sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);
	VirtualProtect(&(vtable[indexQueryVMI]), sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);
	VirtualProtect(&(vtable[indexSetVideoMemoryReservation]), sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);

	if (vtable[indexGetDesc] != ((void*)proxyGetDesc)) {
		originalGetDesc = (OriginalGetDesc)vtable[indexGetDesc];
		vtable[indexGetDesc] = reinterpret_cast<void*>(proxyGetDesc);
		orgQueryVideoMemoryInfo = (OriginalQueryVideoMemoryInfo)vtable[indexQueryVMI];
		vtable[indexQueryVMI] = reinterpret_cast<void*>(proxyQueryVMI);
		orgSetVideoMemoryReservation = (OriginalSetVideoMemoryReservation)vtable[indexSetVideoMemoryReservation];
		vtable[indexSetVideoMemoryReservation] = reinterpret_cast<void*>(proxySetVideoMemoryReservation);
	}
	

	VirtualProtect(&(vtable[indexGetDesc]), sizeof(void*), oldProtect, &oldProtect);
	VirtualProtect(&(vtable[indexQueryVMI]), sizeof(void*), oldProtect, &oldProtect);
	VirtualProtect(&(vtable[indexSetVideoMemoryReservation]), sizeof(void*), oldProtect, &oldProtect);

	return S_OK;
}

HRESULT HookGetDesc1(IDXGIAdapter1* This) {

	void** vtable1 = *(void***)(This);
	const int indexGetDesc1 = 10;	
	DWORD oldProtect;

	VirtualProtect(&(vtable1[indexGetDesc1]), sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);	

	if (vtable1[indexGetDesc1] != ((void*)proxyGetDesc1)) {
		originalGetDesc1 = (OriginalGetDesc1)vtable1[indexGetDesc1];
		vtable1[indexGetDesc1] = reinterpret_cast<void*>(proxyGetDesc1);
	}
	VirtualProtect(&(vtable1[indexGetDesc1]), sizeof(void*), oldProtect, &oldProtect);

	return S_OK;
}
HRESULT HookGetDesc2(IDXGIAdapter2* This) {
	DWORD oldProtect;

	void** vtable2 = *(void***)(This);
	const int indexGetDesc2 = 11;
	VirtualProtect(&(vtable2[indexGetDesc2]), sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect); 

	if (vtable2[indexGetDesc2] != ((void*)proxyGetDesc2)) {

		originalGetDesc2 = (OriginalGetDesc2)vtable2[indexGetDesc2];
		vtable2[indexGetDesc2] = reinterpret_cast<void*>(proxyGetDesc2);
	}
	VirtualProtect(&(vtable2[indexGetDesc2]), sizeof(void*), oldProtect, &oldProtect); 

	return S_OK;
}

HRESULT HookGetDesc3(IDXGIAdapter3* This) {
	DWORD oldProtect;

	void** vtable3 = *(void***)(This);
	const int indexGetDesc3 = 18;
	VirtualProtect(&(vtable3[indexGetDesc3]), sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect);

	if (vtable3[indexGetDesc3] != ((void*)proxyGetDesc3)) {

		originalGetDesc3 = (OriginalGetDesc3)vtable3[indexGetDesc3];
		vtable3[indexGetDesc3] = reinterpret_cast<void*>(proxyGetDesc3);
	}
	VirtualProtect(&(vtable3[indexGetDesc3]), sizeof(void*), oldProtect, &oldProtect); 

	return S_OK;
}


//-----------------------------------------------debuginterface---------------------------------------------------------//

extern "C" __declspec(dllexport) HRESULT  DXGIDeclareAdapterRemovalSupport()
{
	return orgDXGIDeclareAdapterRemovalSupport();
}

extern "C" __declspec(dllexport) HRESULT  DXGIGetDebugInterface1(UINT flags, REFIID riid, void** ppDebug)
{
	return orgDXGIGetDebugInterface1(flags,riid, ppDebug);
 }

//----------------------------------------------------CreateDXGIFactory----------------------------------------------------//


extern "C" __declspec(dllexport) HRESULT __stdcall CreateDXGIFactory(REFIID riid, void** factory)
{

	//Call original function
	IDXGIFactory* pFactory;
	auto hr = orgCreateDXGIFactory1(riid, (void**)&pFactory);

	if SUCCEEDED(hr) {
		auto result = createAdapter(pFactory);
		if (result == S_OK)
		{
			*factory = pFactory;
			logFile << in_time_t << " --- CreateDXGIFactory created  ---- \n"; logFile.flush();

		}
		else
		{
			logFile << in_time_t << " --- CreateDXGIFactory failed  ---- \n"; logFile.flush();

			return result;
		}
	}
	else
	{
		logFile << in_time_t << " (original) CreateDXGIFactory failed" << std::endl; logFile.flush();
		return hr;
	}
	logFile << in_time_t << " ----------CreateDXGIFactory created ---------\n "; logFile.flush();
	logFile.close();
	
	return hr;

}


//----------------------------------------------------CreateDXGIFactory1----------------------------------------------------//


extern "C" __declspec(dllexport) HRESULT __stdcall CreateDXGIFactory1(REFIID riid, void** factory)
{

	//Call the original function dxgi.dll to create a factory
	IDXGIFactory1* pFactory;
	auto hr = orgCreateDXGIFactory1( riid, (void**)&pFactory);

	if SUCCEEDED(hr) {
		auto result = createAdapter(pFactory);
		if (result == S_OK)
		{
			*factory = pFactory;
			logFile << in_time_t << " --- CreateDXGIFactory1 created  ---- \n"; logFile.flush();

		}
		else
		{
			return result;
			logFile << in_time_t << " --- CreateDXGIFactory2 failed  ---- \n"; logFile.flush();

		}
	}
	else
	{
		logFile << in_time_t << " (original) CreateDXGIFactory1 failed" << std::endl; logFile.flush();
		return hr;
	}
	logFile << in_time_t << " ------------CreateDXGIFactory1 created ------------\n "; logFile.flush();
	logFile.close();
	
	return hr;

}

//----------------------------------------------------CreateDXGIFactory2----------------------------------------------------//

extern "C" __declspec(dllexport) HRESULT __stdcall CreateDXGIFactory2(UINT flags, REFIID riid, void** factory)
{

	//CreateDXGIFactory2 call
	IDXGIFactory* pFactory;

	auto hr = orgCreateDXGIFactory2(flags, riid, (void**)&pFactory);
	logFile << in_time_t << " --- CreateDXGIFactory2 start 2  ---- \n"; logFile.flush();

	if ( hr == S_OK)
	{
		auto result = createAdapter(pFactory);
		if (result == S_OK)
		{
			*factory = pFactory;
			logFile << in_time_t << " --- CreateDXGIFactory2 created  ---- \n"; logFile.flush();
		}
		else
		{
			std::cout << "CreateDXGIFactory2 failed" << std::endl;
			logFile << in_time_t << " CreateDXGIFactory2 failed \n"; logFile.flush();
			return hr;
		}
	}
	else
	{
		logFile << in_time_t << " (original) CreateDXGIFactory2 failed " << hr << std::endl; logFile.flush();
		return hr;
		
	}

	return hr;
		
}

//----------------------------------------------------Create adapter----------------------------------------------------//


HRESULT createAdapter(IUnknown* pFactory)
{	

		logFile << in_time_t << " Dummy adapters to force the game to look at vTable modifications \n"; logFile.flush();

		IDXGIFactory* Factory;
		if ((pFactory->QueryInterface(__uuidof(IDXGIFactory), (void**)&Factory)) == S_OK)
		{
			IDXGIAdapter* adapter;
			Factory->EnumAdapters(0, &adapter);
			//DXGI_ADAPTER_DESC desc;
			HookGetDesc(adapter);
			
			Factory->Release();
		}


		IDXGIFactory1* Factory1;
		if ((pFactory->QueryInterface(__uuidof(IDXGIFactory1), (void**)&Factory1)) == S_OK)
		{
			IDXGIAdapter1* adapter1;                             //dummy adapter
			Factory1->EnumAdapters1(0, &adapter1);
			HookGetDesc1(adapter1);
			Factory1->Release();
		}

		IDXGIFactory2* Factory2;
		if ((pFactory->QueryInterface(__uuidof(IDXGIFactory2), (void**)&Factory2)) == S_OK)
		{
			IDXGIAdapter2* adapter2;                           //dummy adapter
			Factory2->EnumAdapters1(0, reinterpret_cast<IDXGIAdapter1**>(&adapter2));
			HookGetDesc2(adapter2);
			Factory2->Release();
		}
		
		IDXGIFactory4* Factory4;
		if ((pFactory->QueryInterface(__uuidof(IDXGIFactory4), (void**)&Factory4)) == S_OK)
		{
			IDXGIAdapter3* adapter4;
			Factory4->EnumAdapters1(0, reinterpret_cast<IDXGIAdapter1**>(&adapter4));
			HookGetDesc2(adapter4);

			//adapter4->GetDesc2(&desc);
			//auto LUID = desc.AdapterLuid;
			//DXGI_QUERY_VIDEO_MEMORY_INFO VMI;
			//adapter4->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &VMI);    //adapter4->SetVideoMemoryReservation(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, (assignedRAM / 16));
			//Factory4->EnumAdapterByLuid(LUID, IID_PPV_ARGS(&adapter4));
			//Factory4->EnumWarpAdapter(IID_PPV_ARGS(&adapter4));
			logFile << in_time_t << " FOUR\n"; logFile.flush();
			Factory4->Release();
		}
		
		IDXGIFactory6* Factory6;
		if ((pFactory->QueryInterface(__uuidof(IDXGIFactory6), (void**)&Factory6)) == S_OK)
		{
			IDXGIAdapter4* adapter6;
			Factory6->EnumAdapters1(0, reinterpret_cast<IDXGIAdapter1**>(&adapter6));
			HookGetDesc3(adapter6);
			//Factory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_UNSPECIFIED, __uuidof(IDXGIFactory6),(void **) &adapter6);
			logFile << in_time_t << " SIX\n"; logFile.flush();
			Factory6->Release();
		}
	
	return S_OK;
}


//int logValueIter = 0;

template <typename u>
int logValue(u desc1)
{

		logFile <<in_time_t<< "\n ------LOG MEMORY VALUES ----- \n "<<std::endl;
		logFile<<in_time_t<<"\n Dedicated Video memory : " << desc1.DedicatedVideoMemory / BtoGB \
			<< "\n Shared system memory: " << desc1.SharedSystemMemory / BtoGB \
			<< "\n Dedicated system memory: " << desc1.DedicatedSystemMemory / BtoGB \
			<< "\n VendorId: " << desc1.VendorId  \
			<< "\n DeviceId: " << desc1.DeviceId  \
			<< std::endl; logFile.flush();
		logFile << in_time_t << L" GPU Name: " << desc1.Description << std::endl; logFile.flush();
	return 0;
}



void DetoursInit() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (check_ags_version()>=6) 
	{
		logFile << " THE VERSION NO. IS: " <<check_ags_version()<< std::endl; logFile.flush();

		if (DetourAttach(&(PVOID&)OriginalAGSInitialize, ProxyAGSInitialize) != NO_ERROR)
		{
			logFile << in_time_t << " AGSInitialize cannot be hooked " << std::endl; logFile.flush();
		}
		if (DetourAttach(&(PVOID&)OriginalAGSDeInitialize, ProxyAGSDeInitialize) != NO_ERROR)
		{
			logFile << in_time_t << " AGSDeInitialize cannot be hooked " << std::endl; logFile.flush();

		}

	}
	else if (check_ags_version()==5)
	{
		logFile << " THE VERSION NO. IS: " << check_ags_version() << std::endl; logFile.flush();
		if (DetourAttach(&(PVOID&)OriginalAGSInit, ProxyAGSDeInit) != NO_ERROR)
		{
			logFile << in_time_t << " AGSInit cannot be hooked " << std::endl; logFile.flush();

		}
		if (DetourAttach(&(PVOID&)OriginalAGSDeInit, ProxyAGSDeInit) != NO_ERROR)
		{
			logFile << in_time_t << " AGSDeInit cannot be hooked " << std::endl; logFile.flush();

		}
	}
	else
	{
		logFile << in_time_t << " amd_ags_x64.dll does not exist" << std::endl; logFile.flush();
	}
	DetourTransactionCommit();
		
	
}

void DetoursDeInit() {

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (check_ags_version()>=6)
	{
		if (DetourDetach(&(PVOID&)OriginalAGSInitialize, ProxyAGSInitialize) != NO_ERROR)
		{
			logFile << in_time_t << " (DETACH)AGSInitialize cannot be hooked " << std::endl; logFile.flush();

		}
		if (DetourDetach(&(PVOID&)OriginalAGSDeInitialize, ProxyAGSDeInitialize) != NO_ERROR)
		{
			logFile << in_time_t << " (DETACH)AGSDeInitialize cannot be hooked " << std::endl; logFile.flush();

		}
	}
	else if(check_ags_version()==5)
	{
		if (DetourDetach(&(PVOID&)OriginalAGSInit, ProxyAGSDeInit) != NO_ERROR)
		{
			logFile << in_time_t << " (DETACH)AGSInit cannot be hooked " << std::endl; logFile.flush();

		}
		if (DetourDetach(&(PVOID&)OriginalAGSDeInit, ProxyAGSDeInit) != NO_ERROR)
		{
			logFile << in_time_t << " (DETACH)AGSDeInit cannot be hooked " << std::endl; logFile.flush();

		}
	}
	else
	{
		logFile << in_time_t << " amd_ags_x64.dll does not exist" << std::endl; logFile.flush();

	}
	DetourTransactionCommit();
}


//---------------------------------------------Main---------------------------------------------------//

int main()
{

	////-----------------------------------------------------------------------------------/
	//
	//IDXGIFactory6* pFactory2 ;
	//IDXGIAdapter4* adapter2;
	//DXGI_ADAPTER_DESC desc ;DXGI_QUERY_VIDEO_MEMORY_INFO VMI;

	//auto hc = CreateDXGIFactory2(0,__uuidof(IDXGIFactory2), (void**)&pFactory2);
	//if (hc == S_OK) {
	//	auto hc=pFactory2->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&adapter2));
	//	if (hc == S_OK) {
	//		adapter2->GetDesc(&desc);
	//		adapter2->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &VMI);
	//	}

	//}
	//logValue(desc);
	
	return 0;
}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
 		DetoursInit();
		break;
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_DETACH: 
		DetoursDeInit();
		break;
	}
	return TRUE;
}
