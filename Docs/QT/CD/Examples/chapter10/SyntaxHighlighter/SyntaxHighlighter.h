/* ======================================================================
**  SyntaxHighlighter.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _SyntaxHighlighter_h_
#define _SyntaxHighlighter_h_

#include <QSyntaxHighlighter>

class QTextDocument;

// ======================================================================
class SyntaxHighlighter: public QSyntaxHighlighter {
Q_OBJECT
protected:
    enum { NormalState = -1, InsideCStyleComment, InsideCString };

    virtual void highlightBlock(const QString&);

    QString getKeyword(int i, const QString& str) const;

public:
    SyntaxHighlighter(QTextDocument* parent = 0);
};
#endif //_SyntaxHighlighter_h_
