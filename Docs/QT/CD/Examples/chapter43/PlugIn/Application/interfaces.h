/* ======================================================================
**  interfaces.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _interfaces_h_
#define _interfaces_h_

class QString;
class QStringList;

// ======================================================================
class StringInterface {
public:
    virtual ~StringInterface() {}

    virtual QStringList operations() const = 0;

    virtual QString operation(const QString& strText, 
                              const QString& strOperation
                             ) = 0;
};

Q_DECLARE_INTERFACE(StringInterface,
                    "com.mysoft.Application.StringInterface/1.0"
                   )
#endif //_interfaces_h_
