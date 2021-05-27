#include "h.h"

HINSTANCE hInst;
HICON hIcon;
HWND hButtonOpenFileDialog;                    // 파일열기 대화상자를 실행하기 위한 버튼의 핸들
HWND hEditFileToBeOpened;                      // 파일의 경로와 이름을 가져오는 에디트 컨트롤의 핸들

OPENFILENAME OFN;                               // 파일열기 대화상자를 초기화하기 위한 변수
const UINT nFileNameMaxLen = 1024;           // 다음 줄에 정의하는 szFileName 문자열의 최대 길이
WCHAR szFileName[nFileNameMaxLen];    // 파일의 경로 및 이름을 복사하기 위한 문자열

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
	//system("..\CSharp4\bin\x64\Debug\batch.bat");
	hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDB_PNG1), IMAGE_ICON, 16, 16, LR_DEFAULTSIZE);
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, (DLGPROC)&DlgProc);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	ICallClass* cpi = NULL;//DLL 참조
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_APITranslate, NULL, CLSCTX_INPROC_SERVER, IID_ICallClass, reinterpret_cast<void**>(&cpi));

	HDC hdc;
	HWND hCombo1,hCombo2;//combobox컨트롤
	HANDLE hFile;//file 입출력

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
			//result = cpi->translate(str,source,target);//c#의 string = LPCWSTR;;;;;ㅅㅂ
			
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
			return 0;}		  
		case IDC_DECODE: {
			TCHAR str[100000];
			GetDlgItemText(hDlg, IDC_INPUT, str, 100000);
			LPWSTR lang;
			lang = cpi->language(str);
			SetDlgItemText(hDlg, IDC_DETECTOUT, lang);
			return 0; }
		case IDC_SOURCE: {
			if (HIWORD(wParam) == CBN_SELCHANGE) {// 하나가 선택됨(상태 변경)
				hCombo1 = GetDlgItem(hDlg, IDC_SOURCE);
				int num = SendMessage(hCombo1, CB_GETCURSEL, 0, 0);
				source = setLang(num);
				//SetDlgItemText(hDlg, IDC_DETECTOUT, source);
			}
			break;}
		case IDC_TARGET: {
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				hCombo2 = GetDlgItem(hDlg, IDC_TARGET);
				int num = SendMessage(hCombo2, CB_GETCURSEL, 0, 0);
				target = setLang(num);
			}
			break; }
		case IDC_LOADFILE: {
			// OPENFILENAME형 변수의 멤버들의 값을 설정
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hDlg;
			OFN.lpstrFilter = _T("All Files(*.*)\0*.*\0");

			OFN.lpstrFile = szFileName;
			OFN.nMaxFile = nFileNameMaxLen;
			// 파일열기 대화상자를 열고, 선택된 파일의 이름을 에디트 박스로 복사
			if (0 != GetOpenFileName(&OFN)) {
				SetWindowText(hEditFileToBeOpened, _T("File Open"));//OFN.lpstrFile (파일이름)
				LPWSTR str = cpi->readfile(OFN.lpstrFile);
				//유니코드 문자가 다 깨져서 C#으로 우회
				SetDlgItemText(hDlg, IDC_INPUT, str);
				//in.close();
			}
			break;}
		case IDC_SAVEFILE: {


			break;}
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