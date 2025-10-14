/* ======================================================================
**  WidgetDrag.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2009 by Max Schlee
** ======================================================================
*/
#ifndef _WidgetDrag_h_
#define _WidgetDrag_h_

#include <QDrag>

// ======================================================================
class WidgetMimeData : public QMimeData {
private:
    QWidget* m_pwgt;

public:
    WidgetMimeData() : QMimeData()
    {
    }

    virtual ~WidgetMimeData()
    {
    }

    static QString mimeType()
    {
        return "application/widget";
    }    

    void setWidget(QWidget* pwgt)
    {
        m_pwgt = pwgt;
        setData(mimeType(), QByteArray());
    }

    QWidget* widget() const
    {
        return m_pwgt;
    }
};

// ======================================================================
class WidgetDrag : public QDrag {
public:
    WidgetDrag(QWidget* pwgtDragSource = 0) : QDrag(pwgtDragSource)
    {
    }

    void setWidget(QWidget* pwgt)
    {
         WidgetMimeData* pmd = new WidgetMimeData;
         pmd->setWidget(pwgt);
         setMimeData(pmd);
    }
};

#endif //_WidgetDrag_h_
