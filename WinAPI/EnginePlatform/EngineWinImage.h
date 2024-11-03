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

	// 이미 만들어진 DC를 통해 이미지를 생성
	void Create(HDC _DC)
	{
		ImageDC = _DC;
	}

	// 타겟 이미지와 스케일 값을 기반으로 이미지를 새로 생성
	void Create(UEngineWinImage* _TargetImage, FVector2D _Scale);

	// 크기 조절 없이 이미지를 타겟 이미지에 복사하는 함수
	void CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans);

	// 투명도를 적용하여 이미지 일부를 타겟에 복사
	// _RenderTrans : 렌더링 위치와 크기 정보
	// _LTImageTrans : 이미지 내에서 일부 영역을 선택하여 복사할 위치 정보
	// _Color의 초기값은 마젠타 색으로 설정됨.
	void CopyToTrans(UEngineWinImage* _TargetImage, const FTransform& _RenderTrans, const FTransform& _LTImageTrans, 
		UColor _Color = UColor(255, 0, 255, 0));

	// 지정된 경로의 이미지 파일을 로드 (PNG/BMP 지원)
	void Load(UEngineWinImage* _TargetImage, std::string_view _Path);

	// 이미지의 크기 반환 함수
	FVector2D GetImageScale() const
	{
		return { Info.bmWidth, Info.bmHeight };
	}

protected:

private:
	HDC ImageDC = nullptr;			// 윈도우 그리기 도구 모음 핸들
	HBITMAP hBitMap = nullptr;		// 비트맵 핸들

	// 비트맵에 대한 속성 정보를 알고 있으면 이미지 처리 시 더 유용하기 때문에 BITMAP 구조체 또한 변수 선언
	BITMAP Info;					// 이미지의 속성 정보(크기, 색상, 픽셀 데이터)
};

