#ifndef SRC_GUIMODEL_QEDITORMODIFICATION_H_
#define SRC_GUIMODEL_QEDITORMODIFICATION_H_

#include <QtCore>

enum class Action {
    Add,
    Update,
    Remove
};

class QEditorModification
{
public:
    qint64 offset;
    Action action;
    qint64 sizeBytes;
    QByteArray data;

    QEditorModification(qint64 iOffset, Action action, qint64 iSizeBytes, const QByteArray &data);

    void setOffset(qint64 iNewOffset);

    qint64 getOffset() const;
};

#endif // SRC_GUIMODEL_QEDITORMODIFICATION_H_
