#pragma once
#include <EnginePlatform/EngineWinImage.h>

class UEngineWindow
{
public:
	// static
	// ���� Window �Լ� ����
	static void EngineWindowInit(HINSTANCE _Instance);				// �⺻ ������ ���� �ʱ�ȭ
	static void CreateWindowClass(const WNDCLASSEXA& _Class);		// �� ������ Ŭ���� ���
	
	// ������ �޼��� ���� ����
	static int WindowMessageLoop(UEngineDelegate _StartFunction, UEngineDelegate _FrameFunction);

	// constrcuter destructer
	UEngineWindow();
	~UEngineWindow();

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

	void Create(std::string_view _TitleName, std::string_view _ClassName = "Default");	// ������ ����
	void Open(std::string_view _TitleName = "Window");									// ������ ����

	// ������ ũ�� ��ȯ
	inline FVector2D GetWindowSize() const
	{
		return WindowSize;
	}

	// ������ �̹��� ��ȯ
	inline UEngineWinImage* GetWindowImage() const
	{
		return WindowImage;
	}

	// BackBuffer �̹��� ��ȯ
	inline UEngineWinImage* GetBackBuffer() const
	{
		return BackBufferImage;
	}

	// ������ Ÿ��Ʋ ����
	inline void SetWindowTitle(std::string_view Text)
	{
		SetWindowTextA(WindowHandle, Text.data());
	}

	// ������ ��ġ�� ũ�� ����
	void SetWindowPosAndScale(FVector2D _Pos, FVector2D _Scale);

protected:

private:
	// static
	static HINSTANCE hInstance;									// ���� ������ �ڵ�
	static std::map<std::string, WNDCLASSEXA> WindowClasses;	// ��ϵ� ������ Ŭ������ ������ ����

	FVector2D WindowSize;										// ���� ������ ũ��

	// HDC
	UEngineWinImage* BackBufferImage = nullptr;
	UEngineWinImage* WindowImage = nullptr;

	HWND WindowHandle = nullptr;								// ������ �ڵ�

};

