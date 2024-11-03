#pragma once
#include <io.h>

#include "EnginePath.h"

const int MAXPATH = 256;

class UEngineFile : public UEnginePath
{
public:
	// constrcuter destructer
	UEngineFile();
	UEngineFile(std::string_view _Path);
	UEngineFile(std::filesystem::path _Path);
	~UEngineFile();

	void Write(const void* _Ptr, size_t _Size);
	void Read(void* _Ptr, size_t _Size);

	bool IsExits();
	void FileOpen(const char* _Mode);
	void Close();

protected:

private:
	char Path[MAXPATH] = "";
	FILE* File = nullptr;
};

