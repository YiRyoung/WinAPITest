#include "PreCompile.h"
#include "EngineWinImage.h"

#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

// GDI Plus�� (png Ȯ���� ������ �������� WIndow �⺻ ���̺귯��) ���
#include <objidl.h>
#include <gdiplus.h>

#pragma comment(lib, "Msimg32.lib")	// BMP Ȯ��� ���̺귯��
#pragma comment(lib, "Gdiplus.lib")	// PNG�� ���� Window ����Ƽ�� �׷��� Ȯ��� ���̺귯��

UEngineWinImage::UEngineWinImage()
{
}

UEngineWinImage::~UEngineWinImage()
{
	// ���� ��������� �������� �ʾƵ� �����찡 �޸� ���� ����ֱ� �Ѵ�.
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

// Ÿ�� �̹����� ������ ������ ���� �̹��� ����
void UEngineWinImage::Create(UEngineWinImage* _TargetImage, FVector2D _Scale)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("Main windowDC�� �����ʰ� �̹����� �����Ϸ��� �߽��ϴ�");
		return;
	}
	
	// Ÿ�� �̹����� HDC�� ������� ȣȯ ������ �� ��Ʈ�� ���� (1, 1)
	HBITMAP NewBitMap = static_cast<HBITMAP>(CreateCompatibleBitmap(_TargetImage->GetDC(), _Scale.iX(), _Scale.iY()));
	
	// NewBitmap�� ���� DC ����
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	// NewBitmap�� NewImageDc�� ����
	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitMap));
	DeleteObject(OldBitMap);

	// ���� ��Ʈ�ʰ� DC�� ���� ���� ������ ��ü
	hBitMap = NewBitMap;
	ImageDC = NewImageDC;

	// �̹����� ������ Info ����ü�� ����
	GetObject(hBitMap, sizeof(BITMAP), &Info);
}

// Ÿ�ٿ� �̹����� �״�� ����
void UEngineWinImage::CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("������� ����� �������� �ʽ��ϴ�.");
		return;
	}

	// ���� �̹��� DC�� Ÿ�� DC ����
	HDC CopyDC = ImageDC;
	HDC TargetDC = _TargetImage->ImageDC;
	
	FVector2D LeftTop = _Trans.CenterLeftTop();			// �̹��� ������ ����
	FVector2D RightBot = _Trans.CenterRightBottom();	// �̹��� ���� ����

	// BitBlt : WinAPI �Լ��� ��Ʈ���� ��� ������ ����
	// SRCCOPY : ���� �̹����� �״�� �����ϴ� ����
	//	������ ��� DX  ��� DC�� x   ��� DC�� y    ������ ���� ũ��       ������ ���� ũ��   ���� DC  x  y  ���� ���
	BitBlt(TargetDC, LeftTop.iX(), LeftTop.iY(), _Trans.Scale.iX(), _Trans.Scale.iY(), CopyDC, 0, 0, SRCCOPY);
}

// ������ �����Ͽ� �̹��� �Ϻθ� Ÿ�ٿ� ����
void UEngineWinImage::CopyToTrans(UEngineWinImage* _TargetImage, const FTransform& _RenderTrans, const FTransform& _LTImageTrans, UColor _Color)
{
	// ���� �̹��� DC�� Ÿ�� DC ����
	HDC CopyDC = ImageDC;
	HDC TargetDC = _TargetImage->ImageDC;

	// �̹��� ������ ����
	FVector2D LeftTop = _RenderTrans.CenterLeftTop();

	// TransparentBlt : WinAPI �Լ��� �� ���� HDC�� ���� �̹��� ���� �� Ư�� ������ �����ϰ� ó��
	//				�ش� HDC    ������ x      ������ y           ���� �ʺ�                   ���� ����          ���� HDC
	TransparentBlt(TargetDC, LeftTop.iX(), LeftTop.iY(), _RenderTrans.Scale.iX(), _RenderTrans.Scale.iY(), CopyDC,
	//		   ���� x ������				    ���� y ������					���� �ʺ�						 ���� ����
		_LTImageTrans.Location.iX(), _LTImageTrans.Location.iY(), _LTImageTrans.Scale.iX(), _LTImageTrans.Scale.iY(),
	//  ���� ó���� ����
		_Color.Color);
}

// ��η� �̹����� �ε��Ͽ� Ÿ�ٿ� ����
void UEngineWinImage::Load(UEngineWinImage* _TargetImage, std::string_view _Path)
{
	// �ش� ����� ���� Ȯ���� Ȯ��
	UEnginePath Path = _Path;
	std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

	// �ε��� �̹��� ������ ���� ����
	HBITMAP NewBitmap = nullptr;

	// ���� Ȯ���ڰ� png�� ���
	if (".PNG" == UpperExt)
	{
		// �̹��� �ε������ GOI Plis�� �̿�
		// �������� WinAPI �Լ��� ���

		// GDIplus�� �ڵ�
		ULONG_PTR gidplustoken = 0;
		
		// GDI plus ����� ���� Input
		Gdiplus::GdiplusStartupInput StartupInput;
		Gdiplus::GdiplusStartup(&gidplustoken, &StartupInput, nullptr);

		// �̹��� ��θ� �����ڵ� ���ڿ��� ��ȯ
		std::wstring WidePath = UEngineString::AnsiToUnicode(_Path);

		// GDIplus�� �̹��� �ε�
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(WidePath.c_str());

		// �ε��� �̹����� ���纻�� BMP �������� ��ȯ
		Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());

		// Gdiplus::Status : �Լ� ȣ�� ��� ���� ����, ���� �߻� �� ���� �ڵ� ���� ȣ���
		//                                         ����� ���� (a, r, g, b) -> ����Ÿ  ��ȯ�� ��ü�� ������ ����
		Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(255, 255, 0, 255), &NewBitmap);

		// ȣ���� �������� ���
		if (Gdiplus::Status::Ok != stat)
		{
			MSGASSERT("Png �̹��� �ε忡 �����߽��ϴ�." + std::string(_Path));
			return;
		}

		delete pBitMap;
		delete pImage;
	}
	else if (".BMP" == UpperExt)	// ���� Ȯ���ڰ� bmp��� 
	{
		//                          �ν��Ͻ� �ڵ�  ���� ���     �̹��� Ÿ��  �ʺ� ���� ���Ͽ��� ���� �̹��� �ε�
		HANDLE NewHandle = LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		NewBitmap = reinterpret_cast<HBITMAP>(NewHandle);	// NewHandle�� ��Ʈ�� �ڵ�� �ؼ��ϱ� ���� �� ��ȯ
	}

	if (nullptr == NewBitmap)
	{
		MSGASSERT("�̹��� �ε��� �����߽��ϴ�");
		return;
	}

	// ���ο� DC ���� �� �ε��� �̹��� ����
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());
	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitmap));
	DeleteObject(OldBitMap);

	hBitMap = NewBitmap;
	ImageDC = NewImageDC;

	// �̹��� ������ Info�� ���� 
	GetObject(hBitMap, sizeof(BITMAP), &Info);
}
