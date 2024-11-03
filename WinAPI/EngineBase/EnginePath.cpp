#include "PreCompile.h"
#include "EnginePath.h"
#include "EngineDebug.h"

UEnginePath::UEnginePath() : Path(std::filesystem::current_path())
{
}

UEnginePath::UEnginePath(std::string_view _Path) : Path(_Path)
{
}

UEnginePath::UEnginePath(std::filesystem::path _Path) : Path(_Path)
{
}

UEnginePath::~UEnginePath()
{
}

std::string UEnginePath::GetPathToString()
{
	return Path.string();
}

std::string UEnginePath::GetFileName()
{
	if (true == IsDirectory())
	{
		MSGASSERT("파일 경로 일때만 GetFileName을 호출할수 있습니다." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetDirectoryName()
{
	if (false == IsDirectory())
	{
		MSGASSERT("디렉토리 경로 일때만 GetDirectoryName을 호출할수 있습니다." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetExtension()
{
	// filesystem에서 제공하는 확장자 추출 함수
	return Path.extension().string();
}

bool UEnginePath::IsExists()
{
	// 단순히 존재 여부만을 확인함
	return std::filesystem::exists(Path);
}

bool UEnginePath::IsFile()
{
	// 디렉토리가 아니라면 참
	return false == IsDirectory();
}

bool UEnginePath::IsDirectory()
{
	// filesystem에서 제공해주는 폴더 확인용 함수
	return std::filesystem::is_directory(Path);
}

// 상위 폴더로 이동하여 특정 경로 확인
// 한 단계 위의 폴더로만 이동 가능
bool UEnginePath::MoveParentToDirectory(std::string_view _Path)
{
	UEnginePath DummyPath = UEnginePath(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("디렉토리 경로일때만 MoveParentToDirectory 를 호출할수 있습니다");
		return false;
	}

	bool Result = false;
	std::filesystem::path CurPath = DummyPath.Path;
	std::filesystem::path Root = CurPath.root_path();

	// 현재 경로에서 찾으려는 경로가 있는지 확인
	// 존재하지 않으면 상위 경로로 이동 시킴
	while (true)
	{
		CurPath = DummyPath.Path;

		if (CurPath == Root)
		{
			break;
		}

		CurPath.append(_Path);
		if (true == std::filesystem::exists(CurPath))
		{
			Result = true;
			Path = CurPath;
			break;
		}
		DummyPath.MoveParent();
	}

	return Result;
}

// 상위 경로로 이동
// 상위 폴더 이동과 달리 여러 번 사용 시 최상위 루트까지 이동 가능
void UEnginePath::MoveParent()
{
	Path = Path.parent_path();
}

void UEnginePath::Append(std::string_view _AppendName)
{
	Path.append(_AppendName);
}

