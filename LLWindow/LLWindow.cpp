#include "LLWindow.h"
#include <thread>
#include <iostream>

wstring LLWindow::class_suffix = L"";
static shared_ptr<LLWindow> gwindow = nullptr;

LLWindow::LLWindow() : LLWindow(500, 500, 3)
{
	
}

LLWindow::LLWindow(int width, int heigth, int chnl_count) : m_hwnd(nullptr),
m_width(width), m_height(heigth), m_chnl_count(chnl_count)
{
	LLWindow::class_suffix += L"-";

	m_wclass = LLWindow::class_suffix + LPlayerWinClass;
	m_wtitle = LPlayerWinTitle;
	memset(&m_infohead, 0, sizeof(m_infohead));

	m_video_height = heigth;
	m_video_width = width;

	while (m_video_width % 4)
	{
		m_video_width++;
	}

	m_infohead.biSize = sizeof(m_infohead);
	m_infohead.biWidth = m_video_width;
	m_infohead.biHeight = m_video_height;
	m_infohead.biPlanes = 1;
	m_infohead.biBitCount = 24;
	m_infohead.biCompression = 0;
	m_infohead.biSizeImage = m_video_width * m_video_height * 3;

	m_img_buf = new char[m_video_width * m_video_height * 3];

	memset(m_img_buf, 0, m_video_width * m_video_height * 3);
}

LLWindow::~LLWindow()
{
	delete m_img_buf;
	cout << "destruct" << endl;
}

HWND LLWindow::get_hwnd()
{
	return m_hwnd;
}

void LLWindow::draw_img(char * data)
{	
	int n_line_pix = m_width * 3;
	if (1 == m_chnl_count) 
	{
		for (size_t i = 0; i < m_height; i++)
		{
			//need optimization
			for (size_t j = 0; j < m_width; j++)
			{
				*(m_img_buf + n_line_pix * (m_video_height - i - 1) + 3 * j + 0) = *(data + i * m_width + j);
				*(m_img_buf + n_line_pix * (m_video_height - i - 1) + 3 * j + 1) = *(data + i * m_width + j);
				*(m_img_buf + n_line_pix * (m_video_height - i - 1) + 3 * j + 2) = *(data + i * m_width + j);
			}
		}
	}
	else 
	{

		for (int i = 0; i < m_height; ++i)
		{
			memcpy(m_img_buf + i * n_line_pix, data + (m_height - i - 1) * n_line_pix, n_line_pix);
		}
	}

	::StretchDIBits(m_hdc, 0, 0, m_video_width, m_video_height, 0, 0, m_video_width, m_video_height,
		m_img_buf, (LPBITMAPINFO)&m_infohead, DIB_RGB_COLORS, SRCCOPY);

}

void LLWindow::paint_window() 
{
	RECT rect = { 0 };
	GetWindowRect(m_hwnd, &rect);
	::StretchDIBits(m_hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0, 0, m_video_width, m_video_height,
		m_img_buf, (LPBITMAPINFO)&m_infohead, DIB_RGB_COLORS, SRCCOPY);
}

int LLWindow::register_class()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = LLWindow::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, L"");
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = L"";
	wcex.lpszClassName = m_wclass.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, L"");

	return RegisterClassEx(&wcex);
}

void LLWindow::start_play()
{
	m_start_time = chrono::steady_clock::now();
}

void LLWindow::wait_times(long long int duration)
{
	long long int duration_ = 0;
	do 
	{
		auto cur_point = chrono::steady_clock::now();
		duration_ = (chrono::duration_cast<chrono::milliseconds>(cur_point - m_start_time)).count();

	} while (duration_ < duration);
}

int LLWindow::init_instance()
{

	m_hwnd = CreateWindow(m_wclass.c_str(), m_wtitle.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, m_width, m_height, NULL, NULL, m_hInst, NULL);

	m_hdc = GetDC(m_hwnd);

	if (!m_hwnd)
	{
		return FALSE;
	}

	ShowWindow(m_hwnd, 9);
	UpdateWindow(m_hwnd);
	return true;

}

int LLWindow::show_window()
{
	cout << "count:" << shared_from_this().use_count() << endl;
	WM_DRAW_IMAGE = RegisterWindowMessage(L"LipracerMsg");

	if (!WM_DRAW_IMAGE)
	{
		cout << "Register fail!!!" << endl;
		return 0;
	}
	cout << "count:" << shared_from_this().use_count() << endl;

	auto self(shared_from_this());
	gwindow = self;

	unique_lock<mutex> uqlck(m_window_start_mtx);
	auto msg_loop = [this, self]() -> int 
	{
		//this_thread::sleep_for(chrono::milliseconds(10000));
		cout << "count:" << self.use_count() << endl;
		MSG msg;

		m_hInst = GetModuleHandle(0);

		do
		{
			if (!register_class())
			{
				break;
			}

			if (!init_instance())
			{
				break;
			}
		} while (0);
		
		cout << "create window notify_one!!!" << endl;
		m_window_start_cdv.notify_one();

		while (GetMessage(&msg, m_hwnd, 0, 0/*, PM_REMOVE*/) > 0)
		{
			if (msg.message == WM_QUIT || msg.message == WM_QUIT || msg.message == WM_DESTROY)
			{
				cout << "quit thread\n";
				break;
			}
			if (msg.message == WM_PAINT)
			{

			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		return (int)msg.wParam;
	};

	thread th_msg_loop(msg_loop);
	th_msg_loop.detach();

	m_window_start_cdv.wait(uqlck, [this]() { return m_hwnd; });

	cout << "create window succeed!!!" << endl;

	return 0;
	
}

LRESULT LLWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;


	switch (message)
	{
	case WM_COMMAND:
	
		break;
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		gwindow->paint_window();
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
	
		PostQuitMessage(0);
		break;
	default:
		
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
