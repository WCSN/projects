/*
 * Title       : Directory.cpp
 * Date        : 16.07.2010
 * Author      : Andrei Borodaenko
 * Description : ***
 */

#include <mt/file/Directory.h>
#include <mt/file/FileError.h>

namespace mt {

Directory::Directory(const std::string& path) : m_dirPtr(opendir(path.c_str())),
                                                m_dirPath(path) {
    if (!m_dirPtr) {
        FileError::handleLastError(path);
    }
}

const std::vector<std::string>& Directory::getEntries() {
    rewind();
    m_entries.clear();

    while (true) {
        struct dirent* dirEntry = readdir(m_dirPtr);
        if (!dirEntry) { break; }
        std::string entry(dirEntry->d_name);
        if (entry != "." && entry != "..") {
            m_entries.push_back(entry);
        }
    }

    return m_entries;
}

} // namespace mt
