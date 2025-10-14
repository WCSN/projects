/*
 * Title       : Exception.cpp
 * Date        : 13.07.2010
 * Author      : Andrei Borodaenko
 * Description : ***
 */

#include <mt/common/Exception.h>

namespace mt {

Exception::Exception(const std::string& msg, int code) : m_msg(msg), m_code(code) {

}


Exception::Exception(const std::string& msg, const std::string& arg, int code) : m_msg(msg), m_code(code) {
    if (!arg.empty()) {
        m_msg.append(": ");
        m_msg.append(arg);
    }
}

Exception::Exception(const Exception& exc) : std::exception(exc), m_msg(exc.m_msg), m_code(exc.m_code) {

}

Exception::Exception(int code) : m_code(code) {

}

Exception::~Exception() throw() {

}

Exception& Exception::operator=(const Exception& exc) {
    if (this != &exc) {
        m_msg = exc.m_msg;
        m_code = exc.m_code;
    }

    return *this;
}

std::string Exception::displayText() const {
    std::string text = name();
    if (!m_msg.empty()) {
        text.append(": ");
        text.append(m_msg);
    }
    return text;
}

void Exception::extendedMessage(const std::string& arg) {
    if (!arg.empty()) {
        if (!m_msg.empty()) { m_msg.append(": "); }
        m_msg.append(arg);
    }
}

MT_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
MT_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
MT_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
MT_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
MT_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
MT_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException, "Not implemented")
MT_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
MT_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
MT_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
MT_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
MT_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

MT_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
MT_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
MT_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
MT_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
MT_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
MT_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException, "Error in regular expression")
MT_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
MT_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
MT_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
MT_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
MT_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
MT_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
MT_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
MT_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

MT_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
MT_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
MT_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException, "Circular reference")
MT_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
MT_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
MT_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
MT_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
MT_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
MT_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
MT_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
MT_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
MT_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
MT_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
MT_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
MT_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
MT_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
MT_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException, "Unknown URI scheme")


MT_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
MT_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")

MT_IMPLEMENT_EXCEPTION(LoggerException, Exception, "Logger exception")

MT_IMPLEMENT_EXCEPTION(QueueException, Exception, "Queue interrupted")


MT_IMPLEMENT_EXCEPTION(XmlException, Exception, "XML config exception")
MT_IMPLEMENT_EXCEPTION(XmlExceptionNoMandatoryParamter, XmlException, "No mandatory parameter at configuration")

MT_IMPLEMENT_EXCEPTION(ThreadException, Exception, "Common thread exception")
MT_IMPLEMENT_EXCEPTION(ThreadAlreadyRunningException, ThreadException, "Thread already running exception")
MT_IMPLEMENT_EXCEPTION(ThreadTooManyThreadsException, ThreadException, "Too many threads exception")
MT_IMPLEMENT_EXCEPTION(ThreadInvalidArgumentException, ThreadException, "Thread invalid argument exception")
MT_IMPLEMENT_EXCEPTION(ThreadNoMemoryException, ThreadException, "Thread no memory exception")
MT_IMPLEMENT_EXCEPTION(ThreadCantStartException, ThreadException, "Can't start thread exception")

MT_IMPLEMENT_EXCEPTION(NetworkException, Exception, "Network exception")
MT_IMPLEMENT_EXCEPTION(TcpSocketException, NetworkException, "Tcp socket exception")


MT_IMPLEMENT_EXCEPTION(DbException, Exception, "Database exception")
MT_IMPLEMENT_EXCEPTION(NoDataDbException, DbException, "Attempt to read column without prior row fetch")
MT_IMPLEMENT_EXCEPTION(ServerGoneDbException, DbException, "MySQL server has gone away")
MT_IMPLEMENT_EXCEPTION(NoResultSetDbException, DbException, "Attempt to read a row while there is no result set associated with the statement")
MT_IMPLEMENT_EXCEPTION(NoSuchTableDbException, DbException, "Table doesn't exist")
MT_IMPLEMENT_EXCEPTION(DbExistDbException, DbException, "database already exist")




//TODO add different exception accordingly diff errors

} // namespace mt
