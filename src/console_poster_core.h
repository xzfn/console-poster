#pragma once

#include <string>
#include <vector>

#include <Windows.h>


std::vector<HWND> find_hwnds_by_title(std::string title);

void send_string(HWND hwnd, std::string cmd, int return_code);
