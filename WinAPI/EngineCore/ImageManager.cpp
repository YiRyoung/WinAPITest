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

// �̹��� ���� �ε�
void UImageManager::Load(std::string_view _Path)
{
	// �ش� ����� ���� �̸��� �����Ͽ� �̹��� �ε�
	UEnginePath EnginePath = UEnginePath(_Path);
	std::string FileName = EnginePath.GetFileName();
	Load(FileName, _Path);
}

// �̹��� ���� �ε�
void UImageManager::LoadFolder(std::string_view _Path)
{
	// �ش� ����� ���� �̸��� �����Ͽ� ���� �ε�
	UEnginePath EnginePath = UEnginePath(_Path);
	std::string DirName = EnginePath.GetDirectoryName();
	LoadFolder(DirName, _Path);
}

// Ư�� Ű �̸��� ��θ� ����� �̹��� �ε�
void UImageManager::Load(std::string_view _KeyName, std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);

	if (true == EnginePath.IsDirectory())
	{
		MSGASSERT("���丮�� �ε��� �� �����ϴ�." + std::string(_Path));
		return;
	}

	if (false == EnginePath.IsExists())
	{
		MSGASSERT("��ȿ���� ���� ���� ��� �Դϴ�." + std::string(_Path));
		return;
	}

	// ���� ������ �̹��� ��������(�� �⺻ ���)
	UEngineWinImage* WindowImage = UEngineAPICore::GetCore()->GetMainWindow().GetWindowImage();

	// Ű �̸� �빮�� ��ȯ
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (true == Images.contains(UpperName))
	{
		MSGASSERT("�ε�� �̹����� �� �ε��� �� �����ϴ�." + UpperName);
		return;
	}

	// ���ο� �̹����� �����Ͽ� �̹����� �̸��� ���� �� �̹��� �ʿ� �߰�
	UEngineWinImage* NewImage = new UEngineWinImage();
	NewImage->Load(WindowImage, _Path);
	NewImage->SetName(UpperName);
	Images.insert({ UpperName, NewImage });

	// ���ο� ��������Ʈ�� �����Ͽ� ���߿� �̹����� �ڸ� �� �ʿ��� �⺻ ������ �غ���
	UEngineSprite* NewSprite = new UEngineSprite();

	// ��ȯ ��ü�� �����Ͽ� �ش� ���� �ʱ�ȭ
	FTransform Trans;
	Trans.Location = { 0,0 };
	Trans.Scale = NewImage->GetImageScale();

	// ���ο� ��������Ʈ�� �����Ϳ� �̸��� �����ϰ� �ʿ� �߰���
	NewSprite->PushData(NewImage, Trans);
	NewSprite->SetName(UpperName);
	Sprites.insert({ UpperName , NewSprite });
}

// ���� ���� ��� �̹��� �ε�
void UImageManager::LoadFolder(std::string_view _KeyName, std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);

	if (false == EnginePath.IsExists())
	{
		MSGASSERT("��ȿ���� ���� ���� ��� �Դϴ�." + std::string(_Path));
		return;
	}

	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (true == Sprites.contains(UpperName))
	{
		MSGASSERT("�ε�� �̹����� �� �ε��� �� �����ϴ�." + UpperName);
		return;
	}

	// ��������Ʈ ��ü�� �����Ͽ� �̸� ���� �� �ʿ� �߰�
	UEngineSprite* NewSprite = new UEngineSprite();
	NewSprite->SetName(UpperName);
	Sprites.insert({ UpperName , NewSprite });

	// ���� ������ �̹��� ��������(�� �⺻ ���)
	UEngineWinImage* WindowImage = UEngineAPICore::GetCore()->GetMainWindow().GetWindowImage();

	// ���� ��ü ���� �� ��� �̹��� ���� ��������
	UEngineDirectory Dir = _Path;
	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		// ���� ��θ� �빮�ڷ� ��ȯ
		std::string FilePath = ImageFiles[i].GetPathToString();
		std::string UpperFileName = UEngineString::ToUpper(ImageFiles[i].GetFileName());

		// ��ȯ�� �̸����� �� �̹��� �˻� 
		UEngineWinImage* NewImage = FindImage(UpperFileName);

		// �ش� �̹����� �������� �ʴ´ٸ�
		if (nullptr == NewImage)
		{
			// ���� �����Ͽ� �̸��� ���� �� �ش� �̹����� �ε���
			NewImage = new UEngineWinImage();
			NewImage->SetName(UpperFileName);
			NewImage->Load(WindowImage, FilePath);
		}

		// �̹������� ��Ƴ��� �ʿ� �߰�
		Images.insert({ UpperFileName,  NewImage });

		FTransform Transform;							// ��ȯ ��ü�� ����
		Transform.Location = { 0, 0 };					// ��ġ �ʱ�ȭ
		Transform.Scale = NewImage->GetImageScale();	// �ش� �̹����� ũ�� ����

		NewSprite->PushData(NewImage, Transform);		// ��������Ʈ ������ �߰�
	}
}

// ��������Ʈ �ڸ��� (X, Y)
void UImageManager::CuttingSprite(std::string_view _KeyName, int _X, int _Y)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("�������� ���� ��������Ʈ�� �ڸ����� �߽��ϴ�" + std::string(_KeyName));
		return;
	}

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("�������� ���� �̹����� ������� ��������Ʈ�� �ڸ����� �߽��ϴ�" + std::string(_KeyName));
		return;
	}

	// ��������Ʈ�� �̹����� ������
	UEngineSprite* Sprite = Sprites[UpperName];
	UEngineWinImage* Image = Images[UpperName];

	// ��������Ʈ�� ���� ������ �ʱ�ȭ
	Sprite->ClearSpriteData();

	// �ش� �̹����� �������� �����ͼ� �ڸ� ũ��� ����
	FVector2D Scale = Image->GetImageScale();
	Scale.X /= _X;
	Scale.Y /= _Y;

	// �ڸ��� �Լ� ȣ��
	CuttingSprite(_KeyName, Scale);
}


// ��������Ʈ �ڸ��� (FVector2D)
void UImageManager::CuttingSprite(std::string_view _KeyName, FVector2D _CuttingSize)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("�������� ���� ��������Ʈ�� �ڸ����� �߽��ϴ�" + std::string(_KeyName));
		return;
	}

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("�������� ���� �̹����� ������� ��������Ʈ�� �ڸ����� �߽��ϴ�" + std::string(_KeyName));
		return;
	}

	// ��������Ʈ�� �̹��� ��������
	UEngineSprite* Sprite = Sprites[UpperName];
	UEngineWinImage* Image = Images[UpperName];

	// ��������Ʈ�� ���� ������ �ʱ�ȭ
	Sprite->ClearSpriteData();

	// ��������Ʈ�� �̹����� �̸� ����
	Sprite->SetName(UpperName);
	Image->SetName(UpperName);

	if (0 != (Image->GetImageScale().iX() % _CuttingSize.iX()))
	{
		MSGASSERT("��������Ʈ ���ÿ� x�� �� �������� �ʽ��ϴ�." + std::string(_KeyName));
		return;
	}

	if (0 != (Image->GetImageScale().iY() % _CuttingSize.iY()))
	{
		MSGASSERT("��������Ʈ ���ÿ� y�� �� �������� �ʽ��ϴ�." + std::string(_KeyName));
		return;
	}

	// �ڸ��� �۾��� ���� ���� �ʱ�ȭ
	int SpriteX = Image->GetImageScale().iX() / _CuttingSize.iX();	// X �������� �ڸ� ���� ����
	int SpriteY = Image->GetImageScale().iY() / _CuttingSize.iY();	// y �������� �ڸ� ���� ����

	// �ڸ� �̹����� Ʈ������ ��ü ����
	FTransform CuttingTrans;
	CuttingTrans.Location = FVector2D::ZERO;			// ��ġ �ʱ�ȭ
	CuttingTrans.Scale = _CuttingSize;					// �ڸ� �̹����� ũ��� �缳��

	// ��������Ʈ�� ������ ����
	for (size_t y = 0; y < SpriteY; ++y)				// y�� �ڸ���
	{
		for (size_t x = 0; x < SpriteX; ++x)			// x�� �ڸ���
		{
			// SpriteX, SpriteY�� �������� ��������Ʈ ����
			Sprite->PushData(Image, CuttingTrans);		// ��������Ʈ�� ������ �߰�
			CuttingTrans.Location.X += _CuttingSize.X;	// x �������� ��ġ �̵�
		}

		CuttingTrans.Location.X = 0.0f;					// x ��ġ �ʱ�ȭ
		CuttingTrans.Location.Y += _CuttingSize.Y;		// y �������� ��ġ �̵�
	}
}

// Ű �̸����� ��������Ʈ�� ����� �ε�Ǿ����� Ȯ��
bool UImageManager::IsLoadSprite(std::string_view _KeyName)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);
	return Sprites.contains(UpperName);
}

// �־��� Ű �̸����� �ش� ��������Ʈ�� ã�� �Լ�
UEngineSprite* UImageManager::FindSprite(std::string_view _KeyName)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Sprites.contains(UpperName))
	{
		MSGASSERT("�ε����� ���� ��������Ʈ�� ����Ϸ��� �߽��ϴ�" + std::string(_KeyName));
		return nullptr;
	}

	// ��������Ʈ�� ������ �ʿ��� �ش� ��������Ʈ ��ȯ
	return Sprites[UpperName];
}

// �־��� Ű �̸����� �ش� �̹����� ã�� �Լ�
UEngineWinImage* UImageManager::FindImage(std::string_view _KeyName)
{
	std::string UpperName = UEngineString::ToUpper(_KeyName);

	if (false == Images.contains(UpperName))
	{
		MSGASSERT("�ε����� ���� ��������Ʈ�� ����Ϸ��� �߽��ϴ�" + std::string(_KeyName));
		return nullptr;
	}

	// �̹����� ������ �ʿ��� �ش� �̹��� ��ȯ
	return Images[UpperName];
}
