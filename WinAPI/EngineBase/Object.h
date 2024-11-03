#pragma once

class UObject
{
public:
	// constrcuter destructer
	UObject();
	virtual ~UObject();	// 자식들의 소멸자를 호출하기 위한 가상 함수 테이블

	// delete Function
	UObject(const UObject& _Other) = delete;
	UObject(UObject&& _Other) noexcept = delete;
	UObject& operator=(const UObject& _Other) = delete;
	UObject& operator=(UObject&& _Other) noexcept = delete;

	std::string GetName() const
	{
		return Name;
	}

	std::string_view GetNameView() const
	{
		return Name.c_str();
	}

	// 하위 클래스에서 이름 지정 말고 추가 구현이 필요한 경우를 대비
	virtual void SetName(std::string_view _Name)
	{
		Name = _Name.data();
	}

protected:

private:
	std::string Name;
};

