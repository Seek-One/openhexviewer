#ifndef SRC_GUIMODEL_QEDITORMODIFICATION_H_
#define SRC_GUIMODEL_QEDITORMODIFICATION_H_

#include <QtCore>

enum class Action {
    Add,
    Update,
    Remove
};

/**
 * @brief Represents a modification made to an editor.
 * 
 * The QEditorModification class encapsulates information about a modification 
 * made to an editor, including the offset, action (add, update, or remove), 
 * size in bytes, and the data associated with the modification.
 */
class QEditorModification
{
public:
    qint64 offset; /**< Offset where the modification occurred. */
    Action action; /**< Action performed (add, update, or remove). */
    qint64 sizeBytes; /**< Size of the modification in bytes. */
    QByteArray data; /**< Data associated with the modification. */

    /**
     * @brief Constructs a QEditorModification object.
     * @param iOffset The offset where the modification occurred.
     * @param action The action performed (add, update, or remove).
     * @param iSizeBytes The size of the modification in bytes.
     * @param data The data associated with the modification.
     */
    QEditorModification(qint64 iOffset, Action action, qint64 iSizeBytes, const QByteArray &data);

    /**
     * @brief Sets a new offset for the modification.
     * @param iNewOffset The new offset value.
     */
    void setOffset(qint64 iNewOffset);

    /**
     * @brief Retrieves the offset where the modification occurred.
     * @return The offset value.
     */
    qint64 getOffset() const;
};

#endif // SRC_GUIMODEL_QEDITORMODIFICATION_H_
