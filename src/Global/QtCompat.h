//
// Created by ebeuque on 26/07/2021.
//

#ifndef OPENDBVIEWER_QTCOMPAT_H
#define OPENDBVIEWER_QTCOMPAT_H

#include <QtGlobal>

// Added in Qt 5.11
#if QT_VERSION >= QT_VERSION_CHECK(5,11,0)
#define USE_QTFMHORIZONTALADVANCE
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#define USE_NO_QREGEXP
#define USE_QLIBRARYINFO_PATH
#endif

// Added in Qt 5.5.0
#if QT_VERSION >= QT_VERSION_CHECK(5,5,0)
#define USE_QTASPRINTF
#endif

#ifdef USE_QTASPRINTF
#define QStringASPrintf(str, format, ...) str = QString::asprintf(format, __VA_ARGS__)
#else
#define QStringASPrintf(str, format, ...) str.sprintf(format, __VA_ARGS__)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#define USE_NO_QREGEXP
#endif

#endif //OPENDBVIEWER_QTCOMPAT_H
