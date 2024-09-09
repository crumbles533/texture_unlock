#include "pch.h"
#include "amd_ags_x64_wrapper.h"
HMODULE agsDLL = LoadLibraryA("amd_ags_x64.dll");
funcptr_ags OriginalAGSInitialize = (funcptr_ags)DetourFindFunction("amd_ags_x64.dll", "agsInitialize");
funcptr_ags1 OriginalAGSDeInitialize = (funcptr_ags1)DetourFindFunction("amd_ags_x64.dll", "agsDeInitialize");
funcptr_ags2 OriginalAGSInit = nullptr;
funcptr_ags3 OriginalAGSDeInit = nullptr;



#define assignMemory 8589934592UL

static std::ofstream logFile("AMD_APU_FIX.txt", std::ios_base::out | std::ios_base::app);

extern "C" AMD_AGS_API AGSReturnCode ProxyAGSInitialize(int agsVersion, const AGSConfiguration * config, AGSContext * *context, AGSGPUInfo * gpuInfo)
{
	logFile << " AGSINITIALIZE detouring... " << std::endl; logFile.flush();

	auto rc = OriginalAGSInitialize(agsVersion, config, context, gpuInfo);

		//gpuInfo->driverVersion = "23.40.19.01 - 240215a1 - 400566C - AMD - Software - Adrenalin - Edition";  // 23.40.27.01
		gpuInfo->radeonSoftwareVersion = "24.2.1";
		gpuInfo->devices->adapterString = "AMD Radeon(TM) RX 7900 XTX";
		//gpuInfo->devices->localMemoryInBytes = assignMemory;
		//gpuInfo->devices->sharedMemoryInBytes = assignMemory;
		gpuInfo->devices->isAPU = 0;                      //** NECESSARY IF YOU WANT GAMES LIKE THE LAST OF US P1 TO RUN WITH HIGHER TEXTURE SETTINGS. **// 
														//** BY DEFAULT THOSE SETTINGS ARE LOCKED IF YOU'RE ON AN iGPU/APU. **//
		gpuInfo->devices->deviceId = 0x744c; 
		//gpuInfo->devices->revisionId = 0xc8;
		//gpuInfo->devices->numCUs = 96;
		//gpuInfo->devices->numROPs = 192;
		logFile \
			<< "\n GPU Driver Version : " << gpuInfo->driverVersion\
			<< "\n Radeon Software Version: " << gpuInfo->radeonSoftwareVersion \
			<< "\n adapter string: " << gpuInfo->devices->adapterString\
			<< "\n asicFamily: " << gpuInfo->devices->asicFamily\
			<< "\n deviceId: " << gpuInfo->devices->deviceId\
			<< "\n isAPU: " << gpuInfo->devices->isAPU\
			<< "\n localMemoryInBytes: " << gpuInfo->devices->localMemoryInBytes\
			<< "\n sharedMemoryInBytes: " << gpuInfo->devices->sharedMemoryInBytes\
			<< "\n vendorId; " << gpuInfo->devices->vendorId\
			<< std::endl; logFile.flush();

		return rc;
	//}
}

extern "C" AMD_AGS_API AGSReturnCode ProxyAGSInit(AGSContext * *context, const AGSConfiguration * config,  AGSGPUInfo * gpuInfo)
{
	logFile << " AGSINIT detoured " << std::endl; logFile.flush();

	return  ProxyAGSInitialize(AGS_CURRENT_VERSION, config, context, gpuInfo);
}

extern "C" AMD_AGS_API AGSReturnCode ProxyAGSDeInitialize(AGSContext * context)
{
	logFile << " AGSDEINITIALIZE detoured " << std::endl; logFile.flush();


	return OriginalAGSDeInitialize(context);
}

extern "C" AMD_AGS_API AGSReturnCode ProxyAGSDeInit(AGSContext * context)
{
	logFile    << " AGSDeinit detoured " << std::endl; logFile.flush();

	return ProxyAGSDeInitialize(context);
}





int check_ags_version() {
	logFile << " ----------AGS VERSION ------------- " << std::endl; logFile.flush();

	BOOL query = 0;
	auto file_info_size = GetFileVersionInfoSizeA("amd_ags_x64.dll", NULL);
	if (file_info_size)
	{
		LPVOID lpdata = new BYTE[file_info_size];
		BOOL file_info = GetFileVersionInfoA("amd_ags_x64.dll", NULL, file_info_size, lpdata);
		if (file_info)
		{
			VS_FIXEDFILEINFO* pFileInfo = nullptr;
			UINT len = 0;
			BOOL query = VerQueryValueA(lpdata, "\\", reinterpret_cast<LPVOID*>(&pFileInfo), &len);
			logFile << " Detected Version major: " << HIWORD(pFileInfo->dwFileVersionMS) << std::endl; logFile.flush();
			logFile << " Detected Version minor: " << LOWORD(pFileInfo->dwFileVersionMS)<< std::endl; logFile.flush();


			if (query && HIWORD(pFileInfo->dwFileVersionMS) < 6)
			{
				
				logFile << " ags version is: " << HIWORD(pFileInfo->dwFileVersionMS) << std::endl; logFile.flush();
				logFile << " OLDER VERSION OF AGS DETECTED " << std::endl; logFile.flush();
				OriginalAGSInit = (funcptr_ags2)DetourFindFunction("amd_ags_x64.dll", "agsInit");
				OriginalAGSDeInit = (funcptr_ags3)DetourFindFunction("amd_ags_x64.dll", "agsDeInit");

				logFile << " ------------------ -------------- " << std::endl; logFile.flush();

				return 5;
			

			}
			else
			{
				logFile << " ags version is: " << HIWORD(pFileInfo->dwFileVersionMS) << std::endl; logFile.flush();
				logFile << " ------------------ -------------- " << std::endl; logFile.flush();

				return 6;
			}
		}
		else
		{
			logFile << " ------------------ -------------- " << std::endl; logFile.flush();

			return 0;
		}
	}
	else
	{
		logFile << " ------------------ -------------- " << std::endl; logFile.flush();

		return 0;
	}
}


//extern "C" AMD_AGS_API AGSDriverVersionResult agsCheckDriverVersion(const char* radeonSoftwareVersionReported, unsigned int radeonSoftwareVersionRequired)
//{
//	//std::wofstream logFile("LOG_AGS2.txt", std::ios_base::out | std::ios_base::app);
//	AGSDriverVersionResult dv = orgagsCheckDriverVersion(radeonSoftwareVersionReported, radeonSoftwareVersionRequired);
//	//logFile <<"\n Radeon software version reported: "<< * radeonSoftwareVersionReported << "\n" <<"Required: "<< radeonSoftwareVersionRequired << std::endl; logFile.flush();
//
//	return dv;
//
//}
