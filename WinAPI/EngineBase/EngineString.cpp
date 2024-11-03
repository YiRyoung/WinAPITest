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

    // Result�� ũ�⸦ _stirng�� ũ��� ������
    Result.resize(_string.size());

    for (size_t i = 0; i < _string.size(); i++)
    {
        // �� ���ھ� �빮�ڷ� ��ȯ
        Result[i] = std::toupper(_string[i]);
    }

    return Result;
}

// Windows���� ����
// ��Ƽ����Ʈ�� ���̵� ����Ʈ�� �ٲ��ִ� �Լ�
std::wstring UEngineString::AnsiToUnicode(std::string_view _Name)
{
    // ��ȯ�� �ʿ��� ���̵� ����Ʈ ũ�� ���ϱ�
    //                             ���ڵ�  �ɼ�  ���� �ּ�      ���ڿ��� ���� (����Ʈ ���� �ν�)  ������ ���� ���̵� ���� ����
    int Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), nullptr, 0);
    // CP_ACP : �ý����� �⺻ ANSI �ڵ� �������� ���

    if (0 == Size)
    {
        MSGASSERT("MultiByteToWideChar ���ڿ� ��ȯ�� �����߽��ϴ�" + std::string(_Name));
        return L"";
    }

    // ��ȯ ����� ������ ���̵� ����Ʈ ���� ���� �� ũ�� �Ҵ�
    std::wstring Result;
    Result.resize(Size);

    // ��ȯ�Ϸ��� ���ڿ��� ���̵� ����Ʈ�� ������ ����ֱ�
    Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), &Result[0], Size);

    if (0 == Size)
    {
        MSGASSERT("MultiByteToWideChar ���ڿ� ��ȯ�� �����߽��ϴ�" + std::string(_Name));
        return L"";
    }

    return Result;
}
