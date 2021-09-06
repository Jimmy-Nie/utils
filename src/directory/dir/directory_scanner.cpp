#include "directory/directory_scanner.h"
#include "directory_scanner_impl.h"

#ifdef WIN32
#include "directory_scanner_win32.h"
#else
#include "directory_scanner_unix.h"
#endif

#ifndef WIN32
#include <string.h>
#endif

namespace jm
{
	DirectoryScanner::DirectoryScanner()
	{
#ifdef WIN32
		impl = std::shared_ptr<DirectoryScanner_Impl>(new DirectoryScanner_Win32());
#else
		impl = std::shared_ptr<DirectoryScanner_Impl>(new DirectoryScanner_Unix());
#endif
	}

	DirectoryScanner::~DirectoryScanner()
	{
	}

	bool DirectoryScanner::scan(const std::string& pathname)
	{
		return impl->scan(pathname);
	}

	bool DirectoryScanner::scan(const std::string& pathname, const std::string& pattern)
	{
		return impl->scan(pathname, pattern);
	}

	std::string DirectoryScanner::get_directory_path()
	{
		return impl->get_directory_path();
	}

	int DirectoryScanner::get_size()
	{
		return impl->get_size();
	}

	std::string DirectoryScanner::get_name()
	{
		return impl->get_name();
	}

	std::string DirectoryScanner::get_pathname()
	{
		return impl->get_pathname();
	}

	bool DirectoryScanner::is_directory()
	{
		return impl->is_directory();
	}

	bool DirectoryScanner::is_hidden()
	{
		return impl->is_hidden();
	}

	bool DirectoryScanner::is_readable()
	{
		return impl->is_readable();
	}

	bool DirectoryScanner::is_writable()
	{
		return impl->is_writable();
	}

	bool DirectoryScanner::next()
	{
		return impl->next();
	}
}
