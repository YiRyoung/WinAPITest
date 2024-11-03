#include "PreCompile.h"
#include "EngineCoreDebug.h"

#include <EnginePlatform/EngineWinDebug.h>

#include "EngineAPICore.h"

namespace UEngineDebug
{
	// ����� �ؽ�Ʈ ������ �����ϴ� ����ü
	class DebugTextInfo
	{
	public:
		std::string Text;	// ����� ���ڿ�
		FVector2D Pos;		// ����� ��ġ
	};

	// ����� �ؽ�Ʈ ������ �����ϴ� ����
	std::vector<DebugTextInfo> DebugTexts;

	// �ؽ�Ʈ ��� ��ġ (�⺻ ��ġ : 0, 0)
	FVector2D EngineTextPos = FVector2D::ZERO;

#ifdef _DEBUG
	bool IsDebug = true;	// ����� ��尡 Ȱ��ȭ�� ���
#else
	bool IsDebug = false;	// ����� ��尡 ��Ȱ��ȭ�� ���
#endif

	void SetDebug(bool _IsDebug)
	{
		IsDebug = _IsDebug;
	}

	void SwitchIsDebug()
	{
		IsDebug = !IsDebug;
	}

	// ����� ���ڿ��� ���� ��ġ�� ���
	void CoreOutPutString(std::string_view _Text)
	{
		DebugTexts.push_back({ _Text.data(), EngineTextPos });
		EngineTextPos.Y += 20;
	}

	// ����� ���ڿ��� ������ ��ġ�� ���
	void CoreOutPutString(std::string_view _Text, FVector2D _Pos)
	{
		DebugTexts.push_back({ _Text.data(), _Pos });
	}

	// ��� ����� ���ڿ��� ȭ�鿡 ���
	void PrintEngineDebugText()
	{
		// ���� ����� ��尡 �ƴ� ��� ������� �ʰ� ����
		if (false == IsDebug)
		{
			return;
		}

		// ���� �������� ����۸� �����ͼ�
		UEngineWinImage* BackBuffer = UEngineAPICore::GetCore()->GetMainWindow().GetBackBuffer();

		// ����� ��� ����� ���ڿ��� ȭ�鿡 ����Ѵ�.
		for (size_t i = 0; i < DebugTexts.size(); i++)
		{
			DebugTextInfo& Debug = DebugTexts[i];						// ���� ����� �ؽ�Ʈ ����
			WinAPIOutPutString(BackBuffer, Debug.Text, Debug.Pos);		// WinAPI �Լ��� ���ڿ� ���
		}

		// ��� �� ���� �ʱ�ȭ
		EngineTextPos = FVector2D::ZERO;
		DebugTexts.clear();
	}
}