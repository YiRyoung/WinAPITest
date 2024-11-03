#pragma once

namespace UEngineDebug
{
	void SetIsDebug(bool _IsDebug);	// Debug 모드 설정

	void SwitchIsDebug();			// Debug 모드 토글 (편의)

	void CoreOutPutString(std::string_view _Text);					// 디버그 문자열 출력 (위치 고정)

	void CoreOutPutString(std::string_view _Text, FVector2D _Pos);	// 디버그 문자열 출력 (출력 위치 입력)

	void PrintEngineDebugText();									// 모든 디버그 문자열 화면 출력
}
