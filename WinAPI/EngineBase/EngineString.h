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

	// string_view는 문자열 읽기 전용으로 C++ 17 버전부터 사용 가능 
	// 객체를 생성하지 않기 때문에 메모리 비용을 줄이는 데 유용함. 
	
	// 대문자로 변환
	static std::string ToUpper(std::string_view _string);	

	// 멀티바이트에서 와이드 바이트로 변환
	static std::wstring AnsiToUnicode(std::string_view _Name);	

protected:

private:
	UEngineString();

};

