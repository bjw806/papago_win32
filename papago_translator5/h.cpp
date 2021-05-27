#include "h.h"

typedef std::basic_string<TCHAR> tstring;
TCHAR* StringToTCHAR(string& s) {
	tstring tstr;
	const char* all = s.c_str();
	int len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

string retst(string str) {
	return str;
}

const string TCHARToString(const TCHAR* ptsz) {
	int len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	string s = psz;
	delete[] psz;
	return s;
}

wstring s2ws(const string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

LPWSTR ConvertString(const string& instr){
	// Assumes std::string is encoded in the current Windows ANSI codepage
	int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);
	if (bufferlen == 0){
		return 0;
	}
	LPWSTR widestr = new WCHAR[bufferlen + 1];
	::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);
	widestr[bufferlen] = 0;
	return widestr;
}

string translateDLL(string str) {
	ICallClass* cpi = NULL;
	CoInitialize(NULL);

	string result;
	_bstr_t bstrVal = str.c_str();
	HRESULT hr = CoCreateInstance(CLSID_APITranslate, NULL, CLSCTX_INPROC_SERVER, IID_ICallClass, reinterpret_cast<void**>(&cpi));
	if (SUCCEEDED(hr)) {
		result = cpi->translate(bstrVal, "ko", "en");
		return result;
	}

	CoUninitialize();
	return result;
}

string parse(string str) {
	int pos1 = str.find("\"translatedText\":") + 18;
	int pos2 = str.find("\",\"engineType\":");

	return str.substr(pos1, pos2 - pos1);
}

LPWSTR parseLPWSTR(LPWSTR str) {
	string str1 = CW2A(str);
	int pos1 = str1.find("\"translatedText\":") + 18;
	int pos2 = str1.find("\",\"engineType\":");
	string ret = str1.substr(pos1, pos2 - pos1);
	LPWSTR lp = ConvertString(ret);
	return lp;
}

wchar_t* getClip() {
	HWND hwnd = NULL;
	HANDLE hClipboard;
	OpenClipboard(hwnd);
	hClipboard = GetClipboardData(CF_UNICODETEXT);//CF_TEXT -> X
	if (!hClipboard) {
		CloseClipboard();
		SetDlgItemText(hwnd, IDC_OUTPUT2, _T("클립보드에 데이터가 없습니다."));
	}
	wchar_t* data = (wchar_t*)GlobalLock(hClipboard);

	return data;
}

void setClip(wchar_t* str) {
	HANDLE hClipboard;
	HWND hwnd = NULL;
	//OpenClipboard(hDlg);
	GetDlgItemText(hwnd, IDC_OUTPUT, str, 10000);
}

char* UTF8ToANSI(const char* pszCode)
{
	BSTR    bstrWide;
	char* pszAnsi;
	int     nLength;

	// Get nLength of the Wide Char buffer
	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1,
		NULL, NULL);
	bstrWide = SysAllocStringLen(NULL, nLength);

	// Change UTF-8 to Unicode (UTF-16)
	MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);

	// Get nLength of the multi byte buffer
	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[nLength];

	// Change from unicode to mult byte
	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);

	return pszAnsi;
}

_bstr_t setLang(int num) {
	_bstr_t lang;
	switch (num) {
	case 0:
		lang = _T("ko");
		break;
	case 1:
		lang = _T("en");
		break;
	case 2:
		lang = _T("ja");
		break;
	case 3:
		lang = _T("zh-CN");
		break;
	case 4:
		lang = _T("fr");
		break;
	case 5:
		lang = _T("de");
		break;
	}
	return lang;
}