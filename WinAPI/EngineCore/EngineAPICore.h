#pragma once

// 다른 객체들의 인터페이스 함수를 정의하는 추상 클래스
class UContentsCore
{
public:
	// 게임 시작 시 호출되는 함수
	virtual void BeginPlay() = 0;
	// 매 프레임마다 호출되는 함수
	virtual void Tick() = 0;
};

// 언리얼의 World와 비슷한 개념
// 엔진의 흐름과 레벨 관리를 처리함
class UEngineAPICore
{
public:
	// constrcuter destructer
	UEngineAPICore();
	~UEngineAPICore();

	// delete Function
	UEngineAPICore(const UEngineAPICore& _Other) = delete;
	UEngineAPICore(UEngineAPICore&& _Other) noexcept = delete;
	UEngineAPICore& operator=(const UEngineAPICore& _Other) = delete;
	UEngineAPICore& operator=(UEngineAPICore&& _Other) noexcept = delete;

	// main에서 실행될 시작 함수
	static int EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore);

	// 엔진 코어 반환 함수
	static class UEngineAPICore* GetCore()
	{
		return MainCore;
	}

	// 메인 윈도우 반환 함수
	UEngineWindow& GetMainWindow()
	{
		return EngineMainWindow;
	}

	// 프레임 반환
	float GetDeltaTime()
	{
		return DeltaTimer.GetDeltaTime();
	}

	// 새로운 레벨 생성 및 반환
	template<typename GameModeType, typename MainPawnType>
	ULevel* CreateLevel(std::string_view _LevelName)
	{
		// 빈 레벨을 생성
		ULevel* NewLevel = new ULevel();

		// 지정된 게임 모드와 메인 폰으로 레벨 생성
		// 게임 모드가 레벨의 특성을 설정하는 중요 객체
		NewLevel->CreateGameMode<GameModeType, MainPawnType>();

		// 생성된 레벨을 레벨을 관리하는 맵에 추가
		Levels.insert({ _LevelName.data(), NewLevel });

		return NewLevel;
	}

	// 레벨을 여는 함수
	void OpenLevel(std::string_view _LevelName);

protected:

private:
	// static
	// 엔진 코어 레벨에서 호출되어 게임 전반의 흐름 제어
	static void EngineBeginPlay();
	static void EngineTick();

	static UEngineAPICore* MainCore;	// 엔진 코어
	static UContentsCore* UserCore;		// 프로젝트 코어

	UEngineTimer DeltaTimer = UEngineTimer();			// 델타 타이머
	UEngineWindow EngineMainWindow = UEngineWindow();	// 엔진 메인 윈도우

	// 레벨을 관리하기 위한 맵
	std::map<std::string, class ULevel*> Levels;

	// 현재 및 다음 레벨을 가리키는 포인터
	// 포인터 변경을 통해 레벨 변경 구현
	class ULevel* CurLevel = nullptr;
	class ULevel* NextLevel = nullptr;

	// EngindBeginPlay/EnginTick과 달리 개별 게임 콘텐츠 객체에서 사용되는 인터페이스 메서드
	// 게임 로직을 초기화하고 주기적으로 업데이트하는 데에 쓰임.
	void Tick();
};

