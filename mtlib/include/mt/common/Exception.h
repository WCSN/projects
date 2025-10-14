/*
 * Title       : Exception.h
 * Date        : 12.07.2010
 * Author      : Andrei Borodaenko
 * Description :
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include <typeinfo>
#include <sstream>

namespace mt {

/**
  * \class Exception
  *
  * \brief Base class for all mtlib exceptions
  */
class Exception : public std::exception {

public:
    /** \name Constructors & Destructor */
    //@{
    /** Main constructor.
      * Sets message text to \a msg and code to \a code.
      */
    Exception(const std::string& msg, int code = 0);

    /** Constructor with argument \a arg which contains additional information
      * which will be appended to message text \a msg.
      */
    Exception(const std::string& msg, const std::string& arg, int code = 0);

    /** Copy constructor. */
    Exception(const Exception& exc);

    /** Destructor. */
    virtual ~Exception() throw();
    //@}

    /** \name Operators */
    //@{
    /** Assignment operator. */
    Exception& operator=(const Exception& exc);
    //@}

    /** \name Functions */
    //@{
    /** \return a description of the exception. */
    virtual const char* name() const throw();

    /** \return the name of the exception class. */
    virtual const char* className() const throw();

    /** Same as name(), for compatibility with std::exception. */
    virtual const char* what() const throw();

    /** \return the message text. */
    const std::string& message() const;

    /** \return the exception code if defined. */
    int code() const;

    /** \return a string consisting of the exception name and the message text. */
    std::string displayText() const;

    /** Rethrows the exception. */
    virtual void rethrow() const;
    //@}

protected:
    /** General constructor.
      *  Creates exception with code \a code.
      */
    Exception(int code = 0);

    /** Sets the message for the exception to \a msg. */
    void message(const std::string& msg);

    /** Sets the extended message for the exception e.g.
      *  appends \a arg to message text.
      */
    void extendedMessage(const std::string& arg);

private:
    std::string m_msg; /** message text */
    int m_code; /** code */
};

/** Definitions of the inline-functions. */
inline const char* Exception::name() const throw() {
    return "Exception";
}


inline const char* Exception::className() const throw() {
    return typeid(*this).name();
}


inline const char* Exception::what() const throw() {
    return name();
}

inline const std::string& Exception::message() const {
    return m_msg;
}

inline int Exception::code() const {
    return m_code;
}

inline void Exception::rethrow() const {
    throw *this;
}

inline void Exception::message(const std::string& msg) {
    m_msg = msg;
}

/** Macros which throws exception with given class and message text. */
#define MT_EXCEPTION(CLS, ARGS)   \
        {                         \
            std::stringstream ss; \
            ss << ARGS;           \
            throw CLS(ss.str());  \
        }

/// Macros witch throws exception if CONDITION is FALSE.
#define MT_EXCEPTION_ASSERT(CONDITION, ARGS) \
	{\
		if(!(CONDITION))														\
			MT_EXCEPTION(mt::Exception, ARGS)\
	}

/** Exception declaration macros */
#define MT_DECLARE_EXCEPTION(CLS, BASE) \
    class CLS : public BASE {                                              \
    public:                                                                \
        CLS(int code = 0);                                                 \
        CLS(const std::string& msg, int code = 0);                         \
        CLS(const std::string& msg, const std::string& arg, int code = 0); \
        CLS(const CLS& exc);                                               \
        virtual ~CLS() throw();                                            \
        CLS& operator=(const CLS& exc);                                    \
        const char* name() const throw();                                  \
        const char* className() const throw();                             \
        void rethrow() const;                                              \
    };

/** Exception implementation macros */
#define MT_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)                                                   \
    CLS::CLS(int code) : BASE(code) { }                                                           \
    CLS::CLS(const std::string& msg, int code) : BASE(msg, code) { }                              \
    CLS::CLS(const std::string& msg, const std::string& arg, int code) : BASE(msg, arg, code) { } \
    CLS::CLS(const CLS& exc) : BASE(exc) { }                                                      \
    CLS::~CLS() throw() { }                                                                       \
    CLS& CLS::operator=(const CLS& exc) {                                                         \
        BASE::operator=(exc);                                                                     \
        return *this;                                                                             \
    }                                                                                             \
    const char* CLS::name() const throw() { return NAME; }                                        \
    const char* CLS::className() const throw() { return typeid(*this).name(); }                   \
    void CLS::rethrow() const { throw *this; }


/** Declarations of an exception classes. */
MT_DECLARE_EXCEPTION(LogicException, Exception)
MT_DECLARE_EXCEPTION(AssertionViolationException, LogicException)
MT_DECLARE_EXCEPTION(NullPointerException, LogicException)
MT_DECLARE_EXCEPTION(BugcheckException, LogicException)
MT_DECLARE_EXCEPTION(InvalidArgumentException, LogicException)
MT_DECLARE_EXCEPTION(NotImplementedException, LogicException)
MT_DECLARE_EXCEPTION(RangeException, LogicException)
MT_DECLARE_EXCEPTION(IllegalStateException, LogicException)
MT_DECLARE_EXCEPTION(InvalidAccessException, LogicException)
MT_DECLARE_EXCEPTION(SignalException, LogicException)
MT_DECLARE_EXCEPTION(UnhandledException, LogicException)

MT_DECLARE_EXCEPTION(RuntimeException, Exception)
MT_DECLARE_EXCEPTION(NotFoundException, RuntimeException)
MT_DECLARE_EXCEPTION(ExistsException, RuntimeException)
MT_DECLARE_EXCEPTION(TimeoutException, RuntimeException)
MT_DECLARE_EXCEPTION(SystemException, RuntimeException)
MT_DECLARE_EXCEPTION(RegularExpressionException, RuntimeException)
MT_DECLARE_EXCEPTION(LibraryLoadException, RuntimeException)
MT_DECLARE_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException)
MT_DECLARE_EXCEPTION(NoThreadAvailableException, RuntimeException)
MT_DECLARE_EXCEPTION(PropertyNotSupportedException, RuntimeException)
MT_DECLARE_EXCEPTION(PoolOverflowException, RuntimeException)
MT_DECLARE_EXCEPTION(NoPermissionException, RuntimeException)
MT_DECLARE_EXCEPTION(OutOfMemoryException, RuntimeException)
MT_DECLARE_EXCEPTION(DataException, RuntimeException)

MT_DECLARE_EXCEPTION(DataFormatException, DataException)
MT_DECLARE_EXCEPTION(SyntaxException, DataException)
MT_DECLARE_EXCEPTION(CircularReferenceException, DataException)
MT_DECLARE_EXCEPTION(PathSyntaxException, SyntaxException)
MT_DECLARE_EXCEPTION(IOException, RuntimeException)
MT_DECLARE_EXCEPTION(ProtocolException, IOException)
MT_DECLARE_EXCEPTION(FileException, IOException)
MT_DECLARE_EXCEPTION(FileExistsException, FileException)
MT_DECLARE_EXCEPTION(FileNotFoundException, FileException)
MT_DECLARE_EXCEPTION(PathNotFoundException, FileException)
MT_DECLARE_EXCEPTION(FileReadOnlyException, FileException)
MT_DECLARE_EXCEPTION(FileAccessDeniedException, FileException)
MT_DECLARE_EXCEPTION(CreateFileException, FileException)
MT_DECLARE_EXCEPTION(OpenFileException, FileException)
MT_DECLARE_EXCEPTION(WriteFileException, FileException)
MT_DECLARE_EXCEPTION(ReadFileException, FileException)
MT_DECLARE_EXCEPTION(UnknownURISchemeException, RuntimeException)

MT_DECLARE_EXCEPTION(ApplicationException, Exception)
MT_DECLARE_EXCEPTION(BadCastException, RuntimeException)

MT_DECLARE_EXCEPTION(LoggerException, Exception)

MT_DECLARE_EXCEPTION(QueueException, Exception)

MT_DECLARE_EXCEPTION(XmlException, Exception)
MT_DECLARE_EXCEPTION(XmlExceptionNoMandatoryParamter, XmlException)

MT_DECLARE_EXCEPTION(ThreadException, Exception)
MT_DECLARE_EXCEPTION(ThreadAlreadyRunningException, ThreadException)
MT_DECLARE_EXCEPTION(ThreadTooManyThreadsException, ThreadException)
MT_DECLARE_EXCEPTION(ThreadInvalidArgumentException, ThreadException)
MT_DECLARE_EXCEPTION(ThreadNoMemoryException, ThreadException)
MT_DECLARE_EXCEPTION(ThreadCantStartException, ThreadException)


MT_DECLARE_EXCEPTION(NetworkException, Exception)
MT_DECLARE_EXCEPTION(TcpSocketException, NetworkException)

MT_DECLARE_EXCEPTION(DbException, Exception)
MT_DECLARE_EXCEPTION(NoDataDbException, DbException)
MT_DECLARE_EXCEPTION(ServerGoneDbException, DbException)
MT_DECLARE_EXCEPTION(NoResultSetDbException, DbException)
MT_DECLARE_EXCEPTION(NoSuchTableDbException, DbException)
MT_DECLARE_EXCEPTION(DbExistDbException, DbException)




} // namespace mt

#endif /* EXCEPTION_H */
