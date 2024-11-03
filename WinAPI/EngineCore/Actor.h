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

	// 객체가 레벨에서 동작할 함수
	virtual void BeginPlay() {}
	virtual void Tick(float _DeltaTime) {}

	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}

	// 월드 반환 함수
	class ULevel* GetWorld()
	{
		return World;
	}

	// 위치 설정 함수
	void SetActorLocation(FVector2D _Location)
	{
		Transform.Location = _Location;
	}

	// 위치를 방향만큼 더하는 함수 (이동)
	void AddActorLocation(FVector2D _Direction)
	{
		Transform.Location += _Direction;
	}

	// 위치 반환 함수
	FVector2D GetActorLocation()
	{
		return Transform.Location;
	}

	// 트랜스폼(정보) 반환 함수
	FTransform GetTransform()
	{
		return Transform;
	}

	// 컴포넌트 생성 함수
	// 컴포넌트의 소유자를 현재 액터로 설정
	template<typename ComponentType>
	ComponentType* CreateDefaultSubObject()
	{
		// 새로운 컴포넌트를 생성하여 부모 액터를 현재 액터로 설정
		ComponentType* NewComponent = new ComponentType();
		UActorComponent* ComponentPtr = dynamic_cast<UActorComponent*>(NewComponent);
		ComponentPtr->ParentActor = this;

		// 생성된 컴포넌트를 리스트에 추가
		Components.push_back(NewComponent);
		ComponentList.push_back(NewComponent);
		return NewComponent;	// 
	}

protected:

private:
	// static
	// 새로운 액터 생성 여부 확인
	static bool IsNewActorCreate;

	// 모든 컴포넌트들의 BeginPlay 호출 함수
	static void ComponentBeginPlay();

	// 모든 컴포넌트들을 관리하는 리스트
	static std::list<class UActorComponent*> ComponentList;

	class ULevel* World = nullptr;
	FTransform Transform;

	// 현재 액터에 속한 컴포넌트를 관리하는 리스트
	std::list<class UActorComponent*> Components;
};

