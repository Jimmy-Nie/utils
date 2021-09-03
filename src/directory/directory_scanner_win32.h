#pragma once

#include "directory_scanner_impl.h"
#include <string>

namespace jm
{
	class DirectoryScanner_Win32 : public DirectoryScanner_Impl
	{
	public:
		/// \brief Construct initial directory scanner.
		DirectoryScanner_Win32();

		virtual ~DirectoryScanner_Win32();

		/// \brief Scan for files in a directory.
		virtual bool scan(const std::string &pathname);

		/// \brief Scan for files matching a pattern.
		virtual bool scan(const std::string &pathname, const std::string &pattern);

		/// \brief Returns the path of the directory being scanned.
		virtual std::string get_directory_path();

		/// \brief Returns the size of the current found file.
		virtual int get_size();

		/// \brief Returns the name of the current found file.
		virtual std::string get_name();

		/// \brief Returns the name of the current found file, including the directory path.
		virtual std::string get_pathname();

		/// \brief Returns true if filename is a directory.
		virtual bool is_directory();

		/// \brief Returns true if filename is hidden.
		virtual bool is_hidden();

		/// \brief Returns true if filename is readable.
		virtual bool is_readable();

		/// \brief Returns true if filename is writable.
		virtual bool is_writable();

		/// \brief Find next file in directory scan. Returns false if no more files was found.
		virtual bool next();

	private:
		std::string path_with_ending_slash(const std::string &path);

		WIN32_FIND_DATA fileinfo;
		HANDLE handle;
		bool first_next;
		std::string directory_path;
	};
}
