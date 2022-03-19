#ifndef UNICODE
#define UNICODE
#endif 



#include <Windows.h>
#include "main.h"



#define GAME_WINDOW_WIDTH 1280
#define GAME_WINDOW_HEIGHT 720


//global variables




// Different things btw, when the snake hits itself, or hits any of the edges, it makes the game to 
// terminate (Game Over), but the application doesn't need to stop

//BOOL GameOver = FALSE;
BOOL ApplicationRunning = TRUE;



static MSG Message = { 0 };
static HWND Handle = NULL;





LRESULT CALLBACK WindowProcedure(HWND Handle, UINT UnsignedMessage, WPARAM wParam, LPARAM lParam);



int _stdcall wWinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PWSTR CommandLine, int CmdShow) {



	const wchar_t CLASS_NAME[] = L"Snake Game - Win 32";


	WNDCLASS WindowClass = { 0 };

	WindowClass.lpfnWndProc = WindowProcedure;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = CLASS_NAME;


	RegisterClass(&WindowClass);



	HWND Handle = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Snake Game Win32",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT,
		NULL, NULL, Instance, NULL

	);




	// Handling fail of creatinh window

	if (Handle == NULL) {


		MessageBoxA(NULL, "Window Creation Failed", "Error!", MB_OK | MB_ICONEXCLAMATION);
		return -1;

	}

	ShowWindow(Handle, CmdShow);


	//----------------------------GAME MAIN LOOP----------------------------\\


	CheckMutex();


	while (ApplicationRunning != FALSE) {


		
		
		ProcessPlayerInput();
		


		while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {

			TranslateMessage(&Message);
			DispatchMessage(&Message);
			


		}

		Sleep(1);


		


	}



	return 0;
}














LRESULT CALLBACK WindowProcedure(HWND Handle, UINT UnsignedMessage, WPARAM wParam, LPARAM lParam) {


	switch (UnsignedMessage) {

		case WM_CREATE:
			OutputDebugStringA("WM_CREATE\n");
			break;


		case WM_CLOSE:
			OutputDebugStringA("WM_CLOSE\n");
			DestroyWindow(Handle);
			break;


		case WM_DESTROY:
			OutputDebugStringA("WM_DESTROY\n");
			ApplicationRunning = FALSE;
			ExitProcess(0);
			break;


		case WM_PAINT:
			PAINTSTRUCT PaintStruct;
			HDC DeviceContext = BeginPaint(Handle, &Paint);




			break;



	}




	return DefWindowProc(Handle, UnsignedMessage, wParam, lParam);


}





void CheckMutex() {

	HANDLE Mutex = NULL;

	Mutex = CreateMutex(NULL, false, L"Snake Game Mutex");
	


	if (GetLastError() == ERROR_ALREADY_EXISTS) {

		MessageBoxA(NULL, "Instance of the program already running!", "Error!", MB_ICONERROR | MB_ICONERROR);
		PostQuitMessage(0);
		ExitProcess(0);

	}


}




void ProcessPlayerInput() {



	// Make game exit the process when player presses 'Esc'


	if (SHORT EscKeyPressed = GetAsyncKeyState(VK_ESCAPE)) {

		PostQuitMessage(0);
		ExitProcess(0);

	}




}