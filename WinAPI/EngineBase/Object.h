#pragma once

class UObject
{
public:
	// constrcuter destructer
	UObject();
	virtual ~UObject();	// �ڽĵ��� �Ҹ��ڸ� ȣ���ϱ� ���� ���� �Լ� ���̺�

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

	// ���� Ŭ�������� �̸� ���� ���� �߰� ������ �ʿ��� ��츦 ���
	virtual void SetName(std::string_view _Name)
	{
		Name = _Name.data();
	}

protected:

private:
	std::string Name;
};

