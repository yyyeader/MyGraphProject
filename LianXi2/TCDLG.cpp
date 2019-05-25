// TCDLG.cpp: 实现文件
//

#include "stdafx.h"
#include "LianXi2.h"
#include "TCDLG.h"
#include "afxdialogex.h"


// TCDLG 对话框

IMPLEMENT_DYNAMIC(TCDLG, CDialogEx)

TCDLG::TCDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(TianChongDLG, pParent)
{

}

TCDLG::~TCDLG()
{
}

void TCDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, TianChongColor);
}


BEGIN_MESSAGE_MAP(TCDLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &TCDLG::OnBnClickedOk)
END_MESSAGE_MAP()


// TCDLG 消息处理程序
void TCDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	extern COLORREF Mcolor;
	extern int Mflag;
	Mflag = 6;
	Mcolor = TianChongColor.GetColor();
	CDialogEx::OnOK();
}
