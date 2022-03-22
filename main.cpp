#ifndef UNICODE
#define UNICODE
#endif 



#include <Windows.h>
#include "main.h"



#define GAME_WINDOW_WIDTH 1280
#define GAME_WINDOW_HEIGHT 720


// global variables




// Different things btw, when the snake hits itself, or hits any of the edges, it makes the game to
// terminate (Game Over), but the application doesn't need to stop

//BOOL GameOver = FALSE;
BOOL ApplicationRunning = TRUE;



static MSG Message = { 0 };
HWND GameWindow;
BITMAPINFO GameInfoBitmap;
HBITMAP HandleToBitmap;

void* GameMemoryVariable;




LRESULT CALLBACK WindowProcedure(HWND Handle, UINT UnsignedMessage, WPARAM wParam, LPARAM lParam);



INT WINAPI wWinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PWSTR CommandLine, int CmdShow) {



	const wchar_t CLASS_NAME[] = L"Snake Game Win 32";


	WNDCLASS WindowClass = { };

	WindowClass.lpfnWndProc = WindowProcedure;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = CLASS_NAME;



	RegisterClass(&WindowClass);




	GameWindow = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Snake Game - Win32",
		WS_OVERLAPPEDWINDOW,
		0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT,
		0, 0, Instance, 0

	);

	

	// Handling fail of creating the window

	if (!GameWindow) {



		MessageBox(NULL, L"Window Creation Failed", L"Error!", MB_OK);
		return 0;

	}

	ShowWindow(GameWindow, CmdShow);


	//----------------------------GAME MAIN LOOP----------------------------\\


	CheckMutex();

	CreateBitMapReasource();


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














LRESULT CALLBACK WindowProcedure(HWND Window, UINT UnsignedMessage, WPARAM wParam, LPARAM lParam) {


	switch (UnsignedMessage) {

		case WM_CREATE:
			OutputDebugStringA("WM_CREATE\n");
			break;


		case WM_CLOSE:
			OutputDebugStringA("WM_CLOSE\n");
			DestroyWindow(Window);
			break;


		case WM_DESTROY:
			OutputDebugStringA("WM_DESTROY\n");
			ApplicationRunning = FALSE;
			ExitProcess(0);
			break;


		case WM_PAINT:
			OutputDebugStringA("WM_PAINT\n");
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(GameWindow, &Paint);
			int x_top_position = Paint.rcPaint.top;
			int y_left_position = Paint.rcPaint.left;
			int localWidth = Paint.rcPaint.right - Paint.rcPaint.left;
			int localHeight = Paint.rcPaint.bottom - Paint.rcPaint.top;
			UpdateGameWindow(DeviceContext, x_top_position, x_top_position, localWidth, localHeight);

			EndPaint(GameWindow, &Paint);




			break;



	}



						// REMARK: Never EVER make first argument the global HWND variable 'GameWindow', if so, it 
						// will not return the Handle to the window at CreateWindowEx function, always returning zero

	return DefWindowProc(Window, UnsignedMessage, wParam, lParam);


}





void CheckMutex() {

	HANDLE Mutex = NULL;

	Mutex = CreateMutex(NULL, false, L"Snake Game Mutex");



	if (GetLastError() == ERROR_ALREADY_EXISTS) {

		MessageBoxA(NULL, "Instance of the program already running!", "Error!", MB_ICONERROR | MB_ICONERROR);
		ExitGame();

	}


}




void ProcessPlayerInput() {



	// Make game exit the process when player presses 'Esc'


	if (SHORT EscKeyPressed = GetAsyncKeyState(VK_ESCAPE)) {

		ExitGame();
	}




}



void CreateBitMapReasource() {


	//Setting GameMemoryVariable

	void* GameMemoryVariable = VirtualAlloc(NULL, (GAME_WINDOW_HEIGHT * GAME_WINDOW_WIDTH), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);




	HDC DeviceContext = GetDC(GameWindow);

	GameInfoBitmap.bmiHeader.biSize = sizeof(GameInfoBitmap.bmiHeader);
	GameInfoBitmap.bmiHeader.biWidth = GAME_WINDOW_WIDTH;
	GameInfoBitmap.bmiHeader.biHeight = GAME_WINDOW_HEIGHT;  //REMARK: this makes the coodinate system from top to bottom 
	GameInfoBitmap.bmiHeader.biPlanes = 1;
	GameInfoBitmap.bmiHeader.biBitCount = 32;
	GameInfoBitmap.bmiHeader.biCompression = BI_RGB;
	GameInfoBitmap.bmiHeader.biSize = 0;
	GameInfoBitmap.bmiHeader.biXPelsPerMeter = 0;
	GameInfoBitmap.bmiHeader.biXPelsPerMeter = 0;
	GameInfoBitmap.bmiHeader.biClrUsed = 0;
	GameInfoBitmap.bmiHeader.biClrUsed = 0;


	HBITMAP HandleToBitmap = CreateDIBSection(DeviceContext, &GameInfoBitmap, DIB_RGB_COLORS, &GameMemoryVariable, 0, 0);





}




void UpdateGameWindow(HDC DeviceContext, int x_initial_pos, int y_initial_pos, int width, int height) {

	int StrechResult = StretchDIBits(
		DeviceContext,
		x_initial_pos, y_initial_pos, width, height,
		x_initial_pos, y_initial_pos, width, height,
		&GameInfoBitmap, &GameInfoBitmap,
		DIB_RGB_COLORS, SRCCOPY);






}




void ExitGame() {

	PostQuitMessage(0);
	ExitProcess(0);

}