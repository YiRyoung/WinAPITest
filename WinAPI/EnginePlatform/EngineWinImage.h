#pragma once
#include <EngineBase/Object.h>

class UEngineWinImage : public UObject
{
public:
	// constrcuter destructer
	UEngineWinImage();
	~UEngineWinImage();

	// delete Function
	UEngineWinImage(const UEngineWinImage& _Other) = delete;
	UEngineWinImage(UEngineWinImage&& _Other) noexcept = delete;
	UEngineWinImage& operator=(const UEngineWinImage& _Other) = delete;
	UEngineWinImage& operator=(UEngineWinImage&& _Other) noexcept = delete;

	HDC GetDC()
	{
		return ImageDC;
	}

	// �̹� ������� DC�� ���� �̹����� ����
	void Create(HDC _DC)
	{
		ImageDC = _DC;
	}

	// Ÿ�� �̹����� ������ ���� ������� �̹����� ���� ����
	void Create(UEngineWinImage* _TargetImage, FVector2D _Scale);

	// ũ�� ���� ���� �̹����� Ÿ�� �̹����� �����ϴ� �Լ�
	void CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans);

	// ������ �����Ͽ� �̹��� �Ϻθ� Ÿ�ٿ� ����
	// _RenderTrans : ������ ��ġ�� ũ�� ����
	// _LTImageTrans : �̹��� ������ �Ϻ� ������ �����Ͽ� ������ ��ġ ����
	// _Color�� �ʱⰪ�� ����Ÿ ������ ������.
	void CopyToTrans(UEngineWinImage* _TargetImage, const FTransform& _RenderTrans, const FTransform& _LTImageTrans, 
		UColor _Color = UColor(255, 0, 255, 0));

	// ������ ����� �̹��� ������ �ε� (PNG/BMP ����)
	void Load(UEngineWinImage* _TargetImage, std::string_view _Path);

	// �̹����� ũ�� ��ȯ �Լ�
	FVector2D GetImageScale() const
	{
		return { Info.bmWidth, Info.bmHeight };
	}

protected:

private:
	HDC ImageDC = nullptr;			// ������ �׸��� ���� ���� �ڵ�
	HBITMAP hBitMap = nullptr;		// ��Ʈ�� �ڵ�

	// ��Ʈ�ʿ� ���� �Ӽ� ������ �˰� ������ �̹��� ó�� �� �� �����ϱ� ������ BITMAP ����ü ���� ���� ����
	BITMAP Info;					// �̹����� �Ӽ� ����(ũ��, ����, �ȼ� ������)
};

