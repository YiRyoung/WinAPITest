#pragma once
#include <filesystem>	// C++ 17

class UEnginePath
{
public:
	// constrcuter destructer
	UEnginePath();
	UEnginePath(std::string_view _Path);		// ���ڿ� ��η� �ʱ�ȭ
	UEnginePath(std::filesystem::path _Path);	// ���� ��� ��ü�� �ʱ�ȭ
	~UEnginePath();

	std::string GetPathToString();				// ���� ��θ� string���� ������
	std::string GetFileName();					// ���� �̸��� ������
	std::string GetDirectoryName();				// ���� �̸��� ������
	std::string GetExtension();					// Ȯ���ڸ� ������

	bool IsExists();							// ���� ���� �Լ�
	bool IsFile();								// �������� Ȯ��
	bool IsDirectory();							// �������� Ȯ��
	bool MoveParentToDirectory(std::string_view _Path);	// ���� ������ �̵�

	void MoveParent();							// ���� ��η� �̵�
	void Append(std::string_view _AppendName);	// ��ο� ���ڿ��� �߰�


protected:
	std::filesystem::path Path;

private:

};

