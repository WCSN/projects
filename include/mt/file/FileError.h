/*
 * Title       : FileError.h
 * Date        : 16.07.2010
 * Author      : Andrei Borodaenko
 * Description :
 */

#ifndef FILEERROR_H_
#define FILEERROR_H_

#include <string>

namespace mt {

/**
  * \class FileError
  *
  * \brief Processing different file system errors.
  */
class FileError {

public:
    /** \name Functions */
    //@{
    /**
      * Handles last system error. Throws appropriate exception.
      * \exception mt::Exception (or one of derived from it) according last system error.
      */
    static void handleLastError(const std::string& path);
    //@}
};

} // namespace mt

#endif /* FILEERROR_H_ */
