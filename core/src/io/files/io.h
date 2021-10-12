#pragma once

#include "common.h"
#include <string>
#include <filesystem>

namespace GRAVEngine
{
	namespace IO
	{
		// Is a path a directory
		GRAVAPI inline bool isDirectory(const std::filesystem::path& path) { return std::filesystem::is_directory(path); }
		// Is a path a directory
		GRAVAPI inline bool isDirectory(const char* path) { return isDirectory(std::filesystem::path(path)); }
		// Is a path a directory
		GRAVAPI inline bool isDirectory(const std::string& path) { return isDirectory(std::filesystem::path(path)); }

		// Is a path a file
		GRAVAPI inline bool isFile(const std::filesystem::path& path) { return std::filesystem::is_regular_file(path); }
		// Is a path a file
		GRAVAPI inline bool isFile(const char* path) { return isFile(std::filesystem::path(path)); }
		// Is a path a file
		GRAVAPI inline bool isFile(const std::string& path) { return isFile(std::filesystem::path(path)); }

		// Get the current path
		GRAVAPI inline std::filesystem::path currentPath()							{ return std::filesystem::current_path(); }
		// Change the current path
		GRAVAPI inline void changeCurrentPath(const std::filesystem::path& path)	{ std::filesystem::current_path(path); }
		// Change the current path
		GRAVAPI inline void changeCurrentPath(const char* path)						{ changeCurrentPath(std::filesystem::path(path)); }
		// Change the current path
		GRAVAPI inline void changeCurrentPath(const std::string& path)				{ changeCurrentPath(std::filesystem::path(path)); }

		// Does a file/directory exist
		GRAVAPI inline bool exists(const std::filesystem::path& path)		{ return std::filesystem::exists(path); }
		// Does a file/directory exist
		GRAVAPI inline bool exists(const char* path)						{ return IO::exists(std::filesystem::path(path)); }
		// Does a file/directory exist
		GRAVAPI inline bool exists(const std::string& path)					{ return IO::exists(std::filesystem::path(path)); }

		// Create a directory and all of the required directories too
		GRAVAPI inline bool createDirectory(const std::filesystem::path& path)	{ return std::filesystem::create_directories(path); }
		// Create a directory and all of the required directories too
		GRAVAPI inline bool createDirectory(const char* path)					{ return createDirectory(std::filesystem::path(path)); }
		// Create a directory and all of the required directories too
		GRAVAPI inline bool createDirectory(const std::string& path)			{ return createDirectory(std::filesystem::path(path)); }

		// Delete a file
		GRAVAPI inline bool deleteFile(const std::filesystem::path& path)	{ return (isFile(path) == false) ? false : std::filesystem::remove(path); }
		// Delete a file
		GRAVAPI inline bool deleteFile(const char* path)					{ return deleteFile(std::filesystem::path(path)); }
		// Delete a file
		GRAVAPI inline bool deleteFile(const std::string& path)				{ return deleteFile(std::filesystem::path(path)); }

		// Is a file/directory empty
		GRAVAPI inline bool empty(const std::filesystem::path& path)		{ return std::filesystem::is_empty(path); }
		// Is a file/directory empty
		GRAVAPI inline bool empty(const char* path)							{ return empty(std::filesystem::path(path)); }
		// Is a file/directory empty
		GRAVAPI inline bool empty(const std::string& path)					{ return empty(std::filesystem::path(path)); }


		// Get the file extension
		GRAVAPI inline const std::string fileExtension(const std::filesystem::path& path){ return std::string(path.extension().string());	}
		// Get the file extension
		GRAVAPI inline std::string fileExtension(const char* path)					{ return fileExtension(std::filesystem::path(path)); }
		// Get the file extension
		GRAVAPI inline std::string fileExtension(const std::string& path)			{ return fileExtension(std::filesystem::path(path)); }

		// Does the path have the file file extension
		GRAVAPI inline bool hasFileExtension(const std::filesystem::path& path, const std::filesystem::path& extension) { return fileExtension(path) == extension; }
		// Does the path have the file file extension
		GRAVAPI inline bool hasFileExtension(const char* path, const char* extension)									{ return hasFileExtension(std::filesystem::path(path), std::filesystem::path(extension)); }
		// Does the path have the file file extension
		GRAVAPI inline bool hasFileExtension(const std::string& path, const std::string& extension)						{ return hasFileExtension(std::filesystem::path(path), std::filesystem::path(extension)); }
		
		// Does the path have a file extension
		GRAVAPI inline bool hasFileExtension(const std::filesystem::path& path) { return path.has_extension(); }
		// Does the path have a file extension
		GRAVAPI inline bool hasFileExtension(const char* path)					{ return hasFileExtension(std::filesystem::path(path)); }
		// Does the path have a file extension
		GRAVAPI inline bool hasFileExtension(const std::string& path)			{ return hasFileExtension(std::filesystem::path(path)); }


		// Get the path's parent
		GRAVAPI inline bool hasParentPath(const std::filesystem::path& path)	{ return path.has_parent_path(); }
		// Get the path's parent
		GRAVAPI inline bool hasParentPath(const char* path)						{ return hasParentPath(std::filesystem::path(path)); }
		// Get the path's parent
		GRAVAPI inline bool hasParentPath(const std::string& path)				{ return hasParentPath(std::filesystem::path(path)); }

		// Get the path's parent
		GRAVAPI inline std::string parentPath(const std::filesystem::path& path)	{ return path.parent_path().string(); }
		// Get the path's parent
		GRAVAPI inline std::string parentPath(const char* path)						{ return parentPath(std::filesystem::path(path)); }
		// Get the path's parent
		GRAVAPI inline std::string parentPath(const std::string& path)				{ return parentPath(std::filesystem::path(path)); }


		// Get the path's stem
		GRAVAPI inline bool hasPathStem(const std::filesystem::path& path)	{ return path.has_stem(); }
		// Get the path's stem
		GRAVAPI inline bool hasPathStem(const char* path)					{ return hasPathStem(std::filesystem::path(path)); }
		// Get the path's stem
		GRAVAPI inline bool hasPathStem(const std::string& path)			{ return hasPathStem(std::filesystem::path(path)); }

		// Get the path's stem
		GRAVAPI inline std::string pathStem(const std::filesystem::path& path)	{ return path.stem().string(); }
		// Get the path's stem
		GRAVAPI inline std::string pathStem(const char* path)					{ return pathStem(std::filesystem::path(path)); }
		// Get the path's stem
		GRAVAPI inline std::string pathStem(const std::string& path)			{ return pathStem(std::filesystem::path(path)); }

	}
}