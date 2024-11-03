#pragma once

// 두 점(x, y)
class FVector2D
{
public:
	float X = 0.0f;
	float Y = 0.0;

	// 특정 값 고정
	static const FVector2D ZERO;
	static const FVector2D LEFT;
	static const FVector2D RIGHT;
	static const FVector2D UP;
	static const FVector2D DOWN;

	FVector2D() {}
	FVector2D(float _X, float _Y) : X(_X), Y(_Y) {}
	FVector2D(int _X, int _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y)) {}
	FVector2D(long _X, long _Y): X(static_cast<float>(_X)), Y(static_cast<float>(_Y)) {}

	// int로 형변환하는 함수
	int iX() const
	{
		return static_cast<int>(X);
	}

	int iY() const
	{
		return static_cast<int>(Y);
	}

	bool IsZeroed() const
	{
		return X == 0.0f || Y == 0.0f;
	}

	bool operator==(FVector2D _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	bool EqualToInt(FVector2D _Other) const
	{
		return iX() == _Other.iX() && iY() == _Other.iY();
	}

	FVector2D Half() const
	{
		return { X * 0.5f, Y * 0.5f };
	}

	// 사칙연산
	FVector2D operator+(FVector2D _Other) const
	{
		FVector2D Result;
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	FVector2D operator-(FVector2D _Other) const
	{
		FVector2D Result;
		Result.X = X - _Other.X;
		Result.Y = Y - _Other.Y;
		return Result;
	}

	FVector2D operator*(float _Value) const
	{
		FVector2D Result;
		Result.X = X * _Value;
		Result.Y = Y * _Value;
		return Result;
	}

	FVector2D operator/(int _Value) const
	{
		FVector2D Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}

	FVector2D& operator+= (FVector2D _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}

	// 좌표 출력용 함수
	std::string ToString()
	{
		std::string Stream;
		Stream += "X : [";
		Stream += std::to_string(X);
		Stream += "] Y : [";
		Stream += std::to_string(Y);
		Stream += "]";
		return Stream;
	}
};

// 오브젝트의 위치와 크기를 묶은 객체
class FTransform
{
public:
	FVector2D Location;
	FVector2D Scale;

	// 왼쪽 상단
	FVector2D CenterLeftTop() const
	{
		return Location - Scale.Half();
	}

	// 우측 하단
	FVector2D CenterRightBottom() const
	{
		return Location + Scale.Half();
	}
};

// 색을 나타내는 객체 (R, G, B, A)
// RGBA 값을 각각, 혹은 하나의 정수값으로 관리 가능 
class UColor
{
public:
	union			// 모든 멤버가 같은 메모리 위치를 공유함
	{
		int Color;	// RGBA의 색상 값을 하나의 정수 값으로 관리
		struct      // 각각의 메모리 위치에 멤버를 저장
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
			unsigned char A;
		};
	};

	UColor(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
		: R(_R), G(_G), B(_B), A(_A)
	{

	}
};

