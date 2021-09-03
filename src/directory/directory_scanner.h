
#pragma once

#include <memory>

namespace jm
{
	/// \addtogroup bvcutils_I_O_Data bvcutils I/O Data
	/// \{

	class DirectoryScanner_Impl;

	/// \brief Directory scanning class.
	///
	///    <p>DirectoryScanner is used to parse through directory trees and return information about files.</p>
	///    -
	///    <p>Example that prints all files and directories found in the root directory:</p>
	///    <pre>
	///    DirectoryScanner scanner;
	///    if (scanner.scan("/", "*"))
	///    {
	///    		while (scanner.next())
	///    		{
	///    			  std::cout << scanner.get_name() << std::endl;
	///    		}
	///    }
	///    </pre> 
	class DirectoryScanner
	{
	public:
		/// \brief Constructs directory scanner for iterating over a directory.
		DirectoryScanner();

		/// \brief Destructor.
		~DirectoryScanner();

		/// \brief Gets the directory being scanned.
		/** \return Directory being scanned. (including the trailing slash)*/
		std::string get_directory_path();

		/// \brief Gets the name of the current file.
		/** \return The name of the current found file.*/
		std::string get_name();

		/// \brief Gets the size of the current file.
		/** \return The size of the current found file.*/
		int get_size();

		/// \brief Gets the pathname of the current file.
		/** \return The name of the current found file, including the directory path.*/
		std::string get_pathname();

		/// \brief Returns true if the current file is a directory.
		/** \return True if filename is a directory.*/
		bool is_directory();

		/// \brief Returns true if the file is hidden.
		/** \return True if filename is hidden.*/
		bool is_hidden();

		/// \brief Returns true if the file is readable by the current user.
		/** \return True if the file is readable.*/
		bool is_readable();

		/// \brief Returns true if the file is writable by the current user.
		/** \return True if the file is writable.*/
		bool is_writable();

		/// \brief Selects the directory to scan through.
		/** <p>Selects the directory to scan through</p>
			\param pathname Path to the directory to scan (without trailing slash)
			\return true if the directory can be accessed.*/
		bool scan(const std::string& pathname);

		/// \brief Selects the directory to scan through.
		/** <p>Selects the directory to scan through and use a matching pattern on the files.</p>
			WIN32: The pattern is normal DOS pattern matching ("*.*", ?)
			Unix: The pattern is normal pattern matching (*, ?)
			\param pathname Path to the directory to scan (without trailing slash)
			\param pattern Pattern to match files against.
			\return true if the directory can be accessed.*/
		bool scan(const std::string& pathname, const std::string& pattern);

		/// \brief Find next file in directory scan.
		/** \return false if no more files was found.*/
		bool next();

	private:
		std::shared_ptr<DirectoryScanner_Impl> impl;
	};

	/// \}
}
