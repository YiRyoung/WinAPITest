#pragma once
#include <EngineBase/Object.h>

#include "EngineSprite.h"

class AActor : public UObject
{
public:
	typedef AActor Super;

	friend class ULevel;
	// constrcuter destructer
	AActor();
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	// ��ü�� �������� ������ �Լ�
	virtual void BeginPlay() {}
	virtual void Tick(float _DeltaTime) {}

	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}

	// ���� ��ȯ �Լ�
	class ULevel* GetWorld()
	{
		return World;
	}

	// ��ġ ���� �Լ�
	void SetActorLocation(FVector2D _Location)
	{
		Transform.Location = _Location;
	}

	// ��ġ�� ���⸸ŭ ���ϴ� �Լ� (�̵�)
	void AddActorLocation(FVector2D _Direction)
	{
		Transform.Location += _Direction;
	}

	// ��ġ ��ȯ �Լ�
	FVector2D GetActorLocation()
	{
		return Transform.Location;
	}

	// Ʈ������(����) ��ȯ �Լ�
	FTransform GetTransform()
	{
		return Transform;
	}

	// ������Ʈ ���� �Լ�
	// ������Ʈ�� �����ڸ� ���� ���ͷ� ����
	template<typename ComponentType>
	ComponentType* CreateDefaultSubObject()
	{
		// ���ο� ������Ʈ�� �����Ͽ� �θ� ���͸� ���� ���ͷ� ����
		ComponentType* NewComponent = new ComponentType();
		UActorComponent* ComponentPtr = dynamic_cast<UActorComponent*>(NewComponent);
		ComponentPtr->ParentActor = this;

		// ������ ������Ʈ�� ����Ʈ�� �߰�
		Components.push_back(NewComponent);
		ComponentList.push_back(NewComponent);
		return NewComponent;	// 
	}

protected:

private:
	// static
	// ���ο� ���� ���� ���� Ȯ��
	static bool IsNewActorCreate;

	// ��� ������Ʈ���� BeginPlay ȣ�� �Լ�
	static void ComponentBeginPlay();

	// ��� ������Ʈ���� �����ϴ� ����Ʈ
	static std::list<class UActorComponent*> ComponentList;

	class ULevel* World = nullptr;
	FTransform Transform;

	// ���� ���Ϳ� ���� ������Ʈ�� �����ϴ� ����Ʈ
	std::list<class UActorComponent*> Components;
};

