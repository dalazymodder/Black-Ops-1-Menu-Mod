#include "pch.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <string>
#include <xinput.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <d3d9.h>
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "d3d9.lib")


#pragma region Proxy
#pragma pack(1)

FARPROC p[23] = { 0 };


extern "C" __declspec(naked) void Proxy_Direct3DCreate9()
{
    __asm
    {
        jmp p[0 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Direct3DCreate9Ex()
{
    __asm
    {
        jmp p[1 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Direct3DCreate9On12()
{
    __asm
    {
        jmp p[2 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Direct3DCreate9On12Ex()
{
    __asm
    {
        jmp p[3 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_PSGPError()
{
    __asm
    {
        jmp p[4 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_PSGPSampleTexture()
{
    __asm
    {
        jmp p[5 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Ordinal16()
{
    __asm
    {
        jmp p[6 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Ordinal22()
{
    __asm
    {
        jmp p[7 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_D3DPERF_BeginEvent()
{
    __asm
    {
        jmp p[8 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_D3DPERF_EndEvent()
{
    __asm
    {
        jmp p[9 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_D3DPERF_GetStatus()
{
    __asm
    {
        jmp p[10 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_D3DPERF_QueryRepeatFrame()
{
    __asm
    {
        jmp p[11 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_D3DPERF_SetMarker()
{
    __asm
    {
        jmp p[12 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_D3DPERF_SetOptions()
{
    __asm
    {
        jmp p[13 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_D3DPERF_SetRegion()
{
    __asm
    {
        jmp p[14 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Direct3D9EnableMaximizedWindowedModeShim()
{
    __asm
    {
        jmp p[15 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Ordinal17()
{
    __asm
    {
        jmp p[16 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Ordinal18()
{
    __asm
    {
        jmp p[17 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_DebugSetLevel()
{
    __asm
    {
        jmp p[18 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_DebugSetMute()
{
    __asm
    {
        jmp p[19 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Ordinal19()
{
    __asm
    {
        jmp p[20 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Ordinal23()
{
    __asm
    {
        jmp p[21 * 4];
    }
}

extern "C" __declspec(naked) void Proxy_Direct3DShaderValidatorCreate9()
{
    __asm
    {
        jmp p[22 * 4];
    }
}




#pragma endregion


struct EnumWindowsData {
    DWORD processId;
    std::vector<HWND> windows;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    EnumWindowsData* data = reinterpret_cast<EnumWindowsData*>(lParam);

    DWORD windowProcessId;
    GetWindowThreadProcessId(hwnd, &windowProcessId);

    if (windowProcessId == data->processId && IsWindowVisible(hwnd)) {
        if (GetWindow(hwnd, GW_OWNER) == NULL) {
            data->windows.push_back(hwnd);
        }
    }

    return TRUE;
}






const SHORT STICK_DEADZONE = 8000;  
const SHORT STICK_MAX = 32767;


bool IsStickMoved(SHORT x, SHORT y) {
    return (abs(x) > STICK_DEADZONE || abs(y) > STICK_DEADZONE);
}



void ProcessInputAndSendKeys(HWND mainWindow, int controllerIndex) {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    const LPARAM KEYDOWN_LPARAM = 0x1480001;
    const LPARAM KEYUP_LPARAM = 0xC1480001;
    if (XInputGetState(controllerIndex, &state) == ERROR_SUCCESS) {
        SHORT sThumbLX = state.Gamepad.sThumbLX;
        SHORT sThumbLY = state.Gamepad.sThumbLY;
        WORD wButtons = state.Gamepad.wButtons;
        int pause = 500;

        *(DWORD*)0X0256c7cc; //MenuVerticalIndex

        if (wButtons & XINPUT_GAMEPAD_DPAD_UP ||
            (IsStickMoved(sThumbLX, sThumbLY) && sThumbLY > STICK_DEADZONE)) {
            if(*(DWORD*)0X0256c7cc >= 0) {
                *(DWORD*)0X0256c7cc = *(DWORD*)0X0256c7cc - 0x1;
                Sleep(pause);
            }
        }

        if (wButtons & XINPUT_GAMEPAD_DPAD_DOWN ||
            (IsStickMoved(sThumbLX, sThumbLY) && sThumbLY < -STICK_DEADZONE)) {

            
            *(DWORD*)0X0256c7cc = *(DWORD*)0X0256c7cc + 0x1;
            Sleep(pause);
        }


    }
    
}




extern "C" __declspec(dllexport) void menuPatch() {
   
    int controllerIndex = 0; 

        std::ifstream file("controller_index.txt");
    
       
    if (file.is_open()) {
        std::string line;
        if (std::getline(file, line)) {   
                controllerIndex = std::stoi(line);
               
                controllerIndex = max(0, min(3, controllerIndex));
        }
        file.close();
    }

    while (true) {
        HWND mainHwnd = FindWindow("Call of Duty�: BlackOps", NULL);
        
        
        if (*(DWORD*)0x0BCB3AC == 0x0 || *(DWORD*)0x32DA314 == 0x0) {
            ProcessInputAndSendKeys(mainHwnd, controllerIndex);
        }
        Sleep(50);
    }
}





extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
    static HINSTANCE hL;
    if (reason == DLL_PROCESS_ATTACH)
    {
        hL = LoadLibrary(_T("c:\\Windows\\SysWOW64\\d3d9.dll"));
        if (!hL) return false;
        p[0] = GetProcAddress(hL, "Direct3DCreate9");
        p[1] = GetProcAddress(hL, "Direct3DCreate9Ex");
        p[2] = GetProcAddress(hL, "Direct3DCreate9On12");
        p[3] = GetProcAddress(hL, "Direct3DCreate9On12Ex");
        p[4] = GetProcAddress(hL, "PSGPError");
        p[5] = GetProcAddress(hL, "PSGPSampleTexture");
        p[6] = GetProcAddress(hL, "Ordinal16");
        p[7] = GetProcAddress(hL, "Ordinal22");
        p[8] = GetProcAddress(hL, "D3DPERF_BeginEvent");
        p[9] = GetProcAddress(hL, "D3DPERF_EndEvent");
        p[10] = GetProcAddress(hL, "D3DPERF_GetStatus");
        p[11] = GetProcAddress(hL, "D3DPERF_QueryRepeatFrame");
        p[12] = GetProcAddress(hL, "D3DPERF_SetMarker");
        p[13] = GetProcAddress(hL, "D3DPERF_SetOptions");
        p[14] = GetProcAddress(hL, "D3DPERF_SetRegion");
        p[15] = GetProcAddress(hL, "Direct3D9EnableMaximizedWindowedModeShim");
        p[16] = GetProcAddress(hL, "Ordinal17");
        p[17] = GetProcAddress(hL, "Ordinal18");
        p[18] = GetProcAddress(hL, "DebugSetLevel");
        p[19] = GetProcAddress(hL, "DebugSetMute");
        p[20] = GetProcAddress(hL, "Ordinal19");
        p[21] = GetProcAddress(hL, "Ordinal23");
        p[22] = GetProcAddress(hL, "Direct3DShaderValidatorCreate9");
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)menuPatch, 0, 0, 0);
    }
    if (reason == DLL_PROCESS_DETACH)
        FreeLibrary(hL);
    return TRUE;
}