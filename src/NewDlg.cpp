// NewDlg.cpp : Implemention
//

#include "stdafx.h"
#include "FiveInRow.h"
#include "NewDlg.h"
#include "afxdialogex.h"


// CNewDlg dialog

IMPLEMENT_DYNAMIC(CNewDlg, CDialogEx)

CNewDlg::CNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewDlg::IDD, pParent)
{

	m_nSelFirst = 0;
	m_nSelSize = 0;
	m_bP2C = true;
}

CNewDlg::~CNewDlg()
{
}

void CNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_FIRST, m_nSelFirst);
	DDV_MinMaxInt(pDX, m_nSelFirst, 0, 1);
	DDX_CBIndex(pDX, IDC_BOARDSIZE, m_nSelSize);
	DDV_MinMaxInt(pDX, m_nSelSize, 0, 3);
}


BEGIN_MESSAGE_MAP(CNewDlg, CDialogEx)
	ON_BN_CLICKED(IDC_P2C, &CNewDlg::OnBnClickedP2c)
	ON_BN_CLICKED(IDC_P2P, &CNewDlg::OnBnClickedP2p)
END_MESSAGE_MAP()


// CNewDlg message process program


void CNewDlg::OnBnClickedP2c()
{
	// TODO: Add here control information process code
	// Be able to select first hand when choose player vs computer mode
	if (((CButton*)GetDlgItem(IDC_P2C))->GetCheck() == 1)
	{
		m_bP2C = true;
		((CComboBoxEx*)GetDlgItem(IDC_FIRST))->EnableWindow(true);
	}
}


void CNewDlg::OnBnClickedP2p()
{
	// TODO: Add here control information process code
	// Not be able to select first hand when choose player vs player mode
	if (((CButton*)GetDlgItem(IDC_P2P))->GetCheck() == 1)
	{
		m_bP2C = false;
		((CComboBoxEx*)GetDlgItem(IDC_FIRST))->EnableWindow(false);
	}
}


BOOL CNewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add here extra initialization

	// The default mode is player vs computer
	((CButton*)GetDlgItem(IDC_P2C))->SetCheck(BST_CHECKED);
	// The default is player first hand on default mode
	((CComboBoxEx*)GetDlgItem(IDC_FIRST))->SetCurSel(0);
	// The default board size if 13x13
	((CComboBoxEx*)GetDlgItem(IDC_BOARDSIZE))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// Exception: OCX property page should return FALSE
}
