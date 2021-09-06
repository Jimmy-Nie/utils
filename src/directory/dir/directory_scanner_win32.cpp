#include "directory_scanner_win32.h"

namespace jm
{
	DirectoryScanner_Win32::DirectoryScanner_Win32()
		: handle(INVALID_HANDLE_VALUE), first_next(true)
	{
	}

	DirectoryScanner_Win32::~DirectoryScanner_Win32()
	{
		if (handle != INVALID_HANDLE_VALUE) FindClose(handle);
	}

	bool DirectoryScanner_Win32::scan(const std::string &pathname)
	{
		return scan(pathname, "*.*");
	}

	bool DirectoryScanner_Win32::scan(const std::string &pathname, const std::string& pattern)
	{
		if (handle != INVALID_HANDLE_VALUE)
		{
			FindClose(handle);
			handle = INVALID_HANDLE_VALUE;
		}
		/*
			// Find the full path of the directory we are about to search:
			int size_buffer = GetFullPathName(pathname, 0, 0, 0);
			LPWSTR buffer = new WCHAR[size_buffer];
			LPWSTR filepart = 0;
			GetFullPathNameW(pathname, size_buffer, buffer, &filepart);
			full_pathname = path_with_ending_slash(buffer);
			delete[] buffer;
			*/
		directory_path = path_with_ending_slash(pathname);

		// Start our search:
		std::string filename = directory_path + pattern;
		handle = FindFirstFile(filename.c_str(), &fileinfo);
		first_next = true;
		return (handle != INVALID_HANDLE_VALUE);
	}

	std::string DirectoryScanner_Win32::get_directory_path()
	{
		return directory_path;
	}

	int DirectoryScanner_Win32::get_size()
	{
		return fileinfo.nFileSizeLow;
	}

	std::string DirectoryScanner_Win32::get_name()
	{
		if (first_next) return std::string();
		return fileinfo.cFileName;
	}

	std::string DirectoryScanner_Win32::get_pathname()
	{
		if (first_next) return std::string();
		return directory_path + fileinfo.cFileName;
	}

	bool DirectoryScanner_Win32::is_directory()
	{
		if (first_next) return false;
		return (fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
	}

	bool DirectoryScanner_Win32::is_hidden()
	{
		if (first_next) return false;
		return (fileinfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN;
	}

	bool DirectoryScanner_Win32::is_readable()
	{
		if (first_next) return false;

		HANDLE file = CreateFile(
			get_pathname().c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING,
			0);

		if (file != INVALID_HANDLE_VALUE) CloseHandle(file);
		return file != INVALID_HANDLE_VALUE;
	}

	bool DirectoryScanner_Win32::is_writable()
	{
		if (first_next) return false;

		HANDLE file = CreateFile(
			get_pathname().c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING,
			0);

		if (file != INVALID_HANDLE_VALUE) CloseHandle(file);
		return file != INVALID_HANDLE_VALUE;
	}

	bool DirectoryScanner_Win32::next()
	{
		if (handle == INVALID_HANDLE_VALUE) return false;
		if (first_next)
		{
			first_next = false;
			return true;
		}

		return FindNextFile(handle, &fileinfo) == TRUE;
	}

	std::string DirectoryScanner_Win32::path_with_ending_slash(const std::string &path)
	{
		int len = path.length();
		if (len == 0) return ".\\";
		if (path[len - 1] == '/' || path[len - 1] == '\\') return path;
		return path + "\\";
	}
}
