#include "PreCompile.h"
#include "EngineTimer.h"

UEngineTimer::UEngineTimer()
{
	// QueryPerformanceFrequency : �ʴ� ī��Ʈ�� �� �ִ� �ֱ⸦ ��ȯ�Ͽ� Count�� ����
	QueryPerformanceFrequency(&Count);

	// QyadPart : 64��Ʈ ������ ��
	TimeCounter = static_cast<double>(Count.QuadPart);

	// QueryPerformanceCounter : ���� ������ Ÿ�̸� ��
	QueryPerformanceCounter(&PrevTime);
	QueryPerformanceCounter(&CurTime);
}

UEngineTimer::~UEngineTimer()
{
}

void UEngineTimer::TimeCheck()
{
	// ���� ������ Ÿ�̸� �� ����
	QueryPerformanceCounter(&CurTime);

	// ���� ���� - ���� �������� ����� ī��Ʈ ���� ���Ѵ�.
	double Tick = static_cast<double>(CurTime.QuadPart - PrevTime.QuadPart);
	
	// ����� ī��Ʈ ���� �ʴ� ī��Ʈ ���� ������ ����� �ð����� ��ȯ�Ѵ�.
	DeltaTime = Tick / TimeCounter;

	// float ������ �� ��ȯ �� PrevTime�� CurTime���� ������Ʈ
	// -> ���� ��� �ð��� ����ϱ� ���� ����
	fDeltaTime = static_cast<float>(DeltaTime);
	PrevTime.QuadPart = CurTime.QuadPart;
}

void UEngineTimer::TimeStart()
{
	// Ÿ�̸Ӹ� �����ϰų� �ʱ�ȭ�� ���� ���� �� �����ϴ� �Լ�
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
