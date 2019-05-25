// DrawEllipseDLG.cpp: 实现文件
//

#include "stdafx.h"
#include "LianXi2.h"
#include "DrawEllipseDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include  "LianXi2View.h"


// DrawEllipseDLG 对话框

IMPLEMENT_DYNAMIC(DrawEllipseDLG, CDialogEx)

DrawEllipseDLG::DrawEllipseDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(EllipseDLG, pParent)
{

}

DrawEllipseDLG::~DrawEllipseDLG()
{
}

void DrawEllipseDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DrawEllipseDLG, CDialogEx)

	ON_BN_CLICKED(DrawEllipse, &DrawEllipseDLG::OnBnClickedDrawellipse)
	ON_BN_CLICKED(BreshhamEllipse, &DrawEllipseDLG::OnBnClickedBreshhamellipse)
	ON_BN_CLICKED(MDrawEllipse, &DrawEllipseDLG::OnBnClickedMdrawellipse)
END_MESSAGE_MAP()


// DrawEllipseDLG 消息处理程序

extern int Mflag, Mmethod;

void DrawEllipseDLG::OnBnClickedDrawellipse()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 4;
	Mmethod = 1;
	EndDialog(0);
}


void DrawEllipseDLG::OnBnClickedBreshhamellipse()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 4;
	Mmethod = 2;
	EndDialog(0);
}


void DrawEllipseDLG::OnBnClickedMdrawellipse()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 4;
	Mmethod = 2;
	EndDialog(0);
}
