#ifndef SRC_GUIMODEL_QEDITORMODIFICATIONLIST_H_
#define SRC_GUIMODEL_QEDITORMODIFICATIONLIST_H_

#include <QtCore>
#include "QEditorModification.h"

/**
 * @brief Represents a list of editor modifications.
 * 
 * The QEditorModificationList class manages a list of QEditorModification objects,
 * providing methods to add, remove, and query modifications.
 */
class QEditorModificationList
{
public:
    /**
     * @brief Adjusts offsets of modifications starting from a specified index.
     * @param iStartIdx Starting index from which to adjust offsets.
     * @param iDelta Delta value by which to adjust offsets.
     * @param mod The modification that triggered the adjustment.
     */
    void adjustOffsets(int iStartIdx, int iDelta, QEditorModification mod);
    
    /**
     * @brief Adds a new modification to the list.
     * @param iOffset The offset where the modification occurred.
     * @param action The action performed (add, update, or remove).
     * @param iSizeBytes The size of the modification in bytes.
     * @param data The data associated with the modification.
     */
    void addModification(const qint64 iOffset, const Action action, const qint64 iSizeBytes, const QByteArray data);
    
    /**
     * @brief Adds a new modification to the list.
     * @param modification The QEditorModification object representing the modification.
     */
    void addModification(const QEditorModification &modification);

    /**
     * @brief Removes a modification from the list at the specified index.
     * @param iIndex The index of the modification to remove.
     */
    void removeModification(int iIndex);

    /**
     * @brief Removes the last modification from the list.
     */
    void removeLastModification();

    /**
     * @brief Clears all modifications from the list.
     */
    void clearModifications();

    /**
     * @brief Retrieves the last modification that occurred at a specific offset.
     * @param iOffset The offset to query.
     * @return The last modification at the specified offset.
     */
    QEditorModification lastModificationAtPosition(qint64 iOffset) const;
    
    /**
     * @brief Checks if there exists a modification at the specified position.
     * @param iPosition The position to check.
     * @return True if a modification exists at the position; false otherwise.
     */
    bool existsPosition(qint64 iPosition);

private:
    QList<QEditorModification> modifications; /**< List of editor modifications. */
};

#endif /* SRC_GUIMODEL_QEDITORMODIFICATIONLIST_H_ */
