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
_bstr_t setLang(int lang);

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);