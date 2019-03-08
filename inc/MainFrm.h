
// MainFrm.h : CMainFrame class interface
//

#pragma once
#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

	// Custom
private:
	// Whether withdraw is allowed
	bool m_bCanWithdraw;
	// The board side length
	unsigned int m_nBoardSideLen;
	// The whole time of this round
	CTime m_cStartTime;
	// The time of this step
	CTime m_cStepTime;
	// Whether the game is over
	bool m_bIsGameOver;
private:
	bool CreateChess(unsigned int nSize, bool bP2C, bool bPlayerFirst);

// Property
public:

// Manipulate
public:

// Override
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implement
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // Control bar embedded member
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CChildView    m_wndView;

// Generated message map function
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNew();
	afx_msg void OnPaint();
	afx_msg void OnWithdraw();
	afx_msg void OnUpdateWithdraw(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnUmIswin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmStatuschange(WPARAM wParam, LPARAM lParam);
};


