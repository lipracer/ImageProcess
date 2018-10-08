#ifndef _LLWindow_H_
#define _LLWindow_H_


#include <Windows.h>
#include <string>
#include <mutex>
#include <condition_variable>
#include <chrono>

#define LPlayerWinClass L"LipracerWindow"
#define LPlayerWinTitle L"LLPlayer"

using namespace std;

class LLWindow :  public std::enable_shared_from_this<LLWindow>
{
public:
	LLWindow();
	LLWindow(int width, int heigth, int chnl_count);
	virtual ~LLWindow();
	
	int show_window();
	HWND get_hwnd();
	void draw_img(char* data);

	void start_play();
	void wait_times(long long int duration);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	friend void draw_window();
private:
	int init_instance();
	int register_class();

	static wstring class_suffix;



public:
private:
	
	mutex m_window_start_mtx;
	condition_variable m_window_start_cdv;

	HINSTANCE m_hInst;
	HWND m_hwnd;
	HDC m_hdc;

	wstring m_wclass;
	wstring m_wtitle;

	int m_chnl_count;

	int m_width;
	int m_height;

	int m_video_width;
	int m_video_height;

	tagBITMAPINFOHEADER m_infohead;
	char *m_img_buf;

	unsigned int WM_DRAW_IMAGE;

	chrono::time_point<chrono::steady_clock> m_start_time;

};

#endif