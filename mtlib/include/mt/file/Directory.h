/*
 * Title       : Directory.h
 * Date        : 16.07.2010
 * Author      : Andrei Borodaenko
 * Description :
 */

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include <string>
#include <vector>
#include <dirent.h>

namespace mt {

/**
  * \class Directory
  *
  * \brief Incapsulate functions and properties related to UNIX directory.
  */
class Directory {

public:
    /** \name Constructors & Destructor */
    //@{
    /** Create object related to directory having path \a path.
      * \exception mt::Exception if error occures while opening directory.
      */
    Directory(const std::string& path);
    //@}

    /** \name Convenient functions */
    /**
      * \return const-ref to vector containing all entries' names
      *         currenly presented in directory.
      */
    const std::vector<std::string>& getEntries();

protected:
    /** \name Directory operations. */
    //@{
    /** Same-name POSIX function. */
    void rewind();
    //@}

    /** \name Data members. */
    //@{
    DIR* m_dirPtr; /** Pointer to DIR structure. */
    const std::string m_dirPath; /** Path to directory. */
    std::vector<std::string> m_entries; /** All directory entries' names excepts "." and ".." .*/
    //@}
};

/** Definitions of the inline-functions. */
inline void Directory::rewind() {
    rewinddir(m_dirPtr);
}

} // namespace mt

#endif /* DIRECTORY_H_ */
