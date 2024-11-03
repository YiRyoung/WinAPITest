#include "PreCompile.h"
#include "EngineFile.h"
#include "EngineDebug.h"

UEngineFile::UEngineFile()
{
}

// ���� ü�� : �ڽĿ��� �θ� �����ڸ� ��������� ȣ��
UEngineFile::UEngineFile(std::string_view _Path) : UEnginePath(_Path)
{
}

UEngineFile::UEngineFile(std::filesystem::path _Path) : UEnginePath(_Path)
{
}

UEngineFile::~UEngineFile()
{
	Close();
}

void UEngineFile::Write(const void* _Ptr, size_t _Size)
{
	if (0 == _Size)
	{
		MSGASSERT("ũ�Ⱑ 0�� �����͸� ������ �����ϴ�.");
		return;
	}

	if (nullptr == _Ptr)
	{
		MSGASSERT("�������� �ʴ� �޸𸮸� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	// w�� ��쿡 ���� ����ó��
	if (nullptr == File)
	{
		MSGASSERT("���� ���� ���Ͽ� ������ ������ �߽��ϴ�");
		return;
	}

	fwrite(_Ptr, _Size, 1, File);
}

void UEngineFile::Read(void* _Ptr, size_t _Size)
{
	if (0 == _Size)
	{
		MSGASSERT("ũ�Ⱑ 0�� �����͸� �������� �����ϴ�.");
		return;
	}

	if (nullptr == _Ptr)
	{
		MSGASSERT("�������� �ʴ� �޸𸮸� �������� �߽��ϴ�.");
		return;
	}

	if (nullptr == File)
	{
		MSGASSERT("���� ���� ���Ͽ� ������ ������ �߽��ϴ�");
		return;
	}

	fread(_Ptr, _Size, 1, File);
}

bool UEngineFile::IsExits()
{
	// Window ���� �Լ�
	// 00 : ���縸 / 02 : ���� ���� / 04 : �б� ���� / 06 : �б� �� ����
	// �����ϸ� 0 ��ȯ, �������� ���� �� -1 ��ȯ
	int Result = _access(Path, 00);

	return 0 == Result;
}

void UEngineFile::FileOpen(const char* _Mode)
{
	fopen_s(&File, Path, _Mode);

	if (nullptr == File)
	{
		MSGASSERT(Path);
	}
}

void UEngineFile::Close()
{
	if (nullptr != File)
	{
		fclose(File);
		File = nullptr;
	}
}

