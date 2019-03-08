
// ChildView.cpp : CChildView class implementation
//

#include "stdafx.h"
#include "FiveInRow.h"
#include "ChildView.h"
#include "UserMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_bP2C = true;
	m_bIsBlackNow = true;
	m_bIsGameOver = false;
	m_cChess = NULL;
}

CChildView::~CChildView()
{
}

bool CChildView::NewGame(unsigned int nSize, CBitmap* lpBitmap, bool bP2C, bool bPlayerFirst)
{
	if (m_cChess != NULL) delete m_cChess;
	m_cChess = new CFIR(nSize);
	m_cChess->SetBackground(lpBitmap);
	m_bP2C = bP2C;
	m_bIsGameOver = false;
	if (!m_cChess->Create()) return false;
	if (bP2C)
	{
		if (!bPlayerFirst)
		{
			m_cChess->SetComputerFirst();
			GetParent()->SendMessage(UM_STATUSCHANGE, true, 1);
			ComputerMove();
		}
	}
	else
	{
		GetParent()->SendMessage(UM_STATUSCHANGE, false, m_bIsBlackNow);
	}
	
	this->Invalidate();
	return true;
}

void CChildView::ComputerMove()
{
	m_cChess->PlacePiece(m_cChess->GetComputerPosition());
	m_cChess->Show(GetDC(), 0, 0);
	if (m_cChess->IsWin()) GetParent()->SendMessage(UM_ISWIN, true, 1);
	else
	{
		m_cChess->ChangePlayer();
		GetParent()->SendMessage(UM_STATUSCHANGE, true, 0);
	}
}

void CChildView::WithdrawPiece()
{
	// TODO: Add here command process code
	m_cChess->WithdrawPiece();

	m_cChess->Show(GetDC(), 0, 0);
}

void CChildView::GameOver()
{
	m_bIsGameOver = true;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CChildView message process program

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add here dedicated code and call basic class

	// Shield mouse message when game is over
	if (m_bIsGameOver)
	{
		if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONUP) return true;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // Used for drawing device context
	BITMAP bmpInfo;

	// TODO: Add here message process code
	// Show chess board
	m_cChess->Show(&dc, 0, 0);
	m_cChess->GetBitmap(&bmpInfo);
	
	// Donot call CWnd::OnPaint() for painting message
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add here dedicated code and call basic class

	// Show piece but donot place it when press the left mouse
	SetCapture();
	m_cChess->BeginHold();
	m_cChess->HoldPiece(point.x, point.y);

	m_cChess->Show(GetDC(), 0, 0);

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add here dedicated code and call basic class

	if (m_cChess->HoldPiece(point.x, point.y))
	{
		m_cChess->Show(GetDC(), 0, 0);
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	POINT Point;
	// TODO: Add here dedicated code and call basic class

	// Check if the piece is placed on the point
	if (m_cChess->GetPiont(point.x, point.y, Point))
	{
		if (m_cChess->IsForbidden(Point))
		{
			m_cChess->ReleasePiece();
			AfxMessageBox("The black is forbidden");
		}
		else
		{
			m_cChess->PlacePiece(Point);
			if (m_cChess->IsWin())
			{
				if (m_bP2C)
				{
					GetParent()->SendMessage(UM_ISWIN, true, 0);
				}
				else
				{
					GetParent()->SendMessage(UM_ISWIN, false, m_bIsBlackNow);
				}
			}
			else
			{
				m_cChess->ChangePlayer();
				if (m_bP2C)
				{
					GetParent()->SendMessage(UM_STATUSCHANGE, true, 1);
					ComputerMove();
				}
				else
				{
					m_bIsBlackNow = !m_bIsBlackNow;
					GetParent()->SendMessage(UM_STATUSCHANGE, false, m_bIsBlackNow);
				}
			}
		}
	}
	else
	{
		m_cChess->ReleasePiece();
	}
	m_cChess->Show(GetDC(), 0, 0);

	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}
