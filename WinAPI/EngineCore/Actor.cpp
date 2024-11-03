#include "PreCompile.h"
#include "Actor.h"

// delete 또한 헤더가 있어야만 호출 가능
#include "ActorComponent.h"

// 모든 액터 인스턴스가 자신의 컴포넌트를 공유하기 위해 전역 리스트 생성
std::list<UActorComponent*> AActor::ComponentList;

AActor::AActor()
{
}

AActor::~AActor()
{
	// 컴포넌트의 생성주기는 액터의 생명주기와 같다.
	std::list<UActorComponent*>::iterator StartIter = Components.begin();
	std::list<UActorComponent*>::iterator EndIter = Components.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		UActorComponent* Component = *StartIter;

		if (nullptr != Component)
		{
			delete Component;
		}
	}

	Components.clear();
}

// 모든 컴포넌트의 BeginPlay를 호출
void AActor::ComponentBeginPlay()
{
	// 스코프 안에서 작업을 진행하여 변수 범위 제한
	{
		std::list<UActorComponent*>::iterator StartIter = ComponentList.begin();
		std::list<UActorComponent*>::iterator EndIter = ComponentList.end();

		// 리스트의 각 컴포넌트에 대해 BeginPlay 호출
		for (; StartIter != EndIter; ++StartIter)
		{
			UActorComponent* CurActor = *StartIter;
			CurActor->BeginPlay();
		}

		// 모든 컴포넌트의 BeginPlay 실행 후 리스트 초기화
		ComponentList.clear();
	}
}

