#pragma once
#include <EnginePlatform/EngineWinImage.h>

// 스프라이트 시트에서 크기에 맞게 잘린 이미지
class UEngineSprite : public UObject
{
public:
	// 스프라이트 데이터 저장 클래스
	class USpriteData
	{
	public:
		UEngineWinImage* Image;		// 스프라이트 이미지 포인터
		FTransform Transform;		// 스프라이트 정보(위치, 회전, 크기)
	};

	// constrcuter destructer
	UEngineSprite();
	~UEngineSprite();

	// delete Function
	UEngineSprite(const UEngineSprite& _Other) = delete;
	UEngineSprite(UEngineSprite&& _Other) noexcept = delete;
	UEngineSprite& operator=(const UEngineSprite& _Other) = delete;
	UEngineSprite& operator=(UEngineSprite&& _Other) noexcept = delete;

	// 스프라이트 데이터 추가
	void PushData(UEngineWinImage* _Image, const FTransform& _Trans);

	// 해당 스프라이트 데이터 가져오기
	USpriteData GetSpriteData(int _index = 0)
	{
		if (_index >= Data.size())
		{
			MSGASSERT("스프라이트의 인덱스를 오버하여 사용하려고 했습니다." + GetName());
			return;
		}

		// 해당 인덱스의 스프라이트 데이터 반환
		return Data[_index];
	}

	// 모든 스프라이트 데이터 지우기
	void ClearSpriteData()
	{
		Data.clear();
	}

protected:

private:
	// 스프라이트 데이터를 저장
	std::vector<USpriteData> Data;
};

