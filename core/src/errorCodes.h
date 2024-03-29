#pragma once

#include <cerrno>

#define GRAVEngine_ERROR_OPERATION_DENIED			EPERM
#define GRAVEngine_ERROR_FILE_NOT_FOUND				ENOENT
#define GRAVEngine_ERROR_PROCESS_NOT_FOUND			ESRCH
#define GRAVEngine_ERROR_FUNC_INTERUPTED			EINTR
#define GRAVEngine_ERROR_IO							EIO
#define GRAVEngine_ERROR_NO_DEVICE_OR_ADDRESS		ENXIO
#define GRAVEngine_ERROR_ARGUMENT_TOO_LONG			E2BIG
#define GRAVEngine_ERROR_EXECUTABLE_FORMAT			ENOEXEC
#define GRAVEngine_ERROR_BAD_FILE_NUMBER			EBADF
#define GRAVEngine_ERROR_SPAWN_PROCESS_NOT_FOUND	ECHILD
#define GRAVEngine_ERROR_MAX_NESTING_LEVEL			EAGAIN
#define GRAVEngine_ERROR_NO_MEMORY					ENOMEM
#define GRAVEngine_ERROR_PERMISSION_DENIED			EACCES
#define GRAVEngine_ERROR_BAD_ADDRESS				EFAULT
#define GRAVEngine_ERROR_DEVICE_BUSY				EBUSY
#define GRAVEngine_ERROR_FILE_EXIST					EEXIST
#define GRAVEngine_ERROR_CROSS_DEVICE_LINK			EXDEV
#define GRAVEngine_ERROR_DEVICE_NOT_FOUND			ENODEV
#define GRAVEngine_ERROR_NOT_DIRECTORY				ENOTDIR
#define GRAVEngine_ERROR_IS_DIRECTORY				EISDIR
#define GRAVEngine_ERROR_INVALID_ARGUMENT			EINVAL
#define GRAVEngine_ERROR_MAX_SYSTEM_FILES			ENFILE
#define GRAVEngine_ERROR_MAX_FILES					EMFILE
#define GRAVEngine_ERROR_BAD_IO_CONTROL_OP			ENOTTY
#define GRAVEngine_ERROR_FILE_TOO_LARGE				EFBIG
#define GRAVEngine_ERROR_NO_DEVICE_SPACE			ENOSPC
#define GRAVEngine_ERROR_INVALID_SEEK				ESPIPE
#define GRAVEngine_ERROR_READONLY_FILE_SYSTEM		EROFS
#define GRAVEngine_ERROR_TOO_MANY_LINKS				EMLINK
#define GRAVEngine_ERROR_BROKEN_PIPE				EPIPE
#define GRAVEngine_ERROR_MATH_ARGUMENT				EDOM
#define GRAVEngine_ERROR_RESULT_TOO_LARGE			ERANGE
#define GRAVEngine_ERROR_RESOURCE_DEADLOCK_OCCUR	EDEADLK
#define GRAVEngine_ERROR_FILENAME_TOO_LONG			ENAMETOOLONG
#define GRAVEngine_ERROR_NO_LOCKS_AVAILABLE			ENOLCK
#define GRAVEngine_ERROR_FUNCTION_NOT_SUPPORTED		ENOSYS
#define GRAVEngine_ERROR_DIRECTORY_NOT_EMPTY		ENOTEMPTY
#define GRAVEngine_ERROR_ILLEGAL_BYTE_SEQUENCE		EILSEQ
#define GRAVEngine_ERROR_STRING_TRUNCATED			STRUNCATE