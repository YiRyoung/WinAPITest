#pragma once
#include <EngineBase/Object.h>

class UActorComponent : public UObject
{
public:
	friend class AActor;

	typedef UActorComponent Super;

	// constrcuter destructer
	UActorComponent();
	~UActorComponent();

	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	virtual void BeginPlay() {}
	virtual void ComponentTick(float _DeltaTime) {}

	// 해당 컴포넌트가 붙은 부모 액터 가져오는 함수
	class AActor* GetActor()
	{
		return ParentActor;
	}

protected:

private:
	class AActor* ParentActor;
};

