#ifndef SRC_GUIMODEL_QEDITORMODIFICATIONLIST_H_
#define SRC_GUIMODEL_QEDITORMODIFICATIONLIST_H_

#include <QtCore>
#include "QEditorModification.h"

class QEditorModificationList
{
public:
    void adjustOffsets(int iStartIdx, int iDelta, QEditorModification mod);
    void addModification(const qint64 iOffset, const Action action, const qint64 iSizeBytes, const QByteArray data);
    void addModification(const QEditorModification &modification);
    void removeModification(int iIndex);
    void clearModifications();
    QEditorModification lastModificationAtPosition(qint64 iOffset) const;
    bool existsPosition(qint64 iPosition);

private:
    QList<QEditorModification> modifications;
};

#endif /* SRC_GUIMODEL_QEDITORMODIFICATIONLIST_H_ */
