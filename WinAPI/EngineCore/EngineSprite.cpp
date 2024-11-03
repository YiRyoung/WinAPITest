#include "PreCompile.h"
#include "EngineSprite.h"

UEngineSprite::UEngineSprite()
{
}

UEngineSprite::~UEngineSprite()
{
}

void UEngineSprite::PushData(UEngineWinImage* _Image, const FTransform& _Trans)
{
	// 스케일이 0인지 확인 후 오류 메세지 출력
	if (true == _Trans.Scale.IsZeroed())
	{
		MSGASSERT("크기가 0인 이미지를 만들 수 없습니다.");
		return;
	}

	// 스프라이트 데이터를 벡터에 추가
	Data.push_back({ _Image, _Trans });
}

