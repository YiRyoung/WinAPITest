#include "PreCompile.h"
#include "EngineWindow.h"

// staitc 초기화
HINSTANCE UEngineWindow::hInstance = nullptr;
std::map<std::string, WNDCLASSEXA> UEngineWindow::WindowClasses;
int WindowCount = 0;    // 현재 생성된 윈도우 창 개수 

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:      // 메인 창을 그릴 때
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:    // 창이 삭제될 때
        --WindowCount;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 기존 Window에서 제공하는 윈도우 클래스 등록을 랩핑
void UEngineWindow::EngineWindowInit(HINSTANCE _Instance)
{
    hInstance = _Instance;

    WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;                    // 윈도우 스타일
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;                             // 현재 프로세스의 주소
    wcex.hIcon = nullptr;                                   // 윈도우 아이콘 설정
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);          // 윈도우 창 커서 설정
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;                            // 윈도우 창 상단의 메뉴 설정
    wcex.lpszClassName = "Default";                         // 해당 윈도우의 클래스 이름(구분자)
    wcex.hIconSm = nullptr;
    CreateWindowClass(wcex);
}

void UEngineWindow::CreateWindowClass(const WNDCLASSEXA& _Class)
{
    // 맵의 기본적인 사용법으로, find를 통해 모든 노드를 순회하지 않고 
    // 이진 레드 블랙 검색 트리의 구조를 이용해 키를 기준으로 효율적인 탐색을 수행함.
    std::map<std::string, WNDCLASSEXA>::iterator EndIter = WindowClasses.end();

    // find 함수는 해당하는 클래스가 윈도우 클래스 노드들 중에 존재한다면 해당 위치를, 
    // 아니라면 맵의 끝 반복자를 반환.
    std::map<std::string, WNDCLASSEXA>::iterator FindIter = WindowClasses.find(std::string(_Class.lpszClassName));

    // find 함수의 값이 맵의 끝이 아니라면 
    // -> (등록하려는 클래스가 이미 존재한다면)
    if (EndIter != FindIter)
    {
        MSGASSERT(std::string(_Class.lpszClassName) + "같은 이름의 윈도우 클래스를 중복으로 등록되었습니다.");
        return;
    }

    RegisterClassExA(&_Class);  // 클래스 등록 진행

   // std::pair를 사용하여 std::map에 insert하는 코드
   // std::pair의 first는 key값, second는 value 값을 의미함
   // _Class.lpszClassName을 키로 _Class의 값을 저장한다.
    WindowClasses.insert(std::pair{ _Class.lpszClassName, _Class });
}

int UEngineWindow::WindowMessageLoop(UEngineDelegate _StartFunction, UEngineDelegate _FrameFunction)
{
    // 메세지 정보를 저장하기 위한 클래스
    MSG msg = MSG();

   // 만약 프로세스 시작할 때 실행할 함수가 존재한다면 실행.
   // StartFunction : 프로그램이 실행되고 딱 1번 실행되어야 하는 함수
    if (true == _StartFunction.IsBind())
    {
        _StartFunction();
    }

    // 실행 중인 윈도우 창이 1개 이상일 경우 반복됨
    while (0 != WindowCount)
    {
        // GetMessage와 달리 PeekMessage를 사용하여 메세지가 있을 때까지 대기하지 않고
        // 있을 경우에는 메세지를 저장하고 큐에서 제거하도록 함.
        if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // 매 프레임마다 실행되어야 하는 함수가 존재한다면 실행
        // 메세지 우선 처리 후 게임 엔진 실행하는 순서
        if (true == _FrameFunction.IsBind())
        {
            _FrameFunction();
        }
    }
    // msg.wParam : PostQuitMessage의 exitCode 파라미터로 설정한 값이 저장되어 있음
    //              -> 프로그램 종료 코드로 전달
    return (int)msg.wParam;
}

UEngineWindow::UEngineWindow()
{
}

UEngineWindow::~UEngineWindow()
{
    // 릴리즈 순서는 만들어진 순서의 역순으로 하는 것을 권장
    // 생명주기 때문

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

// 윈도우 생성 함수
// string : char[]과 가변적으로 문자 배열을 담아 관리할 수 있다. 
//          std::vector<char>와 비슷하며 \0를 기본적으로 문자열 끝에 추가한다.
// string_view : C++17에서 도입되었으며 std::string의 외부 문자열을 복사하지 않고 
//               읽기 전용으로 사용할 수 있다.
void UEngineWindow::Create(std::string_view _TitleName, std::string_view _ClassName)
{
    // 윈도우 클래스 트리에서 해당 윈도우 클래스가 존재하는지 확인
    // contains : 특정 요소가 컨테이너에 포함되어 있는지를 확인하는 데 사용하는 함수
    //            C++ 20부터 제공되는 기능
    //            vector, list, array 같은 순차적 컨테이너에서는 제공되지 않음
    if (false == WindowClasses.contains(_ClassName.data()))
    {
        MSGASSERT(std::string(_ClassName) + "등록하지 않은 클래스로 윈도우 창을 생성하려 했습니다");
        return;
    }

    // 윈도우 창 생성
    WindowHandle = CreateWindowA(_ClassName.data(), _TitleName.data(), WS_OVERLAPPEDWINDOW,
        0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    // 윈도우가 정상적으로 생성되지 않았다면
    if (nullptr == WindowHandle)
    {
        MSGASSERT(std::string(_TitleName) + "윈도우 생성에 실패했습니다.");
        return;
    }

    // 윈도우의 HDC(그리기 도구)를 가져와서 빈 화면으로 초기화
    // 새로운 이미지를 만드는 것이 아닌 그리기 작업을 위해 HDC를 초기화하는 코드
    HDC WindowMainDC = GetDC(WindowHandle);
    WindowImage = new UEngineWinImage();
    WindowImage->Create(WindowMainDC);
}

void UEngineWindow::Open(std::string_view _TitleName)
{
    // 만들어진 window가 없다면
    if (0 == WindowHandle)
    {
        // 디폴트로 클래스 생성 후 TitleName으로 변경
        Create(_TitleName);
    }

    // 정상적으로 생성되지 않았다면 종료
    if (0 == WindowHandle)
    {
        return;
    }

    ShowWindow(WindowHandle, SW_SHOW);  // 생성된 윈도우 띄우기
    UpdateWindow(WindowHandle);         // WindowHandle의 윈도우에 WM_PAINT 메세지 즉시 전달
    ++WindowCount;                      // 생성된 윈도우 창의 개수 증가
}

void UEngineWindow::SetWindowPosAndScale(FVector2D _Pos, FVector2D _Scale)
{
    // 현재 윈도우 크기와 입력받은 크기가 다르다면 크기 갱신 및 백버퍼 재생성
    if (false == WindowSize.EqualToInt(_Scale))
    {
        if (nullptr != BackBufferImage)
        {
            delete BackBufferImage;
            BackBufferImage = nullptr;
        }

        // 재생성된 백버퍼 이미지를 _Scale 크기로 생성
        BackBufferImage = new UEngineWinImage();
        BackBufferImage->Create(WindowImage, _Scale);
    }

    // 윈도우 크기 갱신
    WindowSize = _Scale;

    // 윈도우 크기에 맞춰 RECT 구조체 설정
    // RECT : 윈도우의 내부 영역 크기를 설정하는 사각형 구조체
    RECT Rc = { 0, 0, _Scale.iX(), _Scale.iY() };

    // 클라이언트 영역 크기에 맞춰 실제 윈도우 크기 조정 및 전체 크기 계산
    //              테두리      제목 표시줄       메뉴
    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);

    // 윈도우 위치 및 크기 설정
    //              윈도우 핸들    Z 순서    윈도우 X    윈도우 y         가로 크기              세로 크기     z 순서 변경 금지 플래그
    ::SetWindowPos(WindowHandle, nullptr, _Pos.iX(), _Pos.iY(), (Rc.right - Rc.left), (Rc.bottom - Rc.top), SWP_NOZORDER);
}

