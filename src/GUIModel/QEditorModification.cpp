#include "QEditorModification.h"

QEditorModification::QEditorModification(qint64 iOffset, Action action, qint64 iSizeBytes, const QByteArray &data)
    : offset(iOffset), action(action), sizeBytes(iSizeBytes), data(data)
{
}

void QEditorModification::setOffset(qint64 iNewOffset)
{
    offset = iNewOffset;
}

qint64 QEditorModification::getOffset() const
{
    return offset;
}
