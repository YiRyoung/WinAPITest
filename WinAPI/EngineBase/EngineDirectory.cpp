#include "PreCompile.h"
#include "EngineDirectory.h"
#include "EngineFile.h"

UEngineDirectory::UEngineDirectory() : UEnginePath()
{
}

UEngineDirectory::UEngineDirectory(std::string_view _Path) : UEnginePath(_Path)
{
}

UEngineDirectory::UEngineDirectory(std::filesystem::path _Path) : UEnginePath(_Path)
{
}

UEngineDirectory::~UEngineDirectory()
{
}

// 폴더 내 모든 파일 목록을 가져오는 함수, 재귀 옵션으로 하위 폴더까지 검색 가능
std::vector<class UEngineFile> UEngineDirectory::GetAllFile(bool _IsRecursive)
{
	// 결과 파일 목록 저장할 벡터
	std::vector<class UEngineFile> Result;

	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	// 반복자가 끝에 반복할 때까지 반복
	while (false == Diriter._At_end())
	{
		// 현재 항목의 경로
		std::filesystem::path FilePath = *Diriter;

		// 파일인지 폴더인지 확인을 위해 해당 경로로 객체 생성
		UEnginePath Path = UEnginePath(FilePath);

		// 해당 경로가 폴더일 경우
		if (true == Path.IsDirectory())
		{
			// 재귀함수 호출이 true일 경우 하위 폴더 내 파일을 결과에 추가
			if (true == _IsRecursive)
			{
				GetAllFileRecursive(FilePath, Result);
			}

			// 다음 항목으로 이동하여 폴더 끝에 도달할 때까지 continue
			++Diriter;
			continue;
		}

		// 파일일 경우 해당 결과를 결과 목록에 추가
		Result.push_back(UEngineFile(FilePath));
		++Diriter;
	}

	// 파일 목록 반환
	return Result;
}

// 폴더 내 모든 하위 폴더 목록을 가져오는 함수
// 폴더만을 검색하여 벡터로 반환
std::vector<class UEngineDirectory> UEngineDirectory::GetAllDirectory()
{
	std::vector<class UEngineDirectory> Result;

	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	while (false == Diriter._At_end())
	{
		std::filesystem::path FilePath = *Diriter;

		UEnginePath Path = UEnginePath(FilePath);

		if (false == Path.IsDirectory())
		{
			++Diriter;
			continue;
		}

		Result.push_back(UEngineDirectory(FilePath));
		++Diriter;
	}

	return Result;
}

// GetAllFile 함수 내부에서 사용되는 함수 (true == _IsRecursive)
// 재귀적으로 하위 디렉토리의 모든 파일을 가져오는 역할을 하는 함수
void UEngineDirectory::GetAllFileRecursive(std::filesystem::path _Path, std::vector<class UEngineFile>& _Result)
{
	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(_Path);

	while (false == Diriter._At_end())
	{
		std::filesystem::path FilePath = *Diriter;

		UEnginePath Path = UEnginePath(FilePath);

		if (true == Path.IsDirectory())
		{
			GetAllFileRecursive(FilePath, _Result);
			++Diriter;
			continue;
		}

		_Result.push_back(UEngineFile(FilePath));
		++Diriter;
	}
}

