#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Practice");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd; 
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		500, 300, 300, 300,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	// custom start
#define KB_NUM		4
	RAWINPUTDEVICE Rid[4];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x06;
	Rid[0].dwFlags = RIDEV_INPUTSINK; // RIDEV_NOLEGACY;
	Rid[0].hwndTarget = hWnd;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = RIDEV_INPUTSINK; // RIDEV_NOLEGACY;
	Rid[1].hwndTarget = hWnd;

	Rid[2].usUsagePage = 0x01;
	Rid[2].usUsage = 0x06;
	Rid[2].dwFlags = RIDEV_NOLEGACY;
	Rid[2].hwndTarget = hWnd;

	if (RegisterRawInputDevices(Rid, 2, sizeof(RAWINPUTDEVICE)) == FALSE) {
		printf("Registration Fail!\n");
	} else {
		printf("Registration Success!\n");
	}

	// custom end
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);
		printf("Test start!\n");
		return 0;
	case WM_DESTROY:
		FreeConsole();
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
		printf("left button was pressed\n");
		return 0;
	case WM_RBUTTONDOWN:
		printf("test\n");
		return 0;

	case WM_INPUT: {
		UINT dwSize;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize,
			sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL) {
			printf("error\n");
			return 0;
		}

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
			sizeof(RAWINPUTHEADER));
		RAWINPUT *raw = (RAWINPUT *)lpb;

		if (raw->header.dwType == RIM_TYPEKEYBOARD) {
			printf("keyboard\n");
		}
		
	} // end of WM_INPUT

	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}