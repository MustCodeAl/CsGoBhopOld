#ifndef BASEPLAYER__
#define BASEPLAYER__

#include "Include.h"

namespace COffsets {
const DWORD m_dwClientState = 0x005B92A4;
const DWORD m_dwLocalPlayer = 0x00A33504;
const DWORD m_dwEntityList = 0x04A4ECA4;
}

// client base player
namespace CBasePlayer {
typedef float vmatrix_t[4][4];

DWORD GetEntityBase(int iEntityNumber);
DWORD GetLocalBase();

DWORD GetEnginePointer();

byte GetEntityTeam(int iEntityNum);
byte GetLocalTeam();

int GetFlags(DWORD Base);
}

#endif

#include "BasePlayer.h"

namespace CBasePlayer {

DWORD GetEntityBase(int iEntityNumber) {
  int address;

  CVariables::Read(CVariables::m_dwClientBase + COffsets::m_dwEntityList + ((iEntityNumber - 1) * 16),
                   &address,
                   sizeof(address));

  return address;
}

DWORD GetLocalBase() {
  int address;

  CVariables::Read(CVariables::m_dwClientBase + COffsets::m_dwLocalPlayer, &address, sizeof(address));

  return address;
}

DWORD GetEnginePointer() {
  DWORD address;

  CVariables::Read(CVariables::m_dwEngineBase + COffsets::m_dwClientState, &address, sizeof(address));

  return address;
}

byte GetEntityTeam(int iEntityNum) {
  byte address;

  CVariables::Read(GetEntityBase(iEntityNum) + 0xF0, &address, sizeof(address));

  return (byte)(address);
}

byte GetLocalTeam() {
  byte address;

  CVariables::Read(GetLocalBase() + 0xF0, &address, sizeof(address));

  return address;
}

int GetFlags(DWORD Base) {
  int address;

  CVariables::Read(Base + 0x100, &address, sizeof(address));

  return address;
}
}

#ifndef BHOP__
#define BHOP__

#include "Include.h"

namespace CBhop {

void Bunnyhop();

}

#endif

#include "Include.h"

namespace CBhop {

void Bunnyhop() {
  DWORD m_dwLocal = CBasePlayer::GetLocalBase();

  int iFlags = CBasePlayer::GetFlags(m_dwLocal);

  DWORD Smash[32];
  char *CSGO = (char *) Smash;

  CSGO[0] = 'C';
  CSGO[1] = 'o';
  CSGO[2] = 'u';
  CSGO[3] = 'n';
  CSGO[4] = 't';
  CSGO[5] = 'e';
  CSGO[6] = 'r';
  CSGO[7] = '-';
  CSGO[8] = 'S';
  CSGO[9] = 't';
  CSGO[10] = 'r';
  CSGO[11] = 'i';
  CSGO[12] = 'k';
  CSGO[13] = 'e';
  CSGO[14] = ':';
  CSGO[15] = ' ';
  CSGO[16] = 'G';
  CSGO[17] = 'l';
  CSGO[18] = 'o';
  CSGO[19] = 'b';
  CSGO[20] = 'a';
  CSGO[21] = 'l';
  CSGO[22] = ' ';
  CSGO[23] = 'O';
  CSGO[24] = 'f';
  CSGO[25] = 'f';
  CSGO[26] = 'e';
  CSGO[27] = 'n';
  CSGO[28] = 's';
  CSGO[29] = 'i';
  CSGO[30] = 'v';
  CSGO[31] = 'e';
  CSGO[32] = 0;

  if (iFlags == 257) {
    SendMessage(FindWindow(NULL, CSGO), WM_KEYDOWN, VK_SPACE, 0x390000);
  }

  if (iFlags == 256) {
    SendMessage(FindWindow(NULL, CSGO), WM_KEYUP, VK_SPACE, 0x390000);
  }

  if (iFlags == 263) {
    SendMessage(FindWindow(NULL, CSGO), WM_KEYDOWN, VK_SPACE, 0x390000);
  }

  if (iFlags == 262) {
    SendMessage(FindWindow(NULL, CSGO), WM_KEYUP, VK_SPACE, 0x390000);
  }
}

}

#include "GetGameModules.h"

namespace CProcess {
bool GetGameModules() {
  if (!CVariables::m_hProcessId) {
    GetProcessIdFromName(CVariables::m_dwProcessId, CVariables::m_chProcessName);
    CVariables::m_hProcessId = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CVariables::m_dwProcessId);
  }

  if (CVariables::m_dwClientBase == NULL) {
    CVariables::m_dwClientBase = CProcess::GetModuleBase(CVariables::m_dwProcessId, "client.dll");
  }

  if (CVariables::m_dwEngineBase == NULL) {
    CVariables::m_dwEngineBase = CProcess::GetModuleBase(CVariables::m_dwProcessId, "engine.dll");
  }

  return true;
}
}

#ifndef GETGAMEMODULES__
#define GETGAMEMODULES__

#include "Include.h"

namespace CProcess {
bool GetGameModules();
}

#endif

#include "GetModuleBase.h"

namespace CProcess {
DWORD GetModuleBase(const DWORD m_dwProcessId, const char *m_chModuleName) {
  HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);
  if (!hSnap) {
    return 0;
  }

  MODULEENTRY32 me;
  me.dwSize = sizeof(MODULEENTRY32);

  DWORD dwReturn = 0;

  if (Module32First(hSnap, &me)) {
    while (Module32Next(hSnap, &me)) {
      if (lstrcmpi(me.szModule, m_chModuleName) == 0) {
        dwReturn = (DWORD) me.modBaseAddr;
        break;
      }
    }
  }
  CloseHandle(hSnap);
  return dwReturn;
}
}

#ifndef GETMODULEBASE__
#define GETMODULEBASE__

#include "Include.h"

namespace CProcess {
DWORD GetModuleBase(const DWORD ProcessId, const char *ModuleName);
}

#endif

#include "GetProcessIdFromName.h"

static bool m_bDoOnce = false;

namespace CProcess {
DWORD GetProcessIdFromName(DWORD &m_dwProcessId, char *m_chProcessName) {
  HWND m_dwWindowHandle = FindWindow(0, m_chProcessName);

  if (m_dwWindowHandle == NULL) {
    CVariables::ThrowError("CS:GO Could not be found.");
  } else {
    if (m_bDoOnce == false) {
      //Beep(700, 500);
      m_bDoOnce = true;
    }
  }

  if (GetWindowThreadProcessId(m_dwWindowHandle, &m_dwProcessId)) {
    return m_dwProcessId;
  } else {
    return false;
  }
}
}

#ifndef GETPROCESSIDFROMNAME__
#define GETPROCESSIDFROMNAME__

#include "Include.h"

namespace CProcess {
DWORD GetProcessIdFromName(DWORD &m_dwProcessId, char *m_chProcessName);
}

#endif

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef INCLUDE__
#define INCLUDE__

#include <Windows.h>

#include <TlHelp32.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <random>
#include <iostream>
#include <time.h>
#include <string>
#include <istream>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

//#include <thread>

using namespace std;

#include "Variables.h"
#include "GetProcessIdFromName.h"
#include "GetModuleBase.h"
#include "GetGameModules.h"
#include "BasePlayer.h"
#include "Bunnyhop.h"

#endif

#include "Include.h"

int main() {
  for (;;) {

    CProcess::GetGameModules();

    if (GetAsyncKeyState(CVariables::m_dwBunnyKey)) {
      if (CVariables::bunnykey != "NULL") {
        CBhop::Bunnyhop();
      }
    }

    Sleep(1);
  }
}

HWND TargetWnd;

int WINAPI
wWinMain(HINSTANCE
hInstance,
HINSTANCE hPrevInstance, PWSTR
pCmdLine,
int nCmdShow
)
{
TargetWnd = FindWindow(0, "Counter-Strike: Global Offensive");
HDC HDC_Desktop = GetDC(TargetWnd);
CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, NULL);

while (!
GetAsyncKeyState(VK_DELETE)
||
CProcess::GetProcessIdFromName(CVariables::m_dwProcessId, CVariables::m_chProcessName
) == NULL) {
Sleep(5);
}
}

#ifndef VARIABLES__
#define VARIABLES__

#include "Include.h"

#define DEG2RAD(x) ( ( float )( x ) * ( float )( ( float )( M_PI ) / 180.0f ) )
#define RAD2DEG(x) ( ( float )( x ) * ( float )( 180.0f / ( float )( M_PI ) ) )
#define M_RADPI 57.295779513082f
#define M_PI 3.14159265358979323846f

namespace CVariables {

extern char *m_chProcessName;
extern DWORD m_dwProcessId;
extern HANDLE m_hProcessId;
extern DWORD m_dwClientBase;
extern DWORD m_dwEngineBase;

int ThrowError(char *m_chError);

bool Read(DWORD_PTR dwAddress, LPVOID lpBuffer, DWORD_PTR dwSize);
bool Write(DWORD_PTR dwAddress, LPCVOID lpBuffer, DWORD_PTR dwSize);

extern DWORD m_dwBunnyKey;
extern char *bunnykey;

}

#endif

#include "Variables.h"

namespace CVariables {

char *m_chProcessName = "Counter-Strike: Global Offensive";
DWORD m_dwProcessId;
HANDLE m_hProcessId;
DWORD m_dwClientBase;
DWORD m_dwEngineBase;

//mouse4 (back mousebtn)
DWORD m_dwBunnyKey = 0x05; //space = 0x20

char *bunnykey;

int ThrowError(char *m_chError) {
  //char *m_chErorr = new char[6];
  //m_chErorr = m_chError;

  //_asm{
  //	push        10h
  //		push        0
  //		mov         eax, [m_chErorr]
  //		push        eax
  //		push        0
  //		call        MessageBox
  //}

  MessageBox(NULL, m_chError, "Error", MB_OK | MB_ICONERROR);

  exit(-1);

  return false;
}

bool Read(DWORD_PTR dwAddress, LPVOID lpBuffer, DWORD_PTR dwSize) {
  SIZE_T Out = NULL;

  return (ReadProcessMemory(m_hProcessId, (LPCVOID) dwAddress, lpBuffer, dwSize, &Out) == TRUE);
}

bool Write(DWORD_PTR dwAddress, LPCVOID lpBuffer, DWORD_PTR dwSize) {
  SIZE_T Out = NULL;

  return (WriteProcessMemory(m_hProcessId, (LPVOID) dwAddress, lpBuffer, dwSize, &Out) == TRUE);
}

}
