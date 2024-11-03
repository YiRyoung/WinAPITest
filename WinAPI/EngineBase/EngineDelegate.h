#pragma once
#include <functional>	// C++ 11부터 지원

// Functional을 활용한 델리게이트 구현 클래스 
class UEngineDelegate
{
public:
	// constrcuter destructer
	UEngineDelegate();
	UEngineDelegate(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}
	~UEngineDelegate();

	// 실행해야 할 함수 목록이 있는지 여부를 알려주는 함수
	bool IsBind()
	{
		return false == Functions.empty();
	}

	void operator+=(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}

	void operator() ()
	{
		std::list<std::function<void()>>::iterator StartIter = Functions.begin();
		std::list<std::function<void()>>::iterator EndIter = Functions.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			std::function<void()>& Function = *StartIter;
			Function();
		}
	}

	void Clear()
	{
		Functions.clear();
	}

protected:

private:
	std::list<std::function<void()>> Functions;
};

