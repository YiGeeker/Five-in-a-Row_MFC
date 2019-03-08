#pragma once


// CNewDlg dialog

class CNewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewDlg)

public:
	CNewDlg(CWnd* pParent = NULL);   // Standard constructor
	virtual ~CNewDlg();

// Dialog data
	enum { IDD = IDD_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedP2c();
	afx_msg void OnBnClickedP2p();
	virtual BOOL OnInitDialog();
	// 0 for player on the offensive, 1 for computer on the offensive
	int m_nSelFirst;
	// Board size selection
	int m_nSelSize;
	bool m_bP2C;
};
