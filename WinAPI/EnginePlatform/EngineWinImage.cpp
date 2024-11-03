#include "PreCompile.h"
#include "EngineWinImage.h"

#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

// GDI Plus용 (png 확장자 파일을 가져오는 WIndow 기본 라이브러리) 헤더
#include <objidl.h>
#include <gdiplus.h>

#pragma comment(lib, "Msimg32.lib")	// BMP 확장용 라이브러리
#pragma comment(lib, "Gdiplus.lib")	// PNG를 통한 Window 네이티브 그래픽 확장용 라이브러리

UEngineWinImage::UEngineWinImage()
{
}

UEngineWinImage::~UEngineWinImage()
{
	// 굳이 명시적으로 구현하지 않아도 윈도우가 메모리 릭을 잡아주긴 한다.
	if (nullptr != hBitMap)
	{
		DeleteObject(hBitMap);
		hBitMap = nullptr;
	}

	if (nullptr != ImageDC)
	{
		DeleteDC(ImageDC);
		ImageDC = nullptr;
	}
}

// 타겟 이미지와 스케일 값으로 새로 이미지 생성
void UEngineWinImage::Create(UEngineWinImage* _TargetImage, FVector2D _Scale)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("Main windowDC를 넣지않고 이미지를 생성하려고 했습니다");
		return;
	}
	
	// 타겟 이미지의 HDC를 기반으로 호환 가능한 빈 비트맵 생성 (1, 1)
	HBITMAP NewBitMap = static_cast<HBITMAP>(CreateCompatibleBitmap(_TargetImage->GetDC(), _Scale.iX(), _Scale.iY()));
	
	// NewBitmap에 대한 DC 생성
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	// NewBitmap과 NewImageDc를 연결
	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitMap));
	DeleteObject(OldBitMap);

	// 기존 비트맵과 DC를 새로 만든 것으로 교체
	hBitMap = NewBitMap;
	ImageDC = NewImageDC;

	// 이미지의 정보를 Info 구조체에 저장
	GetObject(hBitMap, sizeof(BITMAP), &Info);
}

// 타겟에 이미지를 그대로 복사
void UEngineWinImage::CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("복사받을 대상이 존재하지 않습니다.");
		return;
	}

	// 현재 이미지 DC에 타겟 DC 설정
	HDC CopyDC = ImageDC;
	HDC TargetDC = _TargetImage->ImageDC;
	
	FVector2D LeftTop = _Trans.CenterLeftTop();			// 이미지 시작점 설정
	FVector2D RightBot = _Trans.CenterRightBottom();	// 이미지 끝점 설정

	// BitBlt : WinAPI 함수로 비트맵을 블록 단위로 복사
	// SRCCOPY : 원본 이미지를 그대로 복사하는 설정
	//	복사할 대상 DX  대상 DC의 x   대상 DC의 y    복사할 가로 크기       복사할 세로 크기   원본 DC  x  y  복사 방식
	BitBlt(TargetDC, LeftTop.iX(), LeftTop.iY(), _Trans.Scale.iX(), _Trans.Scale.iY(), CopyDC, 0, 0, SRCCOPY);
}

// 투명도를 적용하여 이미지 일부를 타겟에 복사
void UEngineWinImage::CopyToTrans(UEngineWinImage* _TargetImage, const FTransform& _RenderTrans, const FTransform& _LTImageTrans, UColor _Color)
{
	// 현재 이미지 DC에 타겟 DC 설정
	HDC CopyDC = ImageDC;
	HDC TargetDC = _TargetImage->ImageDC;

	// 이미지 시작점 설정
	FVector2D LeftTop = _RenderTrans.CenterLeftTop();

	// TransparentBlt : WinAPI 함수로 두 개의 HDC를 통해 이미지 복사 및 특정 색상을 투명하게 처리
	//				해당 HDC    복사할 x      복사할 y           복사 너비                   복사 높이          원본 HDC
	TransparentBlt(TargetDC, LeftTop.iX(), LeftTop.iY(), _RenderTrans.Scale.iX(), _RenderTrans.Scale.iY(), CopyDC,
	//		   원본 x 시작점				    원본 y 시작점					복사 너비						 복사 높이
		_LTImageTrans.Location.iX(), _LTImageTrans.Location.iY(), _LTImageTrans.Scale.iX(), _LTImageTrans.Scale.iY(),
	//  투명 처리할 색상
		_Color.Color);
}

// 경로로 이미지를 로드하여 타겟에 적용
void UEngineWinImage::Load(UEngineWinImage* _TargetImage, std::string_view _Path)
{
	// 해당 경로의 파일 확장자 확인
	UEnginePath Path = _Path;
	std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

	// 로드한 이미지 저장할 공간 생성
	HBITMAP NewBitmap = nullptr;

	// 파일 확장자가 png일 경우
	if (".PNG" == UpperExt)
	{
		// 이미지 로드까지만 GOI Plis를 이용
		// 랜더링은 WinAPI 함수를 사용

		// GDIplus용 핸들
		ULONG_PTR gidplustoken = 0;
		
		// GDI plus 사용을 위한 Input
		Gdiplus::GdiplusStartupInput StartupInput;
		Gdiplus::GdiplusStartup(&gidplustoken, &StartupInput, nullptr);

		// 이미지 경로를 유니코드 문자열로 변환
		std::wstring WidePath = UEngineString::AnsiToUnicode(_Path);

		// GDIplus로 이미지 로드
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(WidePath.c_str());

		// 로드한 이미지의 복사본을 BMP 형식으로 변환
		Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());

		// Gdiplus::Status : 함수 호출 결과 성공 여부, 오류 발생 시 오류 코드 등이 호출됨
		//                                         투명색 지정 (a, r, g, b) -> 마젠타  변환된 객체의 포인터 저장
		Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(255, 255, 0, 255), &NewBitmap);

		// 호출이 실패했을 경우
		if (Gdiplus::Status::Ok != stat)
		{
			MSGASSERT("Png 이미지 로드에 실패했습니다." + std::string(_Path));
			return;
		}

		delete pBitMap;
		delete pImage;
	}
	else if (".BMP" == UpperExt)	// 파일 확장자가 bmp라면 
	{
		//                          인스턴스 핸들  파일 경로     이미지 타입  너비 높이 파일에서 직접 이미지 로드
		HANDLE NewHandle = LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		NewBitmap = reinterpret_cast<HBITMAP>(NewHandle);	// NewHandle을 비트맵 핸들로 해석하기 위한 형 변환
	}

	if (nullptr == NewBitmap)
	{
		MSGASSERT("이미지 로딩에 실패했습니다");
		return;
	}

	// 새로운 DC 생성 및 로드한 이미지 연결
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());
	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitmap));
	DeleteObject(OldBitMap);

	hBitMap = NewBitmap;
	ImageDC = NewImageDC;

	// 이미지 정보를 Info에 저장 
	GetObject(hBitMap, sizeof(BITMAP), &Info);
}
