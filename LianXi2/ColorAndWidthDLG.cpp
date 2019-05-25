// ColorAndWidthDLG.cpp: 实现文件
//

#include "stdafx.h"
#include "LianXi2.h"
#include "ColorAndWidthDLG.h"
#include "afxdialogex.h"


// ColorAndWidthDLG 对话框

IMPLEMENT_DYNAMIC(ColorAndWidthDLG, CDialogEx)

ColorAndWidthDLG::ColorAndWidthDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ColorAndWidth, pParent)
{

}

ColorAndWidthDLG::~ColorAndWidthDLG()
{
}

void ColorAndWidthDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, GColor);
}


BEGIN_MESSAGE_MAP(ColorAndWidthDLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &ColorAndWidthDLG::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &ColorAndWidthDLG::OnBnClickedMfccolorbutton1)
END_MESSAGE_MAP()


// ColorAndWidthDLG 消息处理程序
extern int Mwidth;
extern COLORREF Mcolor;

void ColorAndWidthDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	Mwidth = GetDlgItemInt(GWidth);
	Mcolor = GColor.GetColor();
	CDialogEx::OnOK();
}


void ColorAndWidthDLG::OnBnClickedMfccolorbutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	COLORREF color = GColor.GetColor();
	GColor.SetColor(color);
}
