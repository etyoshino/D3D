#pragma once
#include"myWin.h"
#include<vector>
#include <string>

class DxgiInfoManager
{
public :
	DxgiInfoManager();
	~DxgiInfoManager();
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	void Set();
	std::vector<std::string> GetMessages() const;
private:
	unsigned long long next = 0;
	struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
};

