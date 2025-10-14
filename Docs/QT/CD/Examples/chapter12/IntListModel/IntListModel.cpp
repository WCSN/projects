/* ======================================================================
**  IntListModel.cpp
** ======================================================================
**
** ======================================================================
**  Copyright (c) 2007 by Max Schlee
** ======================================================================
*/
#include <QtGui>
#include "IntListModel.h"

// ----------------------------------------------------------------------
IntListModel::IntListModel(const QList<int>& list, QObject* pobj/*=0*/) 
        : QAbstractListModel(pobj)
        , m_list(list) 
{
}

// ----------------------------------------------------------------------
QVariant IntListModel::data(const QModelIndex& index, int nRole) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    return (nRole == Qt::DisplayRole || nRole == Qt::EditRole) 
           ? m_list.at(index.row()) 
           : QVariant();
}

// ----------------------------------------------------------------------
bool IntListModel::setData(const QModelIndex& index, 
                           const QVariant&    value, 
                           int                nRole
                          )
{
    if (index.isValid() && nRole == Qt::EditRole) {
        m_list.replace(index.row(), value.value<int>());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------
int IntListModel::rowCount(const QModelIndex& parent/*=QModelIndex()*/
                          ) const
{
    Q_UNUSED(parent);
    return m_list.size();
}

// ----------------------------------------------------------------------
QVariant IntListModel::headerData(int             nSection, 
                                  Qt::Orientation orientation, 
                                  int             nRole/*=DisplayRole*/
                                 ) const
{
    if (nRole != Qt::DisplayRole) {
        return QVariant();
    }
    return (orientation == Qt::Horizontal) ? QString("Number") 
                                           : QString::number(nSection);
}

// ----------------------------------------------------------------------
Qt::ItemFlags IntListModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsEditable)
                           : flags;
}
