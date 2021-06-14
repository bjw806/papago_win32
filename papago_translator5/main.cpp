#include "h.h"

HINSTANCE hInst;
HICON hIcon;
HWND hButtonOpenFileDialog;                    // 파일열기 대화상자를 실행하기 위한 버튼의 핸들
HWND hEditFileToBeOpened;                      // 파일의 경로와 이름을 가져오는 에디트 컨트롤의 핸들
HWND     hwnd;

OPENFILENAME OFN;                               // 파일열기 대화상자를 초기화하기 위한 변수
OPENFILENAME SFN;
FILE* fPtr;
const UINT nFileNameMaxLen = 1024;           // 다음 줄에 정의하는 szFileName 문자열의 최대 길이
WCHAR szFileName[nFileNameMaxLen];    // 파일의 경로 및 이름을 복사하기 위한 문자열

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
//	//system("..\CSharp4\bin\x64\Debug\batch.bat");
//	hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDB_PNG1), IMAGE_ICON, 16, 16, LR_DEFAULTSIZE);
//	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, (DLGPROC)&DlgProc);
//	return 0;
//}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	hInst = hInstance;
	HWND     hwnd;
	MSG		 msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("번역맨"),
		WS_OVERLAPPEDWINDOW,
		600, 400, 600, 400,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

#define IDC_BUTTON_SHORT 100
#define IDC_BUTTON_FILE 101
#define IDC_BUTTON_EXIT 102
#define IDC_TEXT_TITLE 103

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	static HWND bShortTrans, bFileTrans, bExit, txtTitle;
	RECT rect;
	static HFONT hTitleFont, hBtnFont;
	static HBRUSH hBrush;
	switch (iMsg)
	{
	case WM_CREATE:
		// 짧은 글 번역, 파일 번역, 종료 버튼 생성
		GetClientRect(hwnd, &rect);
		bShortTrans = CreateWindow(_T("button"), _T("짧은 글 번역"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect.right / 2 - 75, rect.bottom / 4 + 15, 150, 50,
			hwnd, (HMENU)IDC_BUTTON_SHORT, hInst, NULL);
		bFileTrans = CreateWindow(_T("button"), _T("파일 번역"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect.right / 2 - 75, rect.bottom / 2 - 25 + 15, 150, 50,
			hwnd, (HMENU)IDC_BUTTON_FILE, hInst, NULL);
		bExit = CreateWindow(_T("button"), _T("종료"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect.right / 2 - 75, rect.bottom * 3 / 4 - 50 + 15, 150, 50,
			hwnd, (HMENU)IDC_BUTTON_EXIT, hInst, NULL);
		txtTitle = CreateWindow(_T("static"), _T("도와줘요 번역맨 ㅠㅠ"),
			WS_CHILD | WS_VISIBLE | SS_CENTER, rect.right / 2 - 150, 40, 300, 30,
			hwnd, (HMENU)IDC_TEXT_TITLE, hInst, NULL);
		hTitleFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
			VARIABLE_PITCH | FF_ROMAN, _T("210 각설탕 Regular"));
		SendMessage(txtTitle, WM_SETFONT, (WPARAM)hTitleFont, (LPARAM)FALSE);
		hBtnFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
			VARIABLE_PITCH | FF_ROMAN, _T("210 공중전화 R"));
		SendMessage(txtTitle, WM_SETFONT, (WPARAM)hTitleFont, (LPARAM)FALSE);
		SendMessage(bFileTrans, WM_SETFONT, (WPARAM)hBtnFont, (LPARAM)FALSE);
		SendMessage(bShortTrans, WM_SETFONT, (WPARAM)hBtnFont, (LPARAM)FALSE);
		SendMessage(bExit, WM_SETFONT, (WPARAM)hBtnFont, (LPARAM)FALSE);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		break;

		// static text control 의 배경색 변경
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == txtTitle) {
			//SetTextColor((HDC)wParam, RGB(0, 0, 0));
			//SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return (LRESULT)hBrush;
		}
		break;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {

		case IDC_BUTTON_SHORT:	// 짧은글 번역 버튼 누르면 다이얼로그 출력
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, (DLGPROC)&DlgProc);
			break;

		case IDC_BUTTON_FILE:	//긴글 번역 버튼 누르면 다이얼로그 출력		
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), HWND_DESKTOP, (DLGPROC)&DlgProc2);
			break;

		case IDC_BUTTON_EXIT:
			PostQuitMessage(0); break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	ICallClass* cpi = NULL;//DLL 참조
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_APITranslate, NULL, CLSCTX_INPROC_SERVER, IID_ICallClass, reinterpret_cast<void**>(&cpi));

	HWND hCombo1,hCombo2;//combobox컨트롤

	///////////////////////////////
	static _bstr_t source = _T("ko"), target = _T("en");
	TCHAR Items[][20] = { _T("한국어"),_T("영어"),_T("일본어"),_T("중국어"),_T("프랑스어"),_T("독일어")};
	//static string strresult = "";
	static LPWSTR result = _T("");
	int string_length = 0;
	///////////////////////////////
	switch (message){
	case WM_INITDIALOG: {
		//////////Dlg 무조건 가운데로 맞추기
		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		int x, y, width, height;
		RECT rtDesk, rtWindow;
		GetWindowRect(GetDesktopWindow(), &rtDesk);
		GetWindowRect(hDlg, &rtWindow);
		width = rtWindow.right - rtWindow.left;
		height = rtWindow.bottom - rtWindow.top;
		x = (rtDesk.right - width) / 2;
		y = (rtDesk.bottom - height) / 2;
		MoveWindow(hDlg, x, y, width, height, TRUE);
		///////////////////
		hCombo1 = GetDlgItem(hDlg, IDC_SOURCE);
		hCombo2 = GetDlgItem(hDlg, IDC_TARGET);
		hButtonOpenFileDialog = GetDlgItem(hDlg, IDC_LOADFILE);
		hEditFileToBeOpened = GetDlgItem(hDlg, IDC_DETECTOUT);
		for (int i = 0; i < 6; i++) {
			SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)Items[i]);
			SendMessage(hCombo2, CB_ADDSTRING, 0, (LPARAM)Items[i]);
		}
		SendMessage(hCombo1, CB_SETCURSEL, (WPARAM)0, 0);//기본값 설정
		SendMessage(hCombo2, CB_SETCURSEL, (WPARAM)1, 0);
		return TRUE; 
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_TRANSLATE: {
			//입력한 거에서 먼저 "\n" 구분
			//\n 단위로 끊어서 번역하고 이어붙임
			TCHAR str[100000];
			string_length = GetDlgItemText(hDlg, IDC_INPUT, str, 100000);
			
			/////////////////엔터 인식////////////////
			LPWSTR p1 = _T("");
			LPWSTR temp = wcstok(str, _T("\n"));
			int i = 0;
			result = _T("");
			SetDlgItemText(hDlg, IDC_OUTPUT2, _T("Loading"));
			while (temp != NULL) {
				p1 = cpi->translate(temp, source, target);
				result = cpi->connect(result, p1, i);
				temp = wcstok(NULL, _T("\n"));
				i++;
			}
			//////////////////////////////////////////	
			SetDlgItemText(hDlg, IDC_OUTPUT2, _T("Done!"));
			SetDlgItemText(hDlg, IDC_OUTPUT, result);
			return 0;
		}	// case IDC_TRANSLATE		  
		case IDC_DECODE: {
			TCHAR str[100000];
			GetDlgItemText(hDlg, IDC_INPUT, str, 100000);
			LPWSTR lang;
			lang = cpi->language(str);
			SetDlgItemText(hDlg, IDC_DETECTOUT, lang);
			return 0;
		}	// case IDC_DECODE 
		case IDC_SOURCE: {
			if (HIWORD(wParam) == CBN_SELCHANGE) {// 하나가 선택됨(상태 변경)
				hCombo1 = GetDlgItem(hDlg, IDC_SOURCE);
				int num = SendMessage(hCombo1, CB_GETCURSEL, 0, 0);
				source = setLang(num);
				//SetDlgItemText(hDlg, IDC_DETECTOUT, source);
			}
			break;
		}	// case IDC_SOURCE
		case IDC_TARGET: {
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				hCombo2 = GetDlgItem(hDlg, IDC_TARGET);
				int num = SendMessage(hCombo2, CB_GETCURSEL, 0, 0);
				target = setLang(num);
			}
			break; 
		}	// case IDC_TARGET
		case IDC_CLEAR: {
			SetDlgItemText(hDlg, IDC_DETECTOUT, _T("Clear"));
			SetDlgItemText(hDlg, IDC_INPUT, _T(""));
			SetDlgItemText(hDlg, IDC_OUTPUT, _T(""));
			break; }
		case IDC_PASTE: {
			unsigned int priority_list = CF_TEXT;
			char* p_string = NULL;
			//클리보드에 문자열이 저장되어 있는지 확인한다.
			if (::GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT) {
				//클립보드를 연다.
				if (::OpenClipboard(hDlg)) {
					// 클립보드에서 문자열이 저장된 메모리의 핸들 값을 얻는다.
					HANDLE h_clipboard_data = ::GetClipboardData(CF_TEXT);
					if (h_clipboard_data != NULL) {
						// 메모리 핸들 값을 이용하여 실제 사용가능한 주소를 얻는다.
						char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
						// 클립보드에 저장된 문자열의 길이를 구한다. ('\0'포함 크기)
						int string_len = strlen(p_clipboard_data) + 1;
						// 클립보드에 저장된 문자열을 저장하기 위해 메모리를 할당한다.
						p_string = new char[string_len];
						// 할당된 메모리에 클리보드 문자열을 복사한다.
						memcpy(p_string, p_clipboard_data, string_len);
						// 문자열을 복사하기 위해서 Lock했던 메모리를 해제한다.
						::GlobalUnlock(h_clipboard_data);
						SetDlgItemText(hDlg, IDC_DETECTOUT, _T("Paste"));
					}
					::CloseClipboard();
				}
			}
			// 클립보드 문자열을 복사한 메모리의 주소를 반환한다.
			SetDlgItemText(hDlg, IDC_INPUT, CA2W(p_string));
			break;}
		case IDC_COPY: {
			cpi->setclipboard(result);
			SetDlgItemText(hDlg, IDC_DETECTOUT, _T("Copied"));
			break; }
     	case IDOK:
		case IDCANCEL: {
			EndDialog(hDlg, 0);
			return TRUE;
			break; }
		}
	}
	}
	CoUninitialize();
	return FALSE;
}



BOOL CALLBACK DlgProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	ICallClass* cpi = NULL;//DLL 참조
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_APITranslate, NULL, CLSCTX_INPROC_SERVER, IID_ICallClass, reinterpret_cast<void**>(&cpi));

	HWND hCombo1, hCombo2;//combobox컨트롤
	static _bstr_t source = _T("ko"), target = _T("en");
	TCHAR Items[][20] = { _T("한국어"),_T("영어"),_T("일본어"),_T("중국어"),_T("프랑스어"),_T("독일어") };
	static LPWSTR result = _T("");
	int string_length = 0;

	//여기부터 수정하던 부분임
	TCHAR strrr[100000];
	LPWSTR p1 = _T("");
	LPWSTR temp = wcstok(strrr, _T("\n"));
	int i = 0;
	result = _T("");


	switch (message) {

	case WM_INITDIALOG: {
		int x, y, width, height;
		RECT rtDesk, rtWindow;
		GetWindowRect(GetDesktopWindow(), &rtDesk);
		GetWindowRect(hDlg, &rtWindow);
		width = rtWindow.right - rtWindow.left;
		height = rtWindow.bottom - rtWindow.top;
		x = (rtDesk.right - width) / 2;
		y = (rtDesk.bottom - height) / 2;
		MoveWindow(hDlg, x, y, width, height, TRUE);

		/////////////////////////
		hCombo1 = GetDlgItem(hDlg, IDC_SOURCE);
		hCombo2 = GetDlgItem(hDlg, IDC_TARGET);
		hButtonOpenFileDialog = GetDlgItem(hDlg, IDC_LOADFILE);
		hEditFileToBeOpened = GetDlgItem(hDlg, IDC_DETECTOUT);
		for (int i = 0; i < 6; i++) {
			SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)Items[i]);
			SendMessage(hCombo2, CB_ADDSTRING, 0, (LPARAM)Items[i]);
		}
		SendMessage(hCombo1, CB_SETCURSEL, (WPARAM)0, 0);//기본값 설정
		SendMessage(hCombo2, CB_SETCURSEL, (WPARAM)1, 0);
		return TRUE;
	}
	case WM_COMMAND: {

		switch (LOWORD(wParam)) {
		case IDCANCEL: {
			EndDialog(hDlg, 0);
			return TRUE;
			break;
		}
		case IDC_SOURCE: {
			if (HIWORD(wParam) == CBN_SELCHANGE) {// 하나가 선택됨(상태 변경)
				hCombo1 = GetDlgItem(hDlg, IDC_SOURCE);
				int num = SendMessage(hCombo1, CB_GETCURSEL, 0, 0);
				source = setLang(num);
			}
			break;
		}
		case IDC_TARGET: {
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				hCombo2 = GetDlgItem(hDlg, IDC_TARGET);
				int num = SendMessage(hCombo2, CB_GETCURSEL, 0, 0);
				target = setLang(num);
			}
			break;
		}
		case IDC_TRANSFILE: {
			// OPENFILENAME형 변수의 멤버들의 값을 설정
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFilter = _T("All Files(*.*)\0*.*\0");

			OFN.lpstrFile = szFileName;
			OFN.nMaxFile = nFileNameMaxLen;
			//----------------------------------------------------------------
			// 파일열기 대화상자를 열고, 선택된 파일의 이름을 에디트 박스로 복사
			if (0 != GetOpenFileName(&OFN)) {
				// 선택한 파일을 열어 번역후 새 파일 생성하는 함수 호출	(h.cpp에 정의되어있음)
				rewriteFile(OFN.lpstrFile, source, target);	
				//cpi->transFile(OFN.lpstrFile, source, target);
				TCHAR str1[20];
				_stprintf_s(str1, _T("파일 번역 완료!"));
				MessageBox(hDlg, str1, _T("확인"), MB_OK);
			}
			break;
		}	// case IDC_TRANSFILE
		}	// switch (LOWORD(wParam))

	
	}	// case WM_COMMAND:
	}	// switch (message)
	return FALSE;
}