#pragma once
#include <EnginePlatform/EngineWinImage.h>

#include "EngineSprite.h"

// 이미지 : 스프라이트 시트
// 스프라이트 : 이미지에서 일정한 크기로 자른 리소스
class UImageManager
{
public:
	// constrcuter destructer
	~UImageManager();

	// delete Function
	UImageManager(const UImageManager& _Other) = delete;
	UImageManager(UImageManager&& _Other) noexcept = delete;
	UImageManager& operator=(const UImageManager& _Other) = delete;
	UImageManager& operator=(UImageManager&& _Other) noexcept = delete;

	// 지역 static 싱글톤
	static UImageManager& GetInst()
	{
		static UImageManager Inst;
		return Inst;
	}

	// 편의성 함수
	void Load(std::string_view _Path);			// 파일 경로로 이미지 로드
	void LoadFolder(std::string_view _Path);	// 폴더에서 모든 이미지 로드

	// 키 이름과 경로를 사용하여 이미지 로드
	void Load(std::string_view _KeyName, std::string_view _Path);
	void LoadFolder(std::string_view _KeyName, std::string_view _Path);

	// 스프라이트 자르기 함수
	void CuttingSprite(std::string_view _KeyName, int _X, int _Y);			// 지정 크기로 자르기(int)
	void CuttingSprite(std::string_view _KeyName, FVector2D _CuttingSize);	// 지정 크기로 자르기(FVector2D)

	// 스프라이트 로드 여부 확인
	bool IsLoadSprite(std::string_view _KeyName);
	UEngineSprite* FindSprite(std::string_view _KeyName);	// 해당 스프라이트 찾기
	UEngineWinImage* FindImage(std::string_view _KeyName);	// 해당 이미지 찾기

protected:

private:
	UImageManager();

	// 스프라이트 시트를 관리하는 맵
	std::map<std::string, UEngineWinImage*> Images;

	// 스프라이트(잘린 이미지)들을 관리하는 맵
	std::map<std::string, UEngineSprite*> Sprites;
};

