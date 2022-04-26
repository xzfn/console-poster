#include "console_poster_core.h"


struct FindByString {
	std::string target;
	std::vector<HWND> result;
};

BOOL CALLBACK proc_find_by_title(HWND hwnd, LPARAM lparam) {
	FindByString *f = (FindByString*)lparam;
	const int max_count = 1024;
	CHAR buffer[max_count];
	GetWindowText(hwnd, buffer, max_count);
	std::string current(buffer);
	std::string target = f->target;
	if (current.find(target) != std::string::npos) {
		f->result.push_back(hwnd);
	}
	return TRUE;
}

std::vector<HWND> find_hwnds_by_title(std::string title)
{
	// typedef BOOL(CALLBACK* WNDENUMPROC)(HWND, LPARAM);
	FindByString f;
	f.target = title;
	EnumWindows(&proc_find_by_title, (LPARAM)&f);
	return f.result;
}

void send_string(HWND hwnd, std::string cmd, int return_code)
{
	for (char c : cmd) {
		PostMessage(hwnd, WM_CHAR, c, 0);
		PostMessage(hwnd, WM_DEADCHAR, 0, 0);
	}
	PostMessage(hwnd, WM_CHAR, return_code, 0);
}
