#include "PreCompile.h"
#include "EngineString.h"
#include "EngineDebug.h"

UEngineString::UEngineString()
{
}

UEngineString::~UEngineString()
{
}

std::string UEngineString::ToUpper(std::string_view _string)
{
    std::string Result = _string.data();

    // Result의 크기를 _stirng의 크기로 재지정
    Result.resize(_string.size());

    for (size_t i = 0; i < _string.size(); i++)
    {
        // 한 글자씩 대문자로 변환
        Result[i] = std::toupper(_string[i]);
    }

    return Result;
}

// Windows에서 지원
// 멀티바이트를 와이드 바이트로 바꿔주는 함수
std::wstring UEngineString::AnsiToUnicode(std::string_view _Name)
{
    // 변환에 필요한 와이드 바이트 크기 구하기
    //                             인코딩  옵션  시작 주소      문자열의 길이 (바이트 단위 인식)  저장할 버퍼 와이드 문자 개수
    int Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), nullptr, 0);
    // CP_ACP : 시스템의 기본 ANSI 코드 페이지를 사용

    if (0 == Size)
    {
        MSGASSERT("MultiByteToWideChar 문자열 변환에 실패했습니다" + std::string(_Name));
        return L"";
    }

    // 변환 결과를 저장할 와이드 바이트 변수 생성 및 크기 할당
    std::wstring Result;
    Result.resize(Size);

    // 변환하려는 문자열을 와이드 바이트형 변수에 집어넣기
    Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), &Result[0], Size);

    if (0 == Size)
    {
        MSGASSERT("MultiByteToWideChar 문자열 변환에 실패했습니다" + std::string(_Name));
        return L"";
    }

    return Result;
}
