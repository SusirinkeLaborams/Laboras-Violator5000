#include "PrecompiledHeader.h"
#include "Utilities.h"

using namespace std;

vector<uint8_t> Utilities::ReadFileToVector(const std::wstring& path)
{	
	auto fileHandle = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	Assert(fileHandle != INVALID_HANDLE_VALUE);

	LARGE_INTEGER fileSize;
	Assert(GetFileSizeEx(fileHandle, &fileSize) != FALSE);

	vector<uint8_t> fileBytes(static_cast<size_t>(fileSize.QuadPart));

	DWORD numberOfBytesRead;
	auto result = ReadFile(fileHandle, &fileBytes[0], static_cast<DWORD>(fileSize.QuadPart), &numberOfBytesRead, nullptr);
	Assert(result != FALSE);
	Assert(numberOfBytesRead == fileSize.QuadPart);

	CloseHandle(fileHandle);
	return fileBytes;
}