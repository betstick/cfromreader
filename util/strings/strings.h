#pragma once
#include "../stdafx.hpp"
#include "cmem.h"

namespace cfr
{
	std::string   stringFromChar(char* c, int len);
	std::wstring wstringFromWchar(wchar_t* c, int len);
	std::wstring wstringFromChar(char* c, int len);

	std::string readString(MEM* src);

	std::wstring readWstring(MEM* src);

	std::wstring shortToWideString(std::string str);
};