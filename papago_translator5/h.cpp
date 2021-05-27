#include "h.h"

typedef std::basic_string<TCHAR> tstring;
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