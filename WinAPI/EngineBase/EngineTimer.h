#pragma once

class UEngineTimer
{
public:
	// constrcuter destructer
	UEngineTimer();
	~UEngineTimer();

	// delete Function
	UEngineTimer(const UEngineTimer& _Other) = delete;
	UEngineTimer(UEngineTimer&& _Other) noexcept = delete;
	UEngineTimer& operator=(const UEngineTimer& _Other) = delete;
	UEngineTimer& operator=(UEngineTimer&& _Other) noexcept = delete;

	float GetDeltaTime() const
	{
		return fDeltaTime;
	}

	double GetDoubleDeltaTime() const
	{
		return DeltaTime;
	}

	void TimeCheck();

	void TimeStart();

	float End();

	double DEnd();

protected:

private:
	// Window���� �ð��� �����ϰ� ������ �� �ִ� �ڷ����� ���� -> __int64��� ���� �ȴ�
	LARGE_INTEGER Count = LARGE_INTEGER();		// 1�ʿ� ��ŭ �� �� �ִ���
	LARGE_INTEGER PrevTime = LARGE_INTEGER();	// ���� �ð�
	LARGE_INTEGER CurTime = LARGE_INTEGER();	// ���� �ð�

	double TimeCounter = 0.0;
	double DeltaTime = 0.0;
	float fDeltaTime = 0.0f;
};

