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
	// �������� 0���� Ȯ�� �� ���� �޼��� ���
	if (true == _Trans.Scale.IsZeroed())
	{
		MSGASSERT("ũ�Ⱑ 0�� �̹����� ���� �� �����ϴ�.");
		return;
	}

	// ��������Ʈ �����͸� ���Ϳ� �߰�
	Data.push_back({ _Image, _Trans });
}

