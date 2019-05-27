
// LianXi2Doc.cpp: CLianXi2Doc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LianXi2.h"
#endif
#include "LianXi2View.h"
#include "LianXi2Doc.h"
#include "MainFrm.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLianXi2Doc

IMPLEMENT_DYNCREATE(CLianXi2Doc, CDocument)

BEGIN_MESSAGE_MAP(CLianXi2Doc, CDocument)
END_MESSAGE_MAP()


// CLianXi2Doc 构造/析构

CLianXi2Doc::CLianXi2Doc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CLianXi2Doc::~CLianXi2Doc()
{
	auto st = graphs.begin();
	auto en = graphs.end();
	while (st != en) {
		delete (*st);
		st++;
	}
	graphs.clear();
}

BOOL CLianXi2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}


// CLianXi2Doc 序列化

void CLianXi2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}




#ifdef SHARED_HANDLERS

// 缩略图的支持
void CLianXi2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CLianXi2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CLianXi2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CLianXi2Doc 诊断

#ifdef _DEBUG
void CLianXi2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLianXi2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLianXi2Doc 命令


//图形相关类及结构体函数定义

//画点
void CLianXi2Doc::MyPoint::draw() {
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CLianXi2View *fa = (CLianXi2View*)pFrame->GetActiveView();
	fa->DefaultDrawLine(center.x, center.y - r, center.x, center.y + r, width, color);
	fa->DefaultDrawLine(center.x - r, center.y, center.x + r, center.y, width, color);
}


//画线
void CLianXi2Doc::MyLine::draw() {
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CLianXi2View *fa = (CLianXi2View*)pFrame->GetActiveView();
	if (method == 0) {
		fa->DefaultDrawLine(sp.x, sp.y, ep.x, ep.y, width, color);
	}
	if (method == 1) {
		//TRACE("%d %d %d %d %d %d\n",sp.x,sp.y,ep.x,ep.y,width,color);
		fa->DDADrawline(sp.x,sp.y,ep.x,ep.y,width,color);
	}
	if (method == 2) {
		fa->MidDrawline(sp.x, sp.y, ep.x, ep.y, width, color);
	}
	if (method == 3) {
		fa->BresenhamLine(sp.x, sp.y, ep.x, ep.y, width, color);
	}
}

//画圆
void CLianXi2Doc::MyCircle::draw() {
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CLianXi2View *fa = (CLianXi2View*)pFrame->GetActiveView();
	if (method == 0) {
		fa->DefaultDrawCircle(center.x, center.y, r, width, color);
	}
	if (method == 1) {
		fa->OnMidpointcircle(center.x,center.y,r,width,color);
	}
	if (method == 2) {
		fa->OnBresenhamcircle(center.x, center.y, r, width, color);
	}
	if (method == 3) {
		fa->PNCircle(center.x, center.y, r, width, color);
	}
}

//画椭圆
void CLianXi2Doc::MyEllipse::draw() {
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CLianXi2View *fa = (CLianXi2View*)pFrame->GetActiveView();
	if (method == 0) {
		fa->DefaultDrawEllipse(center.x, center.y, al, bl, width, color, angle);
	}
	if (method == 1) {
		fa->MidDrawEllipse(center.x, center.y, al, bl, width, color, angle);
	}
	if (method == 2) {
		fa->BresenhamEllipse(center.x, center.y, al, bl, width, color, angle);
	}
}

//画多边形
void CLianXi2Doc::MyPolygon::draw() {
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CLianXi2View *fa = (CLianXi2View*)pFrame->GetActiveView();
	fa->Drawpolygon(points, width, color);
}


//画贝塞尔曲线
void CLianXi2Doc::MyBesizer::draw() {
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CLianXi2View *fa = (CLianXi2View*)pFrame->GetActiveView();
	fa->DrawBezier(points,color,width);
}