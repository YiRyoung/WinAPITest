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

// ���� �� ��� ���� ����� �������� �Լ�, ��� �ɼ����� ���� �������� �˻� ����
std::vector<class UEngineFile> UEngineDirectory::GetAllFile(bool _IsRecursive)
{
	// ��� ���� ��� ������ ����
	std::vector<class UEngineFile> Result;

	std::filesystem::directory_iterator Diriter = std::filesystem::directory_iterator(Path);

	// �ݺ��ڰ� ���� �ݺ��� ������ �ݺ�
	while (false == Diriter._At_end())
	{
		// ���� �׸��� ���
		std::filesystem::path FilePath = *Diriter;

		// �������� �������� Ȯ���� ���� �ش� ��η� ��ü ����
		UEnginePath Path = UEnginePath(FilePath);

		// �ش� ��ΰ� ������ ���
		if (true == Path.IsDirectory())
		{
			// ����Լ� ȣ���� true�� ��� ���� ���� �� ������ ����� �߰�
			if (true == _IsRecursive)
			{
				GetAllFileRecursive(FilePath, Result);
			}

			// ���� �׸����� �̵��Ͽ� ���� ���� ������ ������ continue
			++Diriter;
			continue;
		}

		// ������ ��� �ش� ����� ��� ��Ͽ� �߰�
		Result.push_back(UEngineFile(FilePath));
		++Diriter;
	}

	// ���� ��� ��ȯ
	return Result;
}

// ���� �� ��� ���� ���� ����� �������� �Լ�
// �������� �˻��Ͽ� ���ͷ� ��ȯ
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

// GetAllFile �Լ� ���ο��� ���Ǵ� �Լ� (true == _IsRecursive)
// ��������� ���� ���丮�� ��� ������ �������� ������ �ϴ� �Լ�
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

