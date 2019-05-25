// DCDLG.cpp: 实现文件
//

#include "stdafx.h"
#include "LianXi2.h"
#include "DCDLG.h"
#include "afxdialogex.h"


// DCDLG 对话框

IMPLEMENT_DYNAMIC(DCDLG, CDialogEx)

DCDLG::DCDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DuiChenDLG, pParent)
{

}

DCDLG::~DCDLG()
{
}

void DCDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DCDLG, CDialogEx)
END_MESSAGE_MAP()


// DCDLG 消息处理程序
