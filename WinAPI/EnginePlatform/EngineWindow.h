#pragma once
#include <EnginePlatform/EngineWinImage.h>

class UEngineWindow
{
public:
	// static
	// 기존 Window 함수 랩핑
	static void EngineWindowInit(HINSTANCE _Instance);				// 기본 윈도우 설정 초기화
	static void CreateWindowClass(const WNDCLASSEXA& _Class);		// 새 윈도우 클래스 등록
	
	// 윈도우 메세지 루프 실행
	static int WindowMessageLoop(UEngineDelegate _StartFunction, UEngineDelegate _FrameFunction);

	// constrcuter destructer
	UEngineWindow();
	~UEngineWindow();

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

	void Create(std::string_view _TitleName, std::string_view _ClassName = "Default");	// 윈도우 생성
	void Open(std::string_view _TitleName = "Window");									// 윈도우 열기

	// 윈도우 크기 반환
	inline FVector2D GetWindowSize() const
	{
		return WindowSize;
	}

	// 윈도우 이미지 반환
	inline UEngineWinImage* GetWindowImage() const
	{
		return WindowImage;
	}

	// BackBuffer 이미지 반환
	inline UEngineWinImage* GetBackBuffer() const
	{
		return BackBufferImage;
	}

	// 윈도우 타이틀 설정
	inline void SetWindowTitle(std::string_view Text)
	{
		SetWindowTextA(WindowHandle, Text.data());
	}

	// 윈도우 위치와 크기 설정
	void SetWindowPosAndScale(FVector2D _Pos, FVector2D _Scale);

protected:

private:
	// static
	static HINSTANCE hInstance;									// 현재 윈도우 핸들
	static std::map<std::string, WNDCLASSEXA> WindowClasses;	// 등록된 윈도우 클래스를 맵으로 관리

	FVector2D WindowSize;										// 현재 윈도우 크기

	// HDC
	UEngineWinImage* BackBufferImage = nullptr;
	UEngineWinImage* WindowImage = nullptr;

	HWND WindowHandle = nullptr;								// 윈도우 핸들

};

