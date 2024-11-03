#pragma once
#include <EnginePlatform/EngineWinImage.h>

// ��������Ʈ ��Ʈ���� ũ�⿡ �°� �߸� �̹���
class UEngineSprite : public UObject
{
public:
	// ��������Ʈ ������ ���� Ŭ����
	class USpriteData
	{
	public:
		UEngineWinImage* Image;		// ��������Ʈ �̹��� ������
		FTransform Transform;		// ��������Ʈ ����(��ġ, ȸ��, ũ��)
	};

	// constrcuter destructer
	UEngineSprite();
	~UEngineSprite();

	// delete Function
	UEngineSprite(const UEngineSprite& _Other) = delete;
	UEngineSprite(UEngineSprite&& _Other) noexcept = delete;
	UEngineSprite& operator=(const UEngineSprite& _Other) = delete;
	UEngineSprite& operator=(UEngineSprite&& _Other) noexcept = delete;

	// ��������Ʈ ������ �߰�
	void PushData(UEngineWinImage* _Image, const FTransform& _Trans);

	// �ش� ��������Ʈ ������ ��������
	USpriteData GetSpriteData(int _index = 0)
	{
		if (_index >= Data.size())
		{
			MSGASSERT("��������Ʈ�� �ε����� �����Ͽ� ����Ϸ��� �߽��ϴ�." + GetName());
			return;
		}

		// �ش� �ε����� ��������Ʈ ������ ��ȯ
		return Data[_index];
	}

	// ��� ��������Ʈ ������ �����
	void ClearSpriteData()
	{
		Data.clear();
	}

protected:

private:
	// ��������Ʈ �����͸� ����
	std::vector<USpriteData> Data;
};

