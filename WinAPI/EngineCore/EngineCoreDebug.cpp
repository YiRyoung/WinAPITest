#include "PreCompile.h"
#include "EngineCoreDebug.h"

#include <EnginePlatform/EngineWinDebug.h>

#include "EngineAPICore.h"

namespace UEngineDebug
{
	// 디버그 텍스트 정보를 저장하는 구조체
	class DebugTextInfo
	{
	public:
		std::string Text;	// 출력할 문자열
		FVector2D Pos;		// 출력할 위치
	};

	// 디버그 텍스트 정보를 저장하는 벡터
	std::vector<DebugTextInfo> DebugTexts;

	// 텍스트 출력 위치 (기본 위치 : 0, 0)
	FVector2D EngineTextPos = FVector2D::ZERO;

#ifdef _DEBUG
	bool IsDebug = true;	// 디버그 모드가 활성화된 경우
#else
	bool IsDebug = false;	// 디버그 모드가 비활성화된 경우
#endif

	void SetDebug(bool _IsDebug)
	{
		IsDebug = _IsDebug;
	}

	void SwitchIsDebug()
	{
		IsDebug = !IsDebug;
	}

	// 디버그 문자열을 현재 위치에 출력
	void CoreOutPutString(std::string_view _Text)
	{
		DebugTexts.push_back({ _Text.data(), EngineTextPos });
		EngineTextPos.Y += 20;
	}

	// 디버그 문자열을 지정된 위치에 출력
	void CoreOutPutString(std::string_view _Text, FVector2D _Pos)
	{
		DebugTexts.push_back({ _Text.data(), _Pos });
	}

	// 모든 디버그 문자열을 화면에 출력
	void PrintEngineDebugText()
	{
		// 만약 디버그 모드가 아닐 경우 실행되지 않고 종료
		if (false == IsDebug)
		{
			return;
		}

		// 메인 윈도우의 백버퍼를 가져와서
		UEngineWinImage* BackBuffer = UEngineAPICore::GetCore()->GetMainWindow().GetBackBuffer();

		// 저장된 모든 디버그 문자열을 화면에 출력한다.
		for (size_t i = 0; i < DebugTexts.size(); i++)
		{
			DebugTextInfo& Debug = DebugTexts[i];						// 현재 디버그 텍스트 정보
			WinAPIOutPutString(BackBuffer, Debug.Text, Debug.Pos);		// WinAPI 함수로 문자열 출력
		}

		// 출력 후 상태 초기화
		EngineTextPos = FVector2D::ZERO;
		DebugTexts.clear();
	}
}