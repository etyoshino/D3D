#pragma once
#include"Window.h"
#include"MyTimer.h"

class App
{
public:
	App();
	// Ö¡/ÏûÏ¢Ñ­»·
	int Go();
	~App();
private:
	void DoFrame();
private:
	Window wnd;
	MyTimer timer;
	std::vector<std::unique_ptr<class Box>> boxes;
};
