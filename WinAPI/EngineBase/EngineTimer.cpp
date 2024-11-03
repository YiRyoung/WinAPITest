#include "PreCompile.h"
#include "EngineTimer.h"

UEngineTimer::UEngineTimer()
{
	// QueryPerformanceFrequency : 초당 카운트할 수 있는 주기를 반환하여 Count에 저장
	QueryPerformanceFrequency(&Count);

	// QyadPart : 64비트 정수형 값
	TimeCounter = static_cast<double>(Count.QuadPart);

	// QueryPerformanceCounter : 현재 시점의 타이머 값
	QueryPerformanceCounter(&PrevTime);
	QueryPerformanceCounter(&CurTime);
}

UEngineTimer::~UEngineTimer()
{
}

void UEngineTimer::TimeCheck()
{
	// 현재 시점의 타이머 값 갱신
	QueryPerformanceCounter(&CurTime);

	// 현재 시점 - 과거 시점으로 경과한 카운트 수를 구한다.
	double Tick = static_cast<double>(CurTime.QuadPart - PrevTime.QuadPart);
	
	// 경과한 카운트 수를 초당 카운트 수로 나누어 경과한 시간으로 변환한다.
	DeltaTime = Tick / TimeCounter;

	// float 형으로 형 변환 후 PrevTime을 CurTime으로 업데이트
	// -> 다음 경과 시간을 계산하기 위해 갱신
	fDeltaTime = static_cast<float>(DeltaTime);
	PrevTime.QuadPart = CurTime.QuadPart;
}

void UEngineTimer::TimeStart()
{
	// 타이머를 시작하거나 초기화할 때의 시점 값 저장하는 함수
	QueryPerformanceCounter(&PrevTime);
}

float UEngineTimer::End()
{
	TimeCheck();
	return GetDeltaTime();
}

double UEngineTimer::DEnd()
{
	TimeCheck();
	return GetDoubleDeltaTime();
}
