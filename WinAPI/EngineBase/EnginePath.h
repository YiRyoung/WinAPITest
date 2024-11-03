#pragma once
#include <filesystem>	// C++ 17

class UEnginePath
{
public:
	// constrcuter destructer
	UEnginePath();
	UEnginePath(std::string_view _Path);		// 문자열 경로로 초기화
	UEnginePath(std::filesystem::path _Path);	// 파일 경로 객체로 초기화
	~UEnginePath();

	std::string GetPathToString();				// 파일 경로를 string으로 가져옴
	std::string GetFileName();					// 파일 이름을 가져옴
	std::string GetDirectoryName();				// 폴더 이름을 가져옴
	std::string GetExtension();					// 확장자만 가져옴

	bool IsExists();							// 존재 여부 함수
	bool IsFile();								// 파일인지 확인
	bool IsDirectory();							// 폴더인지 확인
	bool MoveParentToDirectory(std::string_view _Path);	// 상위 폴더로 이동

	void MoveParent();							// 상위 경로로 이동
	void Append(std::string_view _AppendName);	// 경로에 문자열을 추가


protected:
	std::filesystem::path Path;

private:

};

