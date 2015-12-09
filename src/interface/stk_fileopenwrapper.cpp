#include "stk_fileopenwrapper.h"
#include <windows.h>

namespace stk {

std::wstring getOpenFileName(sf::Window& window) {
	OPENFILENAME ofn;
	wchar_t charBuffer[260];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = (HWND) window.getSystemHandle();

	ofn.lpstrFile = charBuffer;
	ofn.lpstrFile[0] = L'\0';

	ofn.nMaxFile = sizeof(charBuffer);
	ofn.lpstrFilter = L"All\0*.*\0Pathfinder Maps\0*.pthfndr\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)==TRUE)
		return std::wstring(ofn.lpstrFile);
	return std::wstring(L"error");
}

std::wstring getSaveFileName(sf::Window& window) {
	OPENFILENAME ofn;
	wchar_t charBuffer[260];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = (HWND) window.getSystemHandle();

	ofn.lpstrFile = charBuffer;
	ofn.lpstrFile[0] = L'\0';

	ofn.nMaxFile = sizeof(charBuffer);
	ofn.lpstrFilter = L"All\0*.*\0Pathfinder Maps\0*.pthfndr\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn)==TRUE)
		return std::wstring(ofn.lpstrFile);
	return std::wstring(L"error");
}

bool yesNoWarningMessage(sf::Window& window, std::wstring texte) {
	return (MessageBox((HWND) window.getSystemHandle(), texte.c_str(), L"Attention", MB_YESNO |  MB_ICONWARNING) == IDYES);
}

} // namespace stk
