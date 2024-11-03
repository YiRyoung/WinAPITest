#include "PreCompile.h"
#include "EngineWindow.h"

// staitc �ʱ�ȭ
HINSTANCE UEngineWindow::hInstance = nullptr;
std::map<std::string, WNDCLASSEXA> UEngineWindow::WindowClasses;
int WindowCount = 0;    // ���� ������ ������ â ���� 

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:      // ���� â�� �׸� ��
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:    // â�� ������ ��
        --WindowCount;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� Window���� �����ϴ� ������ Ŭ���� ����� ����
void UEngineWindow::EngineWindowInit(HINSTANCE _Instance)
{
    hInstance = _Instance;

    WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;                    // ������ ��Ÿ��
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;                             // ���� ���μ����� �ּ�
    wcex.hIcon = nullptr;                                   // ������ ������ ����
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);          // ������ â Ŀ�� ����
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;                            // ������ â ����� �޴� ����
    wcex.lpszClassName = "Default";                         // �ش� �������� Ŭ���� �̸�(������)
    wcex.hIconSm = nullptr;
    CreateWindowClass(wcex);
}

void UEngineWindow::CreateWindowClass(const WNDCLASSEXA& _Class)
{
    // ���� �⺻���� ��������, find�� ���� ��� ��带 ��ȸ���� �ʰ� 
    // ���� ���� �� �˻� Ʈ���� ������ �̿��� Ű�� �������� ȿ������ Ž���� ������.
    std::map<std::string, WNDCLASSEXA>::iterator EndIter = WindowClasses.end();

    // find �Լ��� �ش��ϴ� Ŭ������ ������ Ŭ���� ���� �߿� �����Ѵٸ� �ش� ��ġ��, 
    // �ƴ϶�� ���� �� �ݺ��ڸ� ��ȯ.
    std::map<std::string, WNDCLASSEXA>::iterator FindIter = WindowClasses.find(std::string(_Class.lpszClassName));

    // find �Լ��� ���� ���� ���� �ƴ϶�� 
    // -> (����Ϸ��� Ŭ������ �̹� �����Ѵٸ�)
    if (EndIter != FindIter)
    {
        MSGASSERT(std::string(_Class.lpszClassName) + "���� �̸��� ������ Ŭ������ �ߺ����� ��ϵǾ����ϴ�.");
        return;
    }

    RegisterClassExA(&_Class);  // Ŭ���� ��� ����

   // std::pair�� ����Ͽ� std::map�� insert�ϴ� �ڵ�
   // std::pair�� first�� key��, second�� value ���� �ǹ���
   // _Class.lpszClassName�� Ű�� _Class�� ���� �����Ѵ�.
    WindowClasses.insert(std::pair{ _Class.lpszClassName, _Class });
}

int UEngineWindow::WindowMessageLoop(UEngineDelegate _StartFunction, UEngineDelegate _FrameFunction)
{
    // �޼��� ������ �����ϱ� ���� Ŭ����
    MSG msg = MSG();

   // ���� ���μ��� ������ �� ������ �Լ��� �����Ѵٸ� ����.
   // StartFunction : ���α׷��� ����ǰ� �� 1�� ����Ǿ�� �ϴ� �Լ�
    if (true == _StartFunction.IsBind())
    {
        _StartFunction();
    }

    // ���� ���� ������ â�� 1�� �̻��� ��� �ݺ���
    while (0 != WindowCount)
    {
        // GetMessage�� �޸� PeekMessage�� ����Ͽ� �޼����� ���� ������ ������� �ʰ�
        // ���� ��쿡�� �޼����� �����ϰ� ť���� �����ϵ��� ��.
        if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // �� �����Ӹ��� ����Ǿ�� �ϴ� �Լ��� �����Ѵٸ� ����
        // �޼��� �켱 ó�� �� ���� ���� �����ϴ� ����
        if (true == _FrameFunction.IsBind())
        {
            _FrameFunction();
        }
    }
    // msg.wParam : PostQuitMessage�� exitCode �Ķ���ͷ� ������ ���� ����Ǿ� ����
    //              -> ���α׷� ���� �ڵ�� ����
    return (int)msg.wParam;
}

UEngineWindow::UEngineWindow()
{
}

UEngineWindow::~UEngineWindow()
{
    // ������ ������ ������� ������ �������� �ϴ� ���� ����
    // �����ֱ� ����

    if (nullptr != WindowImage)
    {
        delete WindowImage;
        WindowImage = nullptr;
    }

    if (nullptr != BackBufferImage)
    {
        delete BackBufferImage;
        BackBufferImage = nullptr;
    }

    if (nullptr != WindowHandle)
    {
        DestroyWindow(WindowHandle);
        WindowHandle = nullptr;
    }
}

// ������ ���� �Լ�
// string : char[]�� ���������� ���� �迭�� ��� ������ �� �ִ�. 
//          std::vector<char>�� ����ϸ� \0�� �⺻������ ���ڿ� ���� �߰��Ѵ�.
// string_view : C++17���� ���ԵǾ����� std::string�� �ܺ� ���ڿ��� �������� �ʰ� 
//               �б� �������� ����� �� �ִ�.
void UEngineWindow::Create(std::string_view _TitleName, std::string_view _ClassName)
{
    // ������ Ŭ���� Ʈ������ �ش� ������ Ŭ������ �����ϴ��� Ȯ��
    // contains : Ư�� ��Ұ� �����̳ʿ� ���ԵǾ� �ִ����� Ȯ���ϴ� �� ����ϴ� �Լ�
    //            C++ 20���� �����Ǵ� ���
    //            vector, list, array ���� ������ �����̳ʿ����� �������� ����
    if (false == WindowClasses.contains(_ClassName.data()))
    {
        MSGASSERT(std::string(_ClassName) + "������� ���� Ŭ������ ������ â�� �����Ϸ� �߽��ϴ�");
        return;
    }

    // ������ â ����
    WindowHandle = CreateWindowA(_ClassName.data(), _TitleName.data(), WS_OVERLAPPEDWINDOW,
        0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    // �����찡 ���������� �������� �ʾҴٸ�
    if (nullptr == WindowHandle)
    {
        MSGASSERT(std::string(_TitleName) + "������ ������ �����߽��ϴ�.");
        return;
    }

    // �������� HDC(�׸��� ����)�� �����ͼ� �� ȭ������ �ʱ�ȭ
    // ���ο� �̹����� ����� ���� �ƴ� �׸��� �۾��� ���� HDC�� �ʱ�ȭ�ϴ� �ڵ�
    HDC WindowMainDC = GetDC(WindowHandle);
    WindowImage = new UEngineWinImage();
    WindowImage->Create(WindowMainDC);
}

void UEngineWindow::Open(std::string_view _TitleName)
{
    // ������� window�� ���ٸ�
    if (0 == WindowHandle)
    {
        // ����Ʈ�� Ŭ���� ���� �� TitleName���� ����
        Create(_TitleName);
    }

    // ���������� �������� �ʾҴٸ� ����
    if (0 == WindowHandle)
    {
        return;
    }

    ShowWindow(WindowHandle, SW_SHOW);  // ������ ������ ����
    UpdateWindow(WindowHandle);         // WindowHandle�� �����쿡 WM_PAINT �޼��� ��� ����
    ++WindowCount;                      // ������ ������ â�� ���� ����
}

void UEngineWindow::SetWindowPosAndScale(FVector2D _Pos, FVector2D _Scale)
{
    // ���� ������ ũ��� �Է¹��� ũ�Ⱑ �ٸ��ٸ� ũ�� ���� �� ����� �����
    if (false == WindowSize.EqualToInt(_Scale))
    {
        if (nullptr != BackBufferImage)
        {
            delete BackBufferImage;
            BackBufferImage = nullptr;
        }

        // ������� ����� �̹����� _Scale ũ��� ����
        BackBufferImage = new UEngineWinImage();
        BackBufferImage->Create(WindowImage, _Scale);
    }

    // ������ ũ�� ����
    WindowSize = _Scale;

    // ������ ũ�⿡ ���� RECT ����ü ����
    // RECT : �������� ���� ���� ũ�⸦ �����ϴ� �簢�� ����ü
    RECT Rc = { 0, 0, _Scale.iX(), _Scale.iY() };

    // Ŭ���̾�Ʈ ���� ũ�⿡ ���� ���� ������ ũ�� ���� �� ��ü ũ�� ���
    //              �׵θ�      ���� ǥ����       �޴�
    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);

    // ������ ��ġ �� ũ�� ����
    //              ������ �ڵ�    Z ����    ������ X    ������ y         ���� ũ��              ���� ũ��     z ���� ���� ���� �÷���
    ::SetWindowPos(WindowHandle, nullptr, _Pos.iX(), _Pos.iY(), (Rc.right - Rc.left), (Rc.bottom - Rc.top), SWP_NOZORDER);
}

