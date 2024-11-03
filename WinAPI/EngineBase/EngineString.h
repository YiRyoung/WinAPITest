#pragma once
#include <string>

class UEngineString
{
public:
	// constrcuter destructer
	~UEngineString();

	// delete Function
	UEngineString(const UEngineString& _Other) = delete;
	UEngineString(UEngineString&& _Other) noexcept = delete;
	UEngineString& operator=(const UEngineString& _Other) = delete;
	UEngineString& operator=(UEngineString&& _Other) noexcept = delete;

	// string_view�� ���ڿ� �б� �������� C++ 17 �������� ��� ���� 
	// ��ü�� �������� �ʱ� ������ �޸� ����� ���̴� �� ������. 
	
	// �빮�ڷ� ��ȯ
	static std::string ToUpper(std::string_view _string);	

	// ��Ƽ����Ʈ���� ���̵� ����Ʈ�� ��ȯ
	static std::wstring AnsiToUnicode(std::string_view _Name);	

protected:

private:
	UEngineString();

};

