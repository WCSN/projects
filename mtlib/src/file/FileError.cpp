/*
 * Title       : FileError.cpp
 * Date        : 16.07.2010
 * Author      : Andrei Borodaenko
 * Description : ***
 */

#include <mt/file/FileError.h>
#include <mt/common/Exception.h>

#include <errno.h>
#include <string.h>

namespace mt {

void FileError::handleLastError(const std::string& path) {
    switch (errno) {
    case EIO:
        throw IOException(path);
    case EPERM:
        throw FileAccessDeniedException("insufficient permissions", path);
    case EACCES:
        throw FileAccessDeniedException(path);
    case ENOENT:
        throw FileNotFoundException(path);
    case ENOTDIR:
        throw OpenFileException("not a directory", path);
    case EISDIR:
        throw OpenFileException("not a file", path);
    case EROFS:
        throw FileReadOnlyException(path);
    case EEXIST:
        throw FileExistsException(path);
    case ENOSPC:
        throw FileException("no space left on device", path);
    case EDQUOT:
        throw FileException("disk quota exceeded", path);
    case ENOTEMPTY:
        throw FileException("directory not empty", path);
    case ENAMETOOLONG:
        throw PathSyntaxException(path);
    case ENFILE:
    case EMFILE:
        throw FileException("too many open files", path);
    default:
        throw FileException(strerror(errno), path);
    }
}

} // namespace mt
