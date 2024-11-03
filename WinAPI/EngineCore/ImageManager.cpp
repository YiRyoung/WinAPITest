#include "PreCompile.h"
#include "ImageManager.h"

#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>

#include <EngineCore/EngineAPICore.h>

UImageManager::UImageManager()
{
}

UImageManager::~UImageManager()
{
	std::map<std::string, UEngineWinImage*>::iterator StartIter = Images.begin();
	std::map<std::string, UEngineWinImage*>::iterator EndIter = Images.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		UEngineWinImage* CurRes = StartIter->second;
		if (nullptr != CurRes)
		{
			delete CurRes;
			CurRes = nullptr;
		}
	}

	{
		std::map<std::string, UEngineSprite*>::iterator StartIter = Sprites.begin();
		std::map<std::string, UEngineSprite*>::iterator EndIter = Sprites.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			UEngineSprite* CurRes = StartIter->second;
			if (nullptr != CurRes)
			{
				delete CurRes;
				CurRes = nullptr;
			}
		}
	}
}

// 이미지 파일 로드
void UImageManager::Load(std::string_view _Path)
{
	// 해당 경로의 파일 이름을 추출하여 이미지 로드
	UEnginePath EnginePath = UEnginePath(_Path);
	std::string FileName = EnginePath.GetFileName();
	Load(FileName, _Path);
}

// 이미지 폴더 로드
void UImageManager::LoadFolder(std::string_view _Path)
{
	// 해당 경로의 폴더 이름을 추출하여 폴더 로드
	UEnginePath EnginePath = UEnginePath(_Path);
	std::string DirName = EnginePath.GetDirectoryName();
	LoadFolder(DirName, _Path);
}

// 특정 키 이름과 경로를 사용한 이미지 로드
void UImageManager::Load(std::string_view _KeyName, std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);

	if (true == EnginePath.IsDirectory())
	{
		MSGASSERT("디렉토리는 로드할 수 없습니다." + std::string(_Path));
		return;
	}

	if (false == EnginePath.IsExists())
	{
		MSGASSERT("유효하지 않은 파일 경로 입니다." + std::string(_Path));
		return;
	}

	// 메인 윈도우 이미지 가져오기(흰 기본 배경)
	UEngineWinImage* WindowImage = UEngineAPICore::GetCore()->GetMainWindow().GetWindowImage();

	// 키 이름 대문자 변환
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (true == Images.contains(UpperName))
	{
		MSGASSERT("로드된 이미지를 또 로드할 수 없습니다." + UpperName);
		return;
	}

	// 새로운 이미지를 생성하여 이미지와 이름을 설정 후 이미지 맵에 추가
	UEngineWinImage* NewImage = new UEngineWinImage();
	NewImage->Load(WindowImage, _Path);
	NewImage->SetName(UpperName);
	Images.insert({ UpperName, NewImage });

	// 새로운 스프라이트를 생성하여 나중에 이미지를 자를 때 필요한 기본 정보를 준비함
	UEngineSprite* NewSprite = new UEngineSprite();

	// 변환 객체를 생성하여 해당 변수 초기화
	FTransform Trans;
	Trans.Location = { 0,0 };
	Trans.Scale = NewImage->GetImageScale();

	// 새로운 스프라이트의 데이터와 이름을 설정하고 맵에 추가함
	NewSprite->PushData(NewImage, Trans);
	NewSprite->SetName(UpperName);
	Sprites.insert({ UpperName , NewSprite });
}

// 폴더 내의 모든 이미지 로드
void UImageManager::LoadFolder(std::string_view _KeyName, std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);

	if (false == EnginePath.IsExists())
	{
		MSGASSERT("유효하지 않은 파일 경로 입니다." + std::string(_Path));
		return;
	}

	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (true == Sprites.contains(UpperName))
	{
		MSGASSERT("로드된 이미지를 또 로드할 수 없습니다." + UpperName);
		return;
	}

	// 스프라이트 객체를 생성하여 이름 설정 후 맵에 추가
	UEngineSprite* NewSprite = new UEngineSprite();
	NewSprite->SetName(UpperName);
	Sprites.insert({ UpperName , NewSprite });

	// 메인 윈도우 이미지 가져오기(흰 기본 배경)
	UEngineWinImage* WindowImage = UEngineAPICore::GetCore()->GetMainWindow().GetWindowImage();

	// 폴더 객체 생성 후 모든 이미지 파일 가져오기
	UEngineDirectory Dir = _Path;
	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		// 파일 경로를 대문자로 변환
		std::string FilePath = ImageFiles[i].GetPathToString();
		std::string UpperFileName = UEngineString::ToUpper(ImageFiles[i].GetFileName());

		// 변환된 이름으로 된 이미지 검색 
		UEngineWinImage* NewImage = FindImage(UpperFileName);

		// 해당 이미지가 존재하지 않는다면
		if (nullptr == NewImage)
		{
			// 새로 생성하여 이름을 설정 후 해당 이미지를 로드함
			NewImage = new UEngineWinImage();
			NewImage->SetName(UpperFileName);
			NewImage->Load(WindowImage, FilePath);
		}

		// 이미지들을 모아놓은 맵에 추가
		Images.insert({ UpperFileName,  NewImage });

		FTransform Transform;							// 변환 객체를 생성
		Transform.Location = { 0, 0 };					// 위치 초기화
		Transform.Scale = NewImage->GetImageScale();	// 해당 이미지의 크기 저장

		NewSprite->PushData(NewImage, Transform);		// 스프라이트 데이터 추가
	}
}

// 스프라이트 자르기 (X, Y)
void UImageManager::CuttingSprite(std::string_view _KeyName, int _X, int _Y)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 스프라이트를 자르려고 했습니다" + std::string(_KeyName));
		return;
	}

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 이미지를 기반으로 스프라이트를 자르려고 했습니다" + std::string(_KeyName));
		return;
	}

	// 스프라이트와 이미지를 가져옴
	UEngineSprite* Sprite = Sprites[UpperName];
	UEngineWinImage* Image = Images[UpperName];

	// 스프라이트의 기존 데이터 초기화
	Sprite->ClearSpriteData();

	// 해당 이미지의 스케일을 가져와서 자를 크기로 나눔
	FVector2D Scale = Image->GetImageScale();
	Scale.X /= _X;
	Scale.Y /= _Y;

	// 자르기 함수 호출
	CuttingSprite(_KeyName, Scale);
}


// 스프라이트 자르기 (FVector2D)
void UImageManager::CuttingSprite(std::string_view _KeyName, FVector2D _CuttingSize)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 스프라이트를 자르려고 했습니다" + std::string(_KeyName));
		return;
	}

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 이미지를 기반으로 스프라이트를 자르려고 했습니다" + std::string(_KeyName));
		return;
	}

	// 스프라이트와 이미지 가져오기
	UEngineSprite* Sprite = Sprites[UpperName];
	UEngineWinImage* Image = Images[UpperName];

	// 스프라이트의 기존 데이터 초기화
	Sprite->ClearSpriteData();

	// 스프라이트와 이미지의 이름 설정
	Sprite->SetName(UpperName);
	Image->SetName(UpperName);

	if (0 != (Image->GetImageScale().iX() % _CuttingSize.iX()))
	{
		MSGASSERT("스프라이트 컷팅에 x가 딱 떨어지지 않습니다." + std::string(_KeyName));
		return;
	}

	if (0 != (Image->GetImageScale().iY() % _CuttingSize.iY()))
	{
		MSGASSERT("스프라이트 컷팅에 y가 딱 떨어지지 않습니다." + std::string(_KeyName));
		return;
	}

	// 자르기 작업을 위한 변수 초기화
	int SpriteX = Image->GetImageScale().iX() / _CuttingSize.iX();	// X 방향으로 자른 조각 개수
	int SpriteY = Image->GetImageScale().iY() / _CuttingSize.iY();	// y 방향으로 자른 조각 개수

	// 자른 이미지의 트랜스폼 객체 생성
	FTransform CuttingTrans;
	CuttingTrans.Location = FVector2D::ZERO;			// 위치 초기화
	CuttingTrans.Scale = _CuttingSize;					// 자른 이미지의 크기로 재설정

	// 스프라이트의 시작점 조정
	for (size_t y = 0; y < SpriteY; ++y)				// y축 자르기
	{
		for (size_t x = 0; x < SpriteX; ++x)			// x축 자르기
		{
			// SpriteX, SpriteY를 바탕으로 스프라이트 생성
			Sprite->PushData(Image, CuttingTrans);		// 스프라이트에 데이터 추가
			CuttingTrans.Location.X += _CuttingSize.X;	// x 방향으로 위치 이동
		}

		CuttingTrans.Location.X = 0.0f;					// x 위치 초기화
		CuttingTrans.Location.Y += _CuttingSize.Y;		// y 방향으로 위치 이동
	}
}

// 키 이름으로 스프라이트가 제대로 로드되었는지 확인
bool UImageManager::IsLoadSprite(std::string_view _KeyName)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);
	return Sprites.contains(UpperName);
}

// 주어진 키 이름으로 해당 스프라이트를 찾는 함수
UEngineSprite* UImageManager::FindSprite(std::string_view _KeyName)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("로드하지 않은 스프라이트를 사용하려고 했습니다" + std::string(_KeyName));
		return nullptr;
	}

	// 스프라이트를 저장한 맵에서 해당 스프라이트 반환
	return Sprites[UpperName];
}

// 주어진 키 이름으로 해당 이미지를 찾는 함수
UEngineWinImage* UImageManager::FindImage(std::string_view _KeyName)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("로드하지 않은 스프라이트를 사용하려고 했습니다" + std::string(_KeyName));
		return nullptr;
	}

	// 이미지를 저장한 맵에서 해당 이미지 반환
	return Images[UpperName];
}
