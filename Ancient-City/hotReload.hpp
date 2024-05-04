#pragma once

#include <vector>
#include <windows.h>

#include "utils.h"

static std::vector<std::pair<std::string, HANDLE>> dirHandles;

extern void AddHotReloadDir(const std::string& dirPath)
{
	WCHAR* dirPathUTF16 = new WCHAR[dirPath.size() + 1];
	MultiByteToWideChar(CP_ACP, 0, dirPath.c_str(), -1, dirPathUTF16, dirPath.size() + 1);

	HANDLE handle = FindFirstChangeNotification(dirPathUTF16, FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);

	if (handle == INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
		LOG(std::format("Failed setting up a file handler for {} shader. Windows error code: {}", dirPath, error), Logger::Level::Error);
	}

	dirHandles.emplace_back(dirPath, handle);
	delete[] dirPathUTF16;
}

extern std::vector<std::string> CheckHotReload()
{
	std::vector<std::string> filesToReload;
	for (auto& dirHandle : dirHandles)
	{
		DWORD waitResult = WaitForSingleObject(dirHandle.second, 0);
		if (waitResult == WAIT_OBJECT_0)
		{
			BYTE buffer[4096] = {};
			DWORD bytesReturned;

			if (ReadDirectoryChangesW(dirHandle.second, buffer, sizeof(buffer), TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesReturned, NULL, NULL))
			{
				FILE_NOTIFY_INFORMATION* fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
				while (fileInfo)
				{
					std::wstring fileNameUTF16(fileInfo->FileName, fileInfo->FileNameLength / sizeof(wchar_t));
					char* fileName = new char[fileNameUTF16.size() + 1];
					WideCharToMultiByte(CP_ACP, 0, fileNameUTF16.c_str(), -1, fileName, fileNameUTF16.size() + 1, NULL, NULL);
					filesToReload.push_back(dirHandle.first + fileName);

					if (fileInfo->NextEntryOffset == 0)
						break;

					fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>
						(reinterpret_cast<BYTE*>(fileInfo) + fileInfo->NextEntryOffset);
					delete[] fileName;
				}
			}
			else
			{
				DWORD error = GetLastError();
				LOG(std::format("Failed to read directory changes. Error code: ", error), Logger::Level::Error);
			}

			FindNextChangeNotification(dirHandle.second);
		}
	}
	return filesToReload;
}

extern void CleanHotReloadHandles()
{
	for (auto& dirHandle : dirHandles)
	{
		FindCloseChangeNotification(dirHandle.second);
	}
}
