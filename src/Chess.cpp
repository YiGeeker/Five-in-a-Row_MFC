#include "StdAfx.h"
#include "Chess.h"


CChess::CChess(unsigned int nSize)
{
	unsigned int i;
	m_nSize = nSize;
	m_bHold = false;
	// The first is black
	m_enNow = PLAYER::BLACK;

	// Initialize the whole board array to zero
	m_Board = new PLAYER*[m_nSize];
	for (i = 0; i < m_nSize; i++)
	{
		m_Board[i] = new PLAYER[m_nSize]();
	}

	m_lpbmpHold = NULL;
}

CChess::~CChess()
{
	unsigned int i;
	for (i = 0; i < m_nSize; i++)
	{
		delete[] m_Board[i];
	}
	delete[] m_Board;
}

unsigned int CChess::GetSize()
{
	return m_nSize;
}

void CChess::SetBackground(CBitmap* lpBitmap)
{
	BITMAP bmpInfo;
	byte* lpBits;
	unsigned int nSize;

	lpBitmap->GetBitmap(&bmpInfo);
	nSize = bmpInfo.bmWidth * bmpInfo.bmHeight * (bmpInfo.bmWidthBytes / bmpInfo.bmWidth);
	lpBits = new byte[nSize];
	ZeroMemory(lpBits, nSize);
	lpBitmap->GetBitmapBits(nSize, lpBits);
	m_bmpBoard.CreateBitmap(bmpInfo.bmWidth, bmpInfo.bmHeight, bmpInfo.bmPlanes, bmpInfo.bmBitsPixel, lpBits);
}

bool CChess::Create()
{
	unsigned int i;
	CDC dcMemory;
	BITMAP bmpInfo;

	CBitmap* lpBmp;				// Bitmap pointer of DC
	byte* lpBits;				// Byte address of buffer bitmap
	unsigned int nSize;			// Size of buffer bitmap

	m_bmpBoard.GetBitmap(&bmpInfo);
	if (bmpInfo.bmWidth < bmpInfo.bmHeight)
	{
		m_nCellLen = bmpInfo.bmWidth / m_nSize;
	}
	else
	{
		m_nCellLen = bmpInfo.bmHeight / m_nSize;
	}

	// Coodinates of start point
	const unsigned int nStart = m_nCellLen / 2;

	dcMemory.CreateCompatibleDC(NULL);
	dcMemory.SelectObject(&m_bmpBoard);

	// Draw chess board
	dcMemory.SelectObject(new CPen(PS_SOLID, 1, COLORREF(RGB(0, 0, 0))));
	// Draw horizontal line
	for (i = 0; i < m_nSize; i++)
	{
		dcMemory.MoveTo(nStart, nStart + m_nCellLen*i);
		dcMemory.LineTo(nStart + m_nCellLen*(m_nSize - 1), nStart + m_nCellLen*i);
	}
	// Draw vertical line
	for (i = 0; i < m_nSize; i++)
	{
		dcMemory.MoveTo(nStart + m_nCellLen*i, nStart);
		dcMemory.LineTo(nStart + m_nCellLen*i, nStart + m_nCellLen*(m_nSize - 1));
	}

	// Draw dot, at nine frames from the four corners and at the center, with a radius of 3
	dcMemory.SelectObject(new CBrush(COLORREF(RGB(0, 0, 0))));
	//Top left dot
	dcMemory.Ellipse(nStart + m_nCellLen * 3 - 3, nStart + m_nCellLen * 3 - 3,
		nStart + m_nCellLen * 3 + 3, nStart + m_nCellLen * 3 + 3);
	//Top right dot
	dcMemory.Ellipse(nStart + m_nCellLen * (m_nSize - 4) - 3, nStart + m_nCellLen * 3 - 3,
		nStart + m_nCellLen * (m_nSize - 4) + 3, nStart + m_nCellLen * 3 + 3);
	//Bottom left dot
	dcMemory.Ellipse(nStart + m_nCellLen * 3 - 3, nStart + m_nCellLen * (m_nSize - 4) - 3,
		nStart + m_nCellLen * 3 + 3, nStart + m_nCellLen * (m_nSize - 4) + 3);
	//Bottom right dot
	dcMemory.Ellipse(nStart + m_nCellLen * (m_nSize - 4) - 3, nStart + m_nCellLen * (m_nSize - 4) - 3,
		nStart + m_nCellLen * (m_nSize - 4) + 3, nStart + m_nCellLen * (m_nSize - 4) + 3);
	//centeral dot
	dcMemory.Ellipse(nStart + m_nCellLen * (m_nSize / 2) - 3, nStart + m_nCellLen * (m_nSize / 2) - 3,
		nStart + m_nCellLen * (m_nSize / 2) + 3, nStart + m_nCellLen * (m_nSize / 2) + 3);

	// The chess board background initialization
	lpBmp = dcMemory.GetCurrentBitmap();
	lpBmp->GetBitmap(&bmpInfo);
	nSize = bmpInfo.bmWidth * bmpInfo.bmHeight * (bmpInfo.bmWidthBytes / bmpInfo.bmWidth);
	lpBits = new byte[nSize];
	ZeroMemory(lpBits, nSize);
	lpBmp->GetBitmapBits(nSize, lpBits);

	m_bmpChess.DeleteObject();//Delete the past bitmap
	if (!m_bmpChess.CreateBitmap(bmpInfo.bmWidth, bmpInfo.bmHeight, bmpInfo.bmPlanes, bmpInfo.bmBitsPixel, lpBits))
	{
		delete[] lpBits;
		return false;
	}
	delete[] lpBits;
	return true;
}

bool CChess::Show(CDC* pDC, int x, int y)
{
	CDC dcMemory;
	BITMAP bmpInfo;

	dcMemory.CreateCompatibleDC(pDC);
	if (m_bHold)
	{
		dcMemory.SelectObject(m_lpbmpHold);
		m_lpbmpHold->GetBitmap(&bmpInfo);
	}
	else
	{
		dcMemory.SelectObject(&m_bmpChess);
		m_bmpChess.GetBitmap(&bmpInfo);
	}

	if (!pDC->BitBlt(x, y, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY))
	{
		return false;
	}
	return true;
}

void CChess::GetBitmap(BITMAP* bmpInfo)
{
	m_bmpChess.GetBitmap(bmpInfo);
}

bool CChess::GetPiont(int x, int y, POINT& Point)
{
	unsigned int i, j;
	// Coodinates of start point
	const unsigned int nStart = m_nCellLen / 2;

	// A piece can be placed only if the length between the mouse
	// point and the board point is below one eighth of the frame
	// length
	for (i = 0; i < m_nSize; i++)
	{
		for (j = 0; j < m_nSize; j++)
		{
			// The beginning coodinates is (0, 0)
			if ((nStart + i*m_nCellLen - x)*(nStart + i*m_nCellLen - x) +
				(nStart + j*m_nCellLen - y)*(nStart + j*m_nCellLen - y) < m_nCellLen*m_nCellLen / 64)
			{
				// Check if this point is blank
				if (m_Board[i][j] != 0) break;
				Point.x = i;
				Point.y = j;
				return true;
			}
		}
	}
	return false;
}

void CChess::BeginHold()
{
	BITMAP bmpInfo;

	m_bHold = true;

	if (m_lpbmpHold == NULL) m_lpbmpHold = new CBitmap();
	m_bmpChess.GetBitmap(&bmpInfo);
	m_lpbmpHold->CreateBitmap(bmpInfo.bmWidth, bmpInfo.bmHeight, bmpInfo.bmPlanes, bmpInfo.bmBitsPixel, NULL);
}

bool CChess::HoldPiece(int x, int y)
{
	CDC dcMemory, dcBmp;
	BITMAP bmpInfo;

	// The radius of the piece
	const unsigned int r = m_nCellLen / 4;

	if (!m_bHold) return false;

	dcMemory.CreateCompatibleDC(NULL);
	dcMemory.SelectObject(m_lpbmpHold);

	dcBmp.CreateCompatibleDC(NULL);
	dcBmp.SelectObject(m_bmpChess);
	m_bmpChess.GetBitmap(&bmpInfo);
	dcMemory.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcBmp, 0, 0, SRCCOPY);

	if (m_enNow == PLAYER::BLACK)
	{
		dcMemory.SelectObject(new CPen(PS_SOLID, 1, COLORREF(RGB(0, 0, 0))));
		dcMemory.SelectObject(new CBrush(COLORREF(RGB(0, 0, 0))));
	}
	else
	{
		dcMemory.SelectObject(new CPen(PS_SOLID, 1, COLORREF(RGB(255, 255, 255))));
		dcMemory.SelectObject(new CBrush(COLORREF(RGB(255, 255, 255))));
	}
	dcMemory.Ellipse(x - r, y - r, x + r, y + r);
	return true;
}

void CChess::ReleasePiece()
{
	m_bHold = false;

	if (m_lpbmpHold != NULL)
	{
		delete m_lpbmpHold;
		m_lpbmpHold = NULL;
	}
}

bool CChess::PlacePiece(POINT Point)
{
	CDC dcBmp, dcMemory;

	// The beginning coodinates
	const unsigned int nStart = m_nCellLen / 2;
	// The radius of piece if one fourth of the frame
	const unsigned int r = m_nCellLen / 4;

	this->ReleasePiece();

	dcMemory.CreateCompatibleDC(NULL);
	dcMemory.SelectObject(m_bmpChess);

	// Change the last mark point to unmark
	if (m_vPiecePoint.size() != 0)
	{
		dcBmp.CreateCompatibleDC(NULL);
		dcBmp.SelectObject(&m_bmpBoard);
		dcMemory.BitBlt(nStart + m_vPiecePoint.back().x*m_nCellLen - r * 2, nStart + m_vPiecePoint.back().y*m_nCellLen - r * 2, r * 4, r * 4,
			&dcBmp, nStart + m_vPiecePoint.back().x*m_nCellLen - r * 2, nStart + m_vPiecePoint.back().y*m_nCellLen - r * 2, SRCCOPY);

		if (m_enNow == PLAYER::BLACK)
		{
			dcMemory.SelectObject(new CPen(PS_SOLID, 1, COLORREF(RGB(255, 255, 255))));
			dcMemory.SelectObject(new CBrush(COLORREF(RGB(255, 255, 255))));
		}
		else
		{
			dcMemory.SelectObject(new CPen(PS_SOLID, 1, COLORREF(RGB(0, 0, 0))));
			dcMemory.SelectObject(new CBrush(COLORREF(RGB(0, 0, 0))));
		}

		dcMemory.Ellipse(nStart + m_vPiecePoint.back().x*m_nCellLen - r, nStart + m_vPiecePoint.back().y*m_nCellLen - r,
			nStart + m_vPiecePoint.back().x *m_nCellLen + r, nStart + m_vPiecePoint.back().y*m_nCellLen + r);
	}

	if (m_enNow == PLAYER::BLACK)
	{
		dcMemory.SelectObject(new CPen(PS_SOLID, 1, COLORREF(RGB(0, 0, 0))));
		dcMemory.SelectObject(new CBrush(COLORREF(RGB(0, 0, 0))));
	}
	else
	{
		dcMemory.SelectObject(new CPen(PS_SOLID, 1, COLORREF(RGB(255, 255, 255))));
		dcMemory.SelectObject(new CBrush(COLORREF(RGB(255, 255, 255))));
	}
	// The beginning coodinates is (0, 0)
	dcMemory.Ellipse(nStart + Point.x*m_nCellLen - r, nStart + Point.y*m_nCellLen - r,
		nStart + Point.x *m_nCellLen + r, nStart + Point.y*m_nCellLen + r);
	// Draw a frame at this point
	dcMemory.SelectObject(new CPen(PS_SOLID, 1, COLORREF(RGB(255, 0, 0))));
	// Draw top left frame
	dcMemory.MoveTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen - r * 3 / 2);
	dcMemory.LineTo(nStart + Point.x*m_nCellLen - r, nStart + Point.y*m_nCellLen - r * 3 / 2);
	dcMemory.MoveTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen - r * 3 / 2);
	dcMemory.LineTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen - r);
	// Draw top right frame
	dcMemory.MoveTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen - r * 3 / 2);
	dcMemory.LineTo(nStart + Point.x*m_nCellLen + r, nStart + Point.y*m_nCellLen - r * 3 / 2);
	dcMemory.MoveTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen - r * 3 / 2);
	dcMemory.LineTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen - r);
	// Draw bottom left frame
	dcMemory.MoveTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen + r * 3 / 2);
	dcMemory.LineTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen + r);
	dcMemory.MoveTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen + r * 3 / 2);
	dcMemory.LineTo(nStart + Point.x*m_nCellLen - r, nStart + Point.y*m_nCellLen + r * 3 / 2);
	// Draw bottom right frame
	dcMemory.MoveTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen + r * 3 / 2);
	dcMemory.LineTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen + r);
	dcMemory.MoveTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen + r * 3 / 2);
	dcMemory.LineTo(nStart + Point.x*m_nCellLen + r, nStart + Point.y*m_nCellLen + r * 3 / 2);

	m_Board[Point.x][Point.y] = m_enNow;
	m_vPiecePoint.push_back(Point);

	return true;
}

void CChess::ChangePlayer()
{
	if (m_enNow == PLAYER::BLACK)
	{
		m_enNow = PLAYER::WHITE;
	}
	else
	{
		m_enNow = PLAYER::BLACK;
	}
}

bool CChess::WithdrawPiece()
{
	unsigned int i;
	CDC dcBmp, dcMemory;
	POINT Point;

	if (m_vPiecePoint.size() <= 1) return false;

	// The beginning coodinates
	const unsigned int nStart = m_nCellLen / 2;
	// The radius of the piece is one fourth of the frame
	const unsigned int r = m_nCellLen / 4;

	dcMemory.CreateCompatibleDC(NULL);
	dcMemory.SelectObject(m_bmpChess);
	dcBmp.CreateCompatibleDC(NULL);
	dcBmp.SelectObject(m_bmpBoard);

	// Take back two pieces
	for (i = 0; i < 2; i++)
	{
		Point = m_vPiecePoint.back();
		dcMemory.BitBlt(nStart + Point.x*m_nCellLen - nStart, nStart + Point.y*m_nCellLen - nStart,
			m_nCellLen, m_nCellLen, &dcBmp, nStart + Point.x*m_nCellLen - nStart, nStart + Point.y*m_nCellLen - nStart, SRCCOPY);
		m_Board[Point.x][Point.y] = PLAYER::BLANK;
		m_vPiecePoint.pop_back();
	}

	if (m_vPiecePoint.size() > 0)
	{
		// Mark the last piece
		Point = m_vPiecePoint.back();
		// Draw a frame at this point
		dcMemory.SelectObject(new CPen(PS_SOLID, 1, COLORREF(RGB(255, 0, 0))));
		// Draw top left frame
		dcMemory.MoveTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen - r * 3 / 2);
		dcMemory.LineTo(nStart + Point.x*m_nCellLen - r, nStart + Point.y*m_nCellLen - r * 3 / 2);
		dcMemory.MoveTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen - r * 3 / 2);
		dcMemory.LineTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen - r);
		// Draw top right frame
		dcMemory.MoveTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen - r * 3 / 2);
		dcMemory.LineTo(nStart + Point.x*m_nCellLen + r, nStart + Point.y*m_nCellLen - r * 3 / 2);
		dcMemory.MoveTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen - r * 3 / 2);
		dcMemory.LineTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen - r);
		// Draw bottom left frame
		dcMemory.MoveTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen + r * 3 / 2);
		dcMemory.LineTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen + r);
		dcMemory.MoveTo(nStart + Point.x*m_nCellLen - r * 3 / 2, nStart + Point.y*m_nCellLen + r * 3 / 2);
		dcMemory.LineTo(nStart + Point.x*m_nCellLen - r, nStart + Point.y*m_nCellLen + r * 3 / 2);
		// Draw bottom right frame
		dcMemory.MoveTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen + r * 3 / 2);
		dcMemory.LineTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen + r);
		dcMemory.MoveTo(nStart + Point.x*m_nCellLen + r * 3 / 2, nStart + Point.y*m_nCellLen + r * 3 / 2);
		dcMemory.LineTo(nStart + Point.x*m_nCellLen + r, nStart + Point.y*m_nCellLen + r * 3 / 2);
	}
	return true;
}

CChess::PLAYER** CChess::GetChessArray()
{
	return m_Board;
}

CChess::PLAYER CChess::GetNowPlayer()
{
	return m_enNow;
}

POINT CChess::GetLastPoint()
{
	return m_vPiecePoint.back();
}
