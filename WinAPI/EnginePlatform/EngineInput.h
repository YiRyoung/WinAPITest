#pragma once
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineDelegate.h>

enum class KeyEvent
{
	DOWN,
	PRESS,
	FREE,
	UP
};

class UEngineInput
{
public:
	// constrcuter destructer
	~UEngineInput();

	// delete Function
	UEngineInput(const UEngineInput& _Other) = delete;
	UEngineInput(UEngineInput&& _Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput& _Other) = delete;
	UEngineInput& operator=(UEngineInput&& _Other) noexcept = delete;

	// 지역 static으로 값형 싱글톤 만들기
	// Singleton : 프로그램을 통틀어 한 개에 객체만을 가지게 하는 것
	// static의 특성 때문에 처음 데이터 영역에 할당 이후에는 해당 객체를 반환한다.
	static UEngineInput& GetInst()
	{
		static UEngineInput Inst = UEngineInput();
		return Inst;
	}

	bool IsDown(int _KeyIndex);
	bool IsPress(int _KeyIndex);
	bool IsUp(int _KeyIndex);
	bool IsFree(int _KeyIndex);

	float IsPressTime(int _KeyIndex);

	void KeyCheck(float _DeltaTime);
	void EventCheck(float _DletaTime);
	void BindAction(int _KeyIndex, KeyEvent _EventType, std::function<void()> _Function);

protected:

private:
	class UEngineKey
	{
	public:
		int Key = -1;	// 초기화

		// 상태 초기화
		bool IsDown = false;	// 키가 처음 눌렸을 때
		bool IsPress = false;	// 키가 눌린 이후 계속 누르고 있을 때
		bool IsUp = false;		// 키가 눌리다가 떼었을 때
		bool IsFree = true;		// 키를 뗀 이후 안 누르고 있을 때

		float PressTime = 0.0f;	// 키를 누르고 있었던 시간

		// 이벤트 함수 벡터
		std::vector<std::function<void()>> DownEvents;
		std::vector<std::function<void()>> PressEvents;
		std::vector<std::function<void()>> UpEvents;
		std::vector<std::function<void()>> FreeEvents;

		UEngineKey() {}
		UEngineKey(int _Key) : Key(_Key) {}

		void KeyCheck(float _DeltaTime);
		void EventCheck();
	};

	UEngineInput();

	std::map<int, UEngineKey> Keys;
};

