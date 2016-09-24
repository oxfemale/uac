#include <Windows.h>
#include <direct.h>
#include <stdio.h>
 
STARTUPINFO         si;
PROCESS_INFORMATION pi;
 
char ie[MAX_PATH], module[MAX_PATH];
 
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    GetModuleFileName(0, module, sizeof(module));
 
    snprintf(ie, sizeof(ie), "%c:\\Program Files\\Internet Explorer\\iexplore.exe", module[0]);

    char* dll = "c:\\temp\\inject.dll";
 
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
 
    if(CreateProcess(NULL, ie, NULL, NULL, false, 0, NULL, NULL, &si, &pi) == 0) {
        return GetLastError();
    }
 
    HANDLE baseAddress = VirtualAllocEx(pi.hProcess, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
    if(baseAddress == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }
 
    if(WriteProcessMemory(pi.hProcess, baseAddress, dll, sizeof(dll), NULL) == 0) {
        return GetLastError();
    }
 
    if(CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32"), "LoadLibraryA"), baseAddress, 0, NULL) == 0) {
        return GetLastError();
    }
 
    Sleep(100);
    TerminateThread(pi.hThread, 0);
 
    return 0;
}
