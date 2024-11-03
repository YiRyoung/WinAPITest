#pragma once

// �ٸ� ��ü���� �������̽� �Լ��� �����ϴ� �߻� Ŭ����
class UContentsCore
{
public:
	// ���� ���� �� ȣ��Ǵ� �Լ�
	virtual void BeginPlay() = 0;
	// �� �����Ӹ��� ȣ��Ǵ� �Լ�
	virtual void Tick() = 0;
};

// �𸮾��� World�� ����� ����
// ������ �帧�� ���� ������ ó����
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

	// main���� ����� ���� �Լ�
	static int EngineStart(HINSTANCE _Inst, UContentsCore* _UserCore);

	// ���� �ھ� ��ȯ �Լ�
	static class UEngineAPICore* GetCore()
	{
		return MainCore;
	}

	// ���� ������ ��ȯ �Լ�
	UEngineWindow& GetMainWindow()
	{
		return EngineMainWindow;
	}

	// ������ ��ȯ
	float GetDeltaTime()
	{
		return DeltaTimer.GetDeltaTime();
	}

	// ���ο� ���� ���� �� ��ȯ
	template<typename GameModeType, typename MainPawnType>
	ULevel* CreateLevel(std::string_view _LevelName)
	{
		// �� ������ ����
		ULevel* NewLevel = new ULevel();

		// ������ ���� ���� ���� ������ ���� ����
		// ���� ��尡 ������ Ư���� �����ϴ� �߿� ��ü
		NewLevel->CreateGameMode<GameModeType, MainPawnType>();

		// ������ ������ ������ �����ϴ� �ʿ� �߰�
		Levels.insert({ _LevelName.data(), NewLevel });

		return NewLevel;
	}

	// ������ ���� �Լ�
	void OpenLevel(std::string_view _LevelName);

protected:

private:
	// static
	// ���� �ھ� �������� ȣ��Ǿ� ���� ������ �帧 ����
	static void EngineBeginPlay();
	static void EngineTick();

	static UEngineAPICore* MainCore;	// ���� �ھ�
	static UContentsCore* UserCore;		// ������Ʈ �ھ�

	UEngineTimer DeltaTimer = UEngineTimer();			// ��Ÿ Ÿ�̸�
	UEngineWindow EngineMainWindow = UEngineWindow();	// ���� ���� ������

	// ������ �����ϱ� ���� ��
	std::map<std::string, class ULevel*> Levels;

	// ���� �� ���� ������ ����Ű�� ������
	// ������ ������ ���� ���� ���� ����
	class ULevel* CurLevel = nullptr;
	class ULevel* NextLevel = nullptr;

	// EngindBeginPlay/EnginTick�� �޸� ���� ���� ������ ��ü���� ���Ǵ� �������̽� �޼���
	// ���� ������ �ʱ�ȭ�ϰ� �ֱ������� ������Ʈ�ϴ� ���� ����.
	void Tick();
};

