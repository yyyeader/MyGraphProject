// DrawLineDLG.cpp: 实现文件
//

#include "stdafx.h"
#include "LianXi2.h"
#include "DrawLineDLG.h"
#include "afxdialogex.h"
#include "LianXi2View.h"
#include "MainFrm.h"


// DrawLineDLG 对话框

IMPLEMENT_DYNAMIC(DrawLineDLG, CDialogEx)

DrawLineDLG::DrawLineDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(LineDLG, pParent)
{

}

DrawLineDLG::~DrawLineDLG()
{
}

void DrawLineDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DrawLineDLG, CDialogEx)
	ON_BN_CLICKED(DrawLine, &DrawLineDLG::OnBnClickedDrawline)
	ON_BN_CLICKED(MidLine, &DrawLineDLG::OnBnClickedMidline)
	ON_BN_CLICKED(BreshHam, &DrawLineDLG::OnBnClickedBreshham)
	ON_BN_CLICKED(MDrawLine, &DrawLineDLG::OnBnClickedMdrawline)
END_MESSAGE_MAP()


// DrawLineDLG 消息处理程序
extern int Mflag, Mmethod;

void DrawLineDLG::OnBnClickedDrawline()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 2;
	Mmethod = 1;
}

void DrawLineDLG::OnBnClickedMidline()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 2;
	Mmethod = 2;
	EndDialog(0);
}


void DrawLineDLG::OnBnClickedBreshham()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 2;
	Mmethod = 3;
	EndDialog(0);
}

void DrawLineDLG::OnBnClickedMdrawline()
{
	// TODO: 在此添加控件通知处理程序代码
	Mflag = 2;
	Mmethod = 0;
	EndDialog(0);
}
