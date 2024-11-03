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
	// Window에서 시간을 정밀하게 저장할 수 있는 자료형을 만들어냄 -> __int64라고 보면 된다
	LARGE_INTEGER Count = LARGE_INTEGER();		// 1초에 얼만큼 셀 수 있는지
	LARGE_INTEGER PrevTime = LARGE_INTEGER();	// 기준 시간
	LARGE_INTEGER CurTime = LARGE_INTEGER();	// 현재 시간

	double TimeCounter = 0.0;
	double DeltaTime = 0.0;
	float fDeltaTime = 0.0f;
};

