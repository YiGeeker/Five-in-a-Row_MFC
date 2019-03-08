
// MainFrm.cpp : CMainFrame class implement
//

#include "stdafx.h"
#include "FiveInRow.h"

#include "MainFrm.h"
#include "NewDlg.h"
#include "UserMessage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(IDM_NEW, &CMainFrame::OnNew)
	ON_WM_PAINT()
	ON_COMMAND(IDM_WITHDRAW, &CMainFrame::OnWithdraw)
	ON_UPDATE_COMMAND_UI(IDM_WITHDRAW, &CMainFrame::OnUpdateWithdraw)
	ON_WM_TIMER()
	ON_MESSAGE(UM_ISWIN, &CMainFrame::OnUmIswin)
	ON_MESSAGE(UM_STATUSCHANGE, &CMainFrame::OnUmStatuschange)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	//ID_SEPARATOR,           // Status line indicator
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
	ID_INDICATOR_NOW,//Current player
	ID_INDICATOR_TIME,//Time for this step
	ID_INDICATOR_ALLTIME,//Time for this round
};

// CMainFrame construcor/destructor

CMainFrame::CMainFrame()
{
	// TODO: Add here member initializtion code
	m_bIsGameOver = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create a view to occupy frame workspace
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Create view window unsuccessfully\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR))
	{
		TRACE0("Create toolbar unsuccessfully\n");
		return -1;      // Create unsuccessfully
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Creat status bar unsuccessfully\n");
		return -1;      // Create unsuccessfully
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Remove these three lines if dock toolbar is no need
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	// Create chess board
	CreateChess(13, true, true);
	// Set time
	SetTimer(1, 1000, NULL);
	m_cStartTime = CTime::GetCurrentTime();
	m_cStepTime = m_cStartTime;

	m_bCanWithdraw = true;


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
	// TODO: change window class or style by changing CREATESTRUCT cs
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_SYSMENU;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0, 0, 0, AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnosis

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

bool CMainFrame::CreateChess(unsigned int nSize, bool bP2C, bool bPlayerFirst)
{
	unsigned int i, j;
	CDC dcBmp, dcMemory;
	CBitmap bmpWood, bmpMemory;
	BITMAP bmpInfo;

	// Load wood texture picture
	bmpWood.LoadBitmap(IDB_WOOD);
	bmpWood.GetBitmap(&bmpInfo);

	m_nBoardSideLen = bmpInfo.bmWidth*nSize;
	bmpMemory.CreateBitmap(m_nBoardSideLen, m_nBoardSideLen, bmpInfo.bmPlanes, bmpInfo.bmBitsPixel, NULL);

	dcMemory.CreateCompatibleDC(NULL);
	dcMemory.SelectObject(&bmpMemory);

	// Get wood background
	dcBmp.CreateCompatibleDC(&dcMemory);
	dcBmp.SelectObject(&bmpWood);

	for (i = 0; i < nSize; i++)
	{
		for (j = 0; j < nSize; j++)
		{
			dcMemory.BitBlt(bmpInfo.bmWidth*i, bmpInfo.bmHeight*j, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcBmp, 0, 0, SRCCOPY);
		}
	}
	if (!m_wndView.NewGame(nSize, dcMemory.GetCurrentBitmap(), bP2C, bPlayerFirst)) return false;
	return true;
}

// CMainFrame message process program

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// Move forward the focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// Let view try this command for the first time
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise execute the default process
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnPaint()
{
	unsigned int i,nStatusID,nStatusStyle;
	int nWidth;
	// Get the location of the main window and the child window
	RECT stRect, stChildRect;

	CPaintDC dc(this); // device context for painting
	// TODO: Add here message process code
	// Donot call CFrameWnd::OnPaint() for painting message

	// Revise the size of main window to the board size, including the
	// length difference of the main window and the child window
	m_wndView.GetWindowRect(&stChildRect);
	GetWindowRect(&stRect);
	MoveWindow(stRect.left, stRect.top, (stChildRect.left - stRect.left) + (stRect.right - stChildRect.right) + m_nBoardSideLen,
		(stChildRect.top - stRect.top) + (stRect.bottom - stChildRect.bottom) + m_nBoardSideLen, true);

	// Revise the status bar size
	for (i = 0; i < 3; i++)
	{
		m_wndStatusBar.GetPaneInfo(i, nStatusID, nStatusStyle, nWidth);
		nWidth = m_nBoardSideLen / 3;
		m_wndStatusBar.SetPaneInfo(i, nStatusID, nStatusStyle, nWidth);
	}
}

void CMainFrame::OnNew()
{
	unsigned int nSize[4] = { 13,15,17,19 };
	bool bPlayerFirst[2] = { true,false };
	CNewDlg cNewDlg;

	// TODO: Add here command process code
	if (cNewDlg.DoModal() == IDOK)
	{		
		CreateChess(nSize[cNewDlg.m_nSelSize], cNewDlg.m_bP2C, bPlayerFirst[cNewDlg.m_nSelFirst]);
		m_cStartTime = CTime::GetCurrentTime();
		m_cStepTime = m_cStartTime;
		m_bCanWithdraw = true;

		this->Invalidate();
	}
}

void CMainFrame::OnWithdraw()
{
	// TODO: Add here command process code
	m_wndView.WithdrawPiece();
}

void CMainFrame::OnUpdateWithdraw(CCmdUI *pCmdUI)
{
	// TODO: Add here command for update user interface process code
	pCmdUI->Enable(m_bCanWithdraw);
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	CTime cNowTime;
	CString cStrTime;
	// TODO: Add here message process code and/or call default funciton

	if (!m_bIsGameOver)
	{
		cNowTime = CTime::GetCurrentTime();

		cStrTime = "Step: ";
		cStrTime += (cNowTime - m_cStepTime).Format("%H:%M:%S");
		m_wndStatusBar.SetPaneText(1, cStrTime, true);


		cStrTime = "Round: ";
		cStrTime += (cNowTime - m_cStartTime).Format("%H:%M:%S");
		m_wndStatusBar.SetPaneText(2, cStrTime, true);
	}

	CFrameWnd::OnTimer(nIDEvent);
}

afx_msg LRESULT CMainFrame::OnUmIswin(WPARAM wParam, LPARAM lParam)
{
	// wParam = true for player vs computer, or player vs computer
	if (wParam)
	{
		// lParam = 0 denotes player wins and lParma = 1 denotes
		// computer wins in player vs computer mode
		if (lParam == 0)
		{
			AfxMessageBox("Player wins!");
		}
		else if (lParam == 1)
		{
			AfxMessageBox("Computer wins!");
		}
	}
	else
	{
		// lParam = true denotes the blace wins and lParma = false
		// denotes the white wins in player vs player mode
		if (lParam)
		{
			AfxMessageBox("The black wins!");
		}
		else
		{
			AfxMessageBox("The white wins!");
		}
	}
	m_bCanWithdraw = false;
	m_bIsGameOver = true;
	m_wndView.GameOver();

	return 0;
}

afx_msg LRESULT CMainFrame::OnUmStatuschange(WPARAM wParam, LPARAM lParam)
{
	m_cStepTime = CTime::GetCurrentTime();

	// wParam = true for player vs computer, or player vs computer
	if (wParam)
	{
		// lParam = 0 denotes the current player is player, and lParam
		// = 1 denotes the current player is computer in player vs
		// computer
		if (lParam == 0)
		{
			m_wndStatusBar.SetPaneText(0, "Current: Player", true);
		}
		else if (lParam == 1)
		{
			m_wndStatusBar.SetPaneText(0, "Current: Computer", true);
		}
	}
	else
	{
		// lParam = true denotes the current player is the black, and
		// lParam = false denotes the current player is the white in
		// player vs player
		if (lParam)
		{
			m_wndStatusBar.SetPaneText(0, "Current: Black", true);
		}
		else
		{
			m_wndStatusBar.SetPaneText(0, "Current: White", true);
		}
	}


	return 0;
}
