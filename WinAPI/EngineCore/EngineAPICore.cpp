#include "PreCompile.h"
#include "EngineAPICore.h"

#include <EngineBase/EngineDelegate.h>

#include <EnginePlatform/EngineInput.h>

// static 초기화
UEngineAPICore* UEngineAPICore::MainCore = nullptr;
UContentsCore* UEngineAPICore::UserCore = nullptr;

UEngineAPICore::UEngineAPICore()
{
}

UEngineAPICore::~UEngineAPICore()
{
	std::map<std::string, class ULevel*>::iterator StartIter = Levels.begin();
	std::map<std::string, class ULevel*>::iterator EndIter = Levels.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	Levels.clear();
}

int UEngineAPICore::EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore)
{
	// 메모리 릭 체크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UserCore = _UserCore;	// 프로젝트 코어 설정

	UEngineWindow::EngineWindowInit(_Inst);			// 엔진 윈도우 초기화
	UEngineAPICore Core = UEngineAPICore();			// 새로운 엔진 코어 생성
	Core.EngineMainWindow.Open();					// 메인 윈도우 열기
	MainCore = &Core;								// 메인 코어에 새로운 엔진 코어 설정

	// 엔진 시작과 틱 델리게이트 설정
	UEngineDelegate Start = UEngineDelegate(std::bind_front(EngineBeginPlay));
	UEngineDelegate FrameLoop = UEngineDelegate(std::bind_front(EngineTick));

	return UEngineWindow::WindowMessageLoop(Start, FrameLoop);	// 메세지 루프 시작
}

// 프로젝트 코어의 Beginplay 호출
// 엔진 코어의 EngineBeginPlay는 EngineStart에서 실행하므로 사용 X
void UEngineAPICore::EngineBeginPlay()
{
	UserCore->BeginPlay();
}

// 엔진이 실행되고 나서부터 계속 돌아갈 EngineTick
void UEngineAPICore::EngineTick()
{
	UserCore->Tick();	// 프로젝트 코어의 Tick 호출
	MainCore->Tick();	// 엔진 코어의 Tick 호출
}

void UEngineAPICore::Tick()
{
	// 다음 레벨이 정해져있다면
	if (nullptr != NextLevel)
	{
		// 현재 레벨이 갱신되었다면 전환 종료
		if (nullptr != CurLevel)
		{
			CurLevel->LevelChangeEnd();
		}

		CurLevel = NextLevel;			// 다음 레벨을 현재 레벨로 갱신
		NextLevel->LevelChangeStart();	// 다음 레벨의 전환 시작 처리
		NextLevel = nullptr;			// 다음 레벨 초기화
		DeltaTimer.TimeStart();			// 델타 타임이 지연될 수 있으므로 초기화
	}

	DeltaTimer.TimeCheck();							// 레벨이 변경되며 바뀐 DeltaTime 재계산
	float DeltaTime = DeltaTimer.GetDeltaTime();	// 업데이트 된 DeltaTime 적용

	// 키 입력 확인은 DeltaTime이 갱신된 후 실행
	// -> 매끄럽고 자연스러운 진행을 위해서 
	UEngineInput::GetInst().KeyCheck(DeltaTime);

	// 현재 레벨이 설정되어 있지 않다면 종료
	if (nullptr == CurLevel)
	{
		MSGASSERT("엔진 코어에 현재 레벨이 지정되지 않았습니다");
		return;
	}

	UEngineInput::GetInst().EventCheck(DeltaTime);	// 이벤트 체크
	CurLevel->Tick(DeltaTime);						// 게임 로직 업데이트
	CurLevel->Render(DeltaTime);					// 현재 프레임을 화면에 그리기
}

// 반복자와 Find를 이용해서 레벨들을 저장한 맵에서 다음 레벨에 해당 레벨을 설정함
void UEngineAPICore::OpenLevel(std::string_view _LevelName)
{
	std::string ChangeName = _LevelName.data();

	std::map<std::string, class ULevel*>::iterator FindIter = Levels.find(ChangeName);
	std::map<std::string, class ULevel*>::iterator EndIter = Levels.end();

	if (EndIter == FindIter)
	{
		MSGASSERT(ChangeName + "라는 이름의 레벨은 존재하지 않습니다.");
		return;
	}

	NextLevel = FindIter->second;
}
