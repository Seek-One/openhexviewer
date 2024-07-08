#include "QEditorModificationList.h"

void QEditorModificationList::adjustOffsets(int iStartIdx, int iDelta, QEditorModification mod)
{ 
    for (int i = iStartIdx; i < modifications.size(); ++i) {
        if (modifications.at(i).offset >= mod.offset) {
            modifications[i].setOffset(modifications[i].getOffset() + iDelta);
        }
    }
}

void QEditorModificationList::addModification(const qint64 iOffset, const Action action, const qint64 iSizeBytes, const QByteArray data)
{
    QEditorModification modification(iOffset, action, iSizeBytes, data);
    if (modification.action != Action::Update) {
        int iMove = (modification.action == Action::Add) ? 1 : -1;
        adjustOffsets(0, iMove, modification);
    }
    modifications.append(modification);
}

void QEditorModificationList::addModification(const QEditorModification &modification) 
{
    if (modification.action != Action::Update) {
        int iMove = (modification.action == Action::Add) ? 1 : -1;
        adjustOffsets(0, iMove, modification);
    }
    modifications.append(modification);
}

void QEditorModificationList::removeModification(int iIndex) 
{
    if (iIndex >= 0 && iIndex < modifications.size()) {
        QEditorModification modification = modifications.at(iIndex);
        if (modification.action != Action::Update) {
            int iMove = (modification.action == Action::Add) ? -1 : 1;
            adjustOffsets(iIndex + 1, iMove, modification);
        }
        modifications.removeAt(iIndex);
    } else {
        qWarning() << "[Modification] Invalid index or modification does not exist.";
    }
}

void QEditorModificationList::removeLastModification()
{
    removeModification(modifications.length() - 1);
}

void QEditorModificationList::clearModifications()
{
    modifications.clear();
}

QEditorModification QEditorModificationList::lastModificationAtPosition(qint64 iOffset) const
{
    for (int i = modifications.size() - 1; i >= 0; --i) {
        if (modifications.at(i).offset == iOffset) {
            return modifications.at(i);
        }
    }
    throw std::runtime_error("[Modification] No modification found at the given position");
}

bool QEditorModificationList::existsPosition(qint64 iPosition)
{
    for (QEditorModification iter : modifications) {
        if (iter.offset == iPosition) {
            return true;
        }
    }
    return false;
}
