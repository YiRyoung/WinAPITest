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

	// ���� static���� ���� �̱��� �����
	// Singleton : ���α׷��� ��Ʋ�� �� ���� ��ü���� ������ �ϴ� ��
	// static�� Ư�� ������ ó�� ������ ������ �Ҵ� ���Ŀ��� �ش� ��ü�� ��ȯ�Ѵ�.
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
		int Key = -1;	// �ʱ�ȭ

		// ���� �ʱ�ȭ
		bool IsDown = false;	// Ű�� ó�� ������ ��
		bool IsPress = false;	// Ű�� ���� ���� ��� ������ ���� ��
		bool IsUp = false;		// Ű�� �����ٰ� ������ ��
		bool IsFree = true;		// Ű�� �� ���� �� ������ ���� ��

		float PressTime = 0.0f;	// Ű�� ������ �־��� �ð�

		// �̺�Ʈ �Լ� ����
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

