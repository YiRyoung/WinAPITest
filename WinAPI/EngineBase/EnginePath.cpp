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
		MSGASSERT("���� ��� �϶��� GetFileName�� ȣ���Ҽ� �ֽ��ϴ�." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetDirectoryName()
{
	if (false == IsDirectory())
	{
		MSGASSERT("���丮 ��� �϶��� GetDirectoryName�� ȣ���Ҽ� �ֽ��ϴ�." + Path.string());
		return "";
	}

	return Path.filename().string();
}

std::string UEnginePath::GetExtension()
{
	// filesystem���� �����ϴ� Ȯ���� ���� �Լ�
	return Path.extension().string();
}

bool UEnginePath::IsExists()
{
	// �ܼ��� ���� ���θ��� Ȯ����
	return std::filesystem::exists(Path);
}

bool UEnginePath::IsFile()
{
	// ���丮�� �ƴ϶�� ��
	return false == IsDirectory();
}

bool UEnginePath::IsDirectory()
{
	// filesystem���� �������ִ� ���� Ȯ�ο� �Լ�
	return std::filesystem::is_directory(Path);
}

// ���� ������ �̵��Ͽ� Ư�� ��� Ȯ��
// �� �ܰ� ���� �����θ� �̵� ����
bool UEnginePath::MoveParentToDirectory(std::string_view _Path)
{
	UEnginePath DummyPath = UEnginePath(Path);

	if (false == DummyPath.IsDirectory())
	{
		MSGASSERT("���丮 ����϶��� MoveParentToDirectory �� ȣ���Ҽ� �ֽ��ϴ�");
		return false;
	}

	bool Result = false;
	std::filesystem::path CurPath = DummyPath.Path;
	std::filesystem::path Root = CurPath.root_path();

	// ���� ��ο��� ã������ ��ΰ� �ִ��� Ȯ��
	// �������� ������ ���� ��η� �̵� ��Ŵ
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

// ���� ��η� �̵�
// ���� ���� �̵��� �޸� ���� �� ��� �� �ֻ��� ��Ʈ���� �̵� ����
void UEnginePath::MoveParent()
{
	Path = Path.parent_path();
}

void UEnginePath::Append(std::string_view _AppendName)
{
	Path.append(_AppendName);
}

