#pragma once

namespace UEngineDebug
{
	void SetIsDebug(bool _IsDebug);	// Debug ��� ����

	void SwitchIsDebug();			// Debug ��� ��� (����)

	void CoreOutPutString(std::string_view _Text);					// ����� ���ڿ� ��� (��ġ ����)

	void CoreOutPutString(std::string_view _Text, FVector2D _Pos);	// ����� ���ڿ� ��� (��� ��ġ �Է�)

	void PrintEngineDebugText();									// ��� ����� ���ڿ� ȭ�� ���
}
