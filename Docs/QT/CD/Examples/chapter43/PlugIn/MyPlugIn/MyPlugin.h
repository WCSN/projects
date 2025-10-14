/* ======================================================================
**  MyPlugin.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _MyPlugin_h_
#define _MyPlugin_h_

#include <QObject>
#include "../Application/interfaces.h"

// ======================================================================
class MyPlugin : public QObject, public StringInterface {
Q_OBJECT
Q_INTERFACES(StringInterface)

private:
    QString oddUpper(const QString& str);

public:
    virtual ~MyPlugin();

    virtual QStringList operations(                              ) const;
    virtual QString     operation (const QString&, const QString&);
};
#endif //_MyPlugin_h_
