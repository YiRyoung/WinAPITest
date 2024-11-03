#include "PreCompile.h"
#include "Actor.h"

// delete ���� ����� �־�߸� ȣ�� ����
#include "ActorComponent.h"

// ��� ���� �ν��Ͻ��� �ڽ��� ������Ʈ�� �����ϱ� ���� ���� ����Ʈ ����
std::list<UActorComponent*> AActor::ComponentList;

AActor::AActor()
{
}

AActor::~AActor()
{
	// ������Ʈ�� �����ֱ�� ������ �����ֱ�� ����.
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

// ��� ������Ʈ�� BeginPlay�� ȣ��
void AActor::ComponentBeginPlay()
{
	// ������ �ȿ��� �۾��� �����Ͽ� ���� ���� ����
	{
		std::list<UActorComponent*>::iterator StartIter = ComponentList.begin();
		std::list<UActorComponent*>::iterator EndIter = ComponentList.end();

		// ����Ʈ�� �� ������Ʈ�� ���� BeginPlay ȣ��
		for (; StartIter != EndIter; ++StartIter)
		{
			UActorComponent* CurActor = *StartIter;
			CurActor->BeginPlay();
		}

		// ��� ������Ʈ�� BeginPlay ���� �� ����Ʈ �ʱ�ȭ
		ComponentList.clear();
	}
}

