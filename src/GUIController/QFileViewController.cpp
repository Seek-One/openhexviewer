/*
 * QFileViewController.cpp
 *
 *  Created on: 18 juil. 2018
 *      Author: ebeuque
 */

#include <math.h>
#include <algorithm>

#include "GUI/QFileView.h"

#include "QFileViewController.h"

QFileViewController::QFileViewController(QFileView* pFileView)
{
	m_pFileView = pFileView;
	m_iFileSize = 0;
	m_iFilePos = 0;
	m_bIsFileOpen = false;

	m_iBytePerLine = 0;
	m_iVisibleRowCount = 0;
	m_iTotalRowCount = 0;

	connect(m_pFileView, SIGNAL(sizeChanged()), this, SLOT(updateView()));
	connect(m_pFileView, SIGNAL(rowChanged(int)), this, SLOT(moveToRow(int)));
}

QFileViewController::~QFileViewController()
{

}

bool QFileViewController::openFile(const QString& szFilePath)
{
	bool bRes = true;

	if(m_bIsFileOpen){
		closeFile();
	}

	m_file.setFileName(szFilePath);
	bRes = m_file.open(QIODevice::ReadOnly);
	if(bRes){
		m_bIsFileOpen = true;
		m_iFileSize = m_file.size();

		updateDisplayData();

		bRes = readFile(0);
	}else{
		qWarning("[File] Unable to open file: %s", qPrintable(szFilePath));
	}

	return bRes;
}

bool QFileViewController::readFile(qint64 iStartOffset)
{
	bool bRes;
	m_iFilePos = iStartOffset;
	bRes = m_file.seek(m_iFilePos);

	int iBufferSize = m_iBytePerLine;
	char* pBuffer = new char[m_iBytePerLine];

	quint32 iOffset;
	qint64 iNbRead;
	QString szOffsetText;
	QString szHexText;
	QString szHumanText;
	char c;

	QString szTmp;

	if(bRes){
		for(int i=0; i<m_iVisibleRowCount; i++){
			iNbRead = m_file.read(pBuffer, iBufferSize);
			if(iNbRead > 0){

				// Prepend a line break if not first row
				if(i>0){
					szOffsetText += "\n";
					szHexText += "\n";
					szHumanText += "\n";
				}

				iOffset = (quint32)(m_iFilePos+i*m_iBytePerLine);
				szTmp.sprintf("0x%08X", iOffset);
				szOffsetText += szTmp;

				for(int j=0; j<iNbRead; j++)
				{
					// Set hex text
					c = pBuffer[j];
					szTmp.sprintf("%02X", (unsigned char)c);
					szHexText += szTmp+" ";

					// Set human text
					if(c >= 0x20 && c <= 0x7E){
						szHumanText += c;
					}else{
						szHumanText += ".";
					}
				}
			}
		}

		m_pFileView->setOffsetText(szOffsetText);
		m_pFileView->setHexText(szHexText);
		m_pFileView->setHumanText(szHumanText);
	}

	if(pBuffer){
		delete[] pBuffer;
		pBuffer = NULL;
	}

	return bRes;
}

void QFileViewController::closeFile()
{
	if(m_bIsFileOpen){
		m_file.close();
		m_bIsFileOpen = false;
	}
}

void QFileViewController::selectFileData(qint64 offset, qint64 size)
{
	int iRowStart = offset / m_iBytePerLine;
	int iColStart = offset % m_iBytePerLine;

	qint64 offsetEnd = offset+size-1;
	int iRowEnd = offsetEnd / m_iBytePerLine;
	//int iColEnd = offsetEnd % m_iBytePerLine;

	int iFirstVisibleRow = std::min(iRowStart, std::max(m_iTotalRowCount-m_iVisibleRowCount, 0));
	qint64 iMaxChar = m_iVisibleRowCount*m_iBytePerLine - iColStart;
	qint64 iSize = std::min(size, iMaxChar);
	if(iFirstVisibleRow < iRowStart){
		iColStart += (iRowStart-iFirstVisibleRow) * m_iBytePerLine;
	}

	m_pFileView->moveToRow(iFirstVisibleRow);
	m_pFileView->selectText(iColStart, iColStart + (int)(iSize), (iRowStart-iFirstVisibleRow), (iRowEnd-iRowStart));
}

void QFileViewController::updateDisplayData()
{
	m_iBytePerLine = m_pFileView->getBytePerLine();
	m_iTotalRowCount = ceil(m_iFileSize / (float)m_iBytePerLine);
	m_iVisibleRowCount = m_pFileView->getVisibleRowCount();
	m_pFileView->setRowCount(m_iTotalRowCount);
}

void QFileViewController::updateView()
{
	if(m_bIsFileOpen){
		updateDisplayData();
		readFile(m_iFilePos);
	}
}

void QFileViewController::moveToRow(int iRow)
{
	iRow = qMin(iRow, m_iTotalRowCount - m_iVisibleRowCount);

	qint64 iOffset = (((qint64)iRow) * m_iBytePerLine);
	readFile(iOffset);
}
