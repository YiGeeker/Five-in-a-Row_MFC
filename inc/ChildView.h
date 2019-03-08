
// ChildView.h : CChildView class interface
//


#pragma once
#include "FIR.h"

// CChildView window

class CChildView : public CWnd
{
	// User defination variable
private:
	// Player vs computer or player vs player, the default is player
	// vs computer
	bool m_bP2C;
	// The current is black or white, mainly used in player vs play
	// mode
	bool m_bIsBlackNow;
	// Is game over
	bool m_bIsGameOver;
	// Class of five in row, the default board size if 13x13
	CFIR*  m_cChess;
private:
	void ComputerMove();
public:
	// A new round
	bool NewGame(unsigned int nSize, CBitmap* lpBitmap, bool bP2C, bool bPlayerFirst);
	// Take back a move
	void WithdrawPiece();
	// Game is over
	void GameOver();
// Construct
public:
	CChildView();

// Property
public:

// Manipulate
public:

// Override
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implement
public:
	virtual ~CChildView();

	// Generated message map function
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

