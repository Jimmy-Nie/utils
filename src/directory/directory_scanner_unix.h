
#pragma once

#include "directory_scanner_impl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdio>
#include <unistd.h>
#include <string>

namespace jm
{

class DirectoryScanner_Unix : public DirectoryScanner_Impl
{
//!Construction:
public:
	DirectoryScanner_Unix();

	~DirectoryScanner_Unix();

//!Attributes:
public:
	bool scan(const std::string& pathname) override;

	bool scan(const std::string& pathname, const std::string& pattern) override;

	/// \brief Returns the path of the directory being scanned.
    std::string get_directory_path() override;

	/// \brief Returns the size of the current found file.
	int get_size() override;

	/// \brief Returns the name of the current found file.
	std::string get_name() override;

	/// \brief Returns the name of the current found file, including the directory path.
	std::string get_pathname() override;

	/// \brief Returns true if filename is a directory.
	bool is_directory() override;

	/// \brief Returns true if filename is hidden.
	bool is_hidden() override;

  	/// \brief Returns true if file is readable by current user.
	virtual bool is_readable() override;

	/// \brief Returns true if file is writable by current user.
	virtual bool is_writable() override;
	// todo: add other attributes of a file.

//!Operations:
public:
	/// \brief Find next file in directory scan. Returns false if no more files was found.
	bool next() override;

/// \name Implementation
/// \{

private:
	DIR *dir_temp;

	dirent *entry;

	struct stat statbuf;

	bool use_pattern;

	/// \brief Path stored without the trailing slash
	std::string file_name;

	std::string path_name;

	std::string file_pattern;
/// \}
};

}
