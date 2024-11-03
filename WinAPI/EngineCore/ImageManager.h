#pragma once
#include <EnginePlatform/EngineWinImage.h>

#include "EngineSprite.h"

// �̹��� : ��������Ʈ ��Ʈ
// ��������Ʈ : �̹������� ������ ũ��� �ڸ� ���ҽ�
class UImageManager
{
public:
	// constrcuter destructer
	~UImageManager();

	// delete Function
	UImageManager(const UImageManager& _Other) = delete;
	UImageManager(UImageManager&& _Other) noexcept = delete;
	UImageManager& operator=(const UImageManager& _Other) = delete;
	UImageManager& operator=(UImageManager&& _Other) noexcept = delete;

	// ���� static �̱���
	static UImageManager& GetInst()
	{
		static UImageManager Inst;
		return Inst;
	}

	// ���Ǽ� �Լ�
	void Load(std::string_view _Path);			// ���� ��η� �̹��� �ε�
	void LoadFolder(std::string_view _Path);	// �������� ��� �̹��� �ε�

	// Ű �̸��� ��θ� ����Ͽ� �̹��� �ε�
	void Load(std::string_view _KeyName, std::string_view _Path);
	void LoadFolder(std::string_view _KeyName, std::string_view _Path);

	// ��������Ʈ �ڸ��� �Լ�
	void CuttingSprite(std::string_view _KeyName, int _X, int _Y);			// ���� ũ��� �ڸ���(int)
	void CuttingSprite(std::string_view _KeyName, FVector2D _CuttingSize);	// ���� ũ��� �ڸ���(FVector2D)

	// ��������Ʈ �ε� ���� Ȯ��
	bool IsLoadSprite(std::string_view _KeyName);
	UEngineSprite* FindSprite(std::string_view _KeyName);	// �ش� ��������Ʈ ã��
	UEngineWinImage* FindImage(std::string_view _KeyName);	// �ش� �̹��� ã��

protected:

private:
	UImageManager();

	// ��������Ʈ ��Ʈ�� �����ϴ� ��
	std::map<std::string, UEngineWinImage*> Images;

	// ��������Ʈ(�߸� �̹���)���� �����ϴ� ��
	std::map<std::string, UEngineSprite*> Sprites;
};

