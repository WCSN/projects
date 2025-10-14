/* ======================================================================
**  IntListModel.h
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#ifndef _IntListModel_h_
#define _IntListModel_h_

#include <QAbstractListModel>

// ======================================================================
class IntListModel : public QAbstractListModel {
Q_OBJECT
private:
    QList<int> m_list;

public:
    IntListModel(const QList<int>& list, QObject* pobj = 0); 

    QVariant data(const QModelIndex& index, int nRole) const;

    bool setData(const QModelIndex& index, 
                 const QVariant&    value, 
                 int                nRole
                );

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant headerData(int             nSection, 
                        Qt::Orientation orientation, 
                        int             nRole = Qt::DisplayRole
                       ) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
};
#endif  //_IntListModel_h_
