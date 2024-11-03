#include "PreCompile.h"
#include "EngineFile.h"
#include "EngineDebug.h"

UEngineFile::UEngineFile()
{
}

// 생성 체인 : 자식에서 부모 생성자를 명시적으로 호출
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
		MSGASSERT("크기가 0인 데이터를 쓸수는 없습니다.");
		return;
	}

	if (nullptr == _Ptr)
	{
		MSGASSERT("존재하지 않는 메모리를 사용하려고 했습니다.");
		return;
	}

	// w일 경우에 대한 예외처리
	if (nullptr == File)
	{
		MSGASSERT("열지 않은 파일에 내용을 쓰려고 했습니다");
		return;
	}

	fwrite(_Ptr, _Size, 1, File);
}

void UEngineFile::Read(void* _Ptr, size_t _Size)
{
	if (0 == _Size)
	{
		MSGASSERT("크기가 0인 데이터를 읽을수는 없습니다.");
		return;
	}

	if (nullptr == _Ptr)
	{
		MSGASSERT("존재하지 않는 메모리를 읽으려고 했습니다.");
		return;
	}

	if (nullptr == File)
	{
		MSGASSERT("열지 않은 파일에 내용을 쓰려고 했습니다");
		return;
	}

	fread(_Ptr, _Size, 1, File);
}

bool UEngineFile::IsExits()
{
	// Window 제공 함수
	// 00 : 존재만 / 02 : 쓰기 전용 / 04 : 읽기 전용 / 06 : 읽기 및 쓰기
	// 존재하면 0 반환, 존재하지 않을 시 -1 반환
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

