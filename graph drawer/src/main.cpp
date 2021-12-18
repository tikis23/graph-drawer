#include <Windows.h>
#include "Program.h"

#if _DEBUG

int main()
{
	Program program;
	return 0;
}

#else

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Program program;
    return 0;
}

#endif