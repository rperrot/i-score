
#include "Remove.hpp"

#include <iostream> //DEBUG

using namespace DeviceExplorer::Command;

Remove::Remove()
    : iscore::SerializableCommand("", "Remove ", "")
{

}

void
Remove::set(const QModelIndex& parentIndex, int row,
                                 const QByteArray& data,
                                 const QString& text,
                                 DeviceExplorerModel* model)
{
    Q_ASSERT(model);
    m_model = model;
    m_data = data;
    m_parentPath = model->pathFromIndex(parentIndex);
    m_row = row;

    setText(text);
}


void
Remove::undo()
{
    Q_ASSERT(m_model);

    QModelIndex parentIndex = m_model->pathToIndex(m_parentPath);

    const bool result = m_model->insertTreeData(parentIndex, m_row, m_data);
    m_model->setCachedResult(result);
}

void
Remove::redo()
{
    Q_ASSERT(m_model);
    QModelIndex parentIndex = m_model->pathToIndex(m_parentPath);

    QModelIndex index = parentIndex.child(m_row, 0);
    const bool resultG = m_model->getTreeData(index, m_data);

    if(! resultG)
    {
        m_model->setCachedResult(resultG);
        return;
    }

    const bool result = m_model->removeRows(m_row, 1, parentIndex);
    m_model->setCachedResult(result);
}

bool
Remove::mergeWith(const Command* /*other*/)
{
    return false;
}


void
Remove::serializeImpl(QDataStream& d) const
{
    DeviceExplorerModel::serializePath(d, m_parentPath);
    d << (qint32) m_row;

    d << (qint32) m_data.size();
    d.writeRawData(m_data.data(), m_data.size());

}

void
Remove::deserializeImpl(QDataStream& d)
{
    DeviceExplorerModel::deserializePath(d, m_parentPath);
    qint32 v;
    d >> v;
    m_row = v;

    d >> v;
    int size = v;
    m_data.resize(size);
    d.readRawData(m_data.data(), size);
}