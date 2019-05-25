// DrawCircleDLG.cpp: 实现文件
//

#include "stdafx.h"
#include "LianXi2.h"
#include "DrawCircleDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Lianxi2View.h"


// DrawCircleDLG 对话框

IMPLEMENT_DYNAMIC(DrawCircleDLG, CDialogEx)

DrawCircleDLG::DrawCircleDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CircleDLG, pParent)
{

}

DrawCircleDLG::~DrawCircleDLG()
{
}

void DrawCircleDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DrawCircleDLG, CDialogEx)
	ON_BN_CLICKED(MidCircle, &DrawCircleDLG::OnBnClickedMidcircle)
	ON_BN_CLICKED(BreshhamCircle, &DrawCircleDLG::OnBnClickedBreshhamcircle)
	ON_BN_CLICKED(ZFCircle, &DrawCircleDLG::OnBnClickedZfcircle)
	ON_BN_CLICKED(MDrawCircle, &DrawCircleDLG::OnBnClickedMdrawcircle)
END_MESSAGE_MAP()


extern int Mflag, Mmethod;

void DrawCircleDLG::OnBnClickedMidcircle()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 3;
	Mmethod = 1;
	EndDialog(0);
}

void DrawCircleDLG::OnBnClickedBreshhamcircle()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 3;
	Mmethod = 2;
	EndDialog(0);
}


void DrawCircleDLG::OnBnClickedZfcircle()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 3;
	Mmethod = 3;
	EndDialog(0);
}


void DrawCircleDLG::OnBnClickedMdrawcircle()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 3;
	Mmethod = 0;
	EndDialog(0);
}
