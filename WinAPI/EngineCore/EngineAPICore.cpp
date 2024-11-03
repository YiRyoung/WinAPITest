#include "PreCompile.h"
#include "EngineAPICore.h"

#include <EngineBase/EngineDelegate.h>

#include <EnginePlatform/EngineInput.h>

// static �ʱ�ȭ
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
	// �޸� �� üũ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UserCore = _UserCore;	// ������Ʈ �ھ� ����

	UEngineWindow::EngineWindowInit(_Inst);			// ���� ������ �ʱ�ȭ
	UEngineAPICore Core = UEngineAPICore();			// ���ο� ���� �ھ� ����
	Core.EngineMainWindow.Open();					// ���� ������ ����
	MainCore = &Core;								// ���� �ھ ���ο� ���� �ھ� ����

	// ���� ���۰� ƽ ��������Ʈ ����
	UEngineDelegate Start = UEngineDelegate(std::bind_front(EngineBeginPlay));
	UEngineDelegate FrameLoop = UEngineDelegate(std::bind_front(EngineTick));

	return UEngineWindow::WindowMessageLoop(Start, FrameLoop);	// �޼��� ���� ����
}

// ������Ʈ �ھ��� Beginplay ȣ��
// ���� �ھ��� EngineBeginPlay�� EngineStart���� �����ϹǷ� ��� X
void UEngineAPICore::EngineBeginPlay()
{
	UserCore->BeginPlay();
}

// ������ ����ǰ� �������� ��� ���ư� EngineTick
void UEngineAPICore::EngineTick()
{
	UserCore->Tick();	// ������Ʈ �ھ��� Tick ȣ��
	MainCore->Tick();	// ���� �ھ��� Tick ȣ��
}

void UEngineAPICore::Tick()
{
	// ���� ������ �������ִٸ�
	if (nullptr != NextLevel)
	{
		// ���� ������ ���ŵǾ��ٸ� ��ȯ ����
		if (nullptr != CurLevel)
		{
			CurLevel->LevelChangeEnd();
		}

		CurLevel = NextLevel;			// ���� ������ ���� ������ ����
		NextLevel->LevelChangeStart();	// ���� ������ ��ȯ ���� ó��
		NextLevel = nullptr;			// ���� ���� �ʱ�ȭ
		DeltaTimer.TimeStart();			// ��Ÿ Ÿ���� ������ �� �����Ƿ� �ʱ�ȭ
	}

	DeltaTimer.TimeCheck();							// ������ ����Ǹ� �ٲ� DeltaTime ����
	float DeltaTime = DeltaTimer.GetDeltaTime();	// ������Ʈ �� DeltaTime ����

	// Ű �Է� Ȯ���� DeltaTime�� ���ŵ� �� ����
	// -> �Ų����� �ڿ������� ������ ���ؼ� 
	UEngineInput::GetInst().KeyCheck(DeltaTime);

	// ���� ������ �����Ǿ� ���� �ʴٸ� ����
	if (nullptr == CurLevel)
	{
		MSGASSERT("���� �ھ ���� ������ �������� �ʾҽ��ϴ�");
		return;
	}

	UEngineInput::GetInst().EventCheck(DeltaTime);	// �̺�Ʈ üũ
	CurLevel->Tick(DeltaTime);						// ���� ���� ������Ʈ
	CurLevel->Render(DeltaTime);					// ���� �������� ȭ�鿡 �׸���
}

// �ݺ��ڿ� Find�� �̿��ؼ� �������� ������ �ʿ��� ���� ������ �ش� ������ ������
void UEngineAPICore::OpenLevel(std::string_view _LevelName)
{
	std::string ChangeName = _LevelName.data();

	std::map<std::string, class ULevel*>::iterator FindIter = Levels.find(ChangeName);
	std::map<std::string, class ULevel*>::iterator EndIter = Levels.end();

	if (EndIter == FindIter)
	{
		MSGASSERT(ChangeName + "��� �̸��� ������ �������� �ʽ��ϴ�.");
		return;
	}

	NextLevel = FindIter->second;
}
