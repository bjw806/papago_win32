#include <windows.h>
#include <TCHAR.H>
#include <string>
#include <iostream>
#include <fstream>
#include <atlstr.h>
#include <WTypes.h >
#include <oleauto.h>
#include "resource.h"
#include <locale.h>
using namespace std;

//#import "cstest3.tlb" no_namespace named_guids
#import "CSharp4.tlb" no_namespace named_guids
//#import "CSharp5_Json.tlb" no_namespace named_guids
//#import "CSharp6_Json.tlb" no_namespace named_guids
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4278)
#pragma warning(disable: 4996)


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
const string TCHARToString(const TCHAR* ptsz);
wstring s2ws(const std::string& s);
LPWSTR ConvertString(const string& instr);
string translateDLL(string str);
string retst(string str);
TCHAR* StringToTCHAR(string& s);
string parse(string str);
LPWSTR parseLPWSTR(LPWSTR str);
wchar_t* getClip();
void setClip(wchar_t* str);
char* UTF8ToANSI(const char* pszCode);
_bstr_t setLang(int lang);
void rewriteFile(TCHAR filename[], _bstr_t src, _bstr_t trg);	// 파일을 읽고 다른 파일에 쓰는 함수

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);