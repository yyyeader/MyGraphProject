
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
#include "iostream"
using namespace std;

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
	fa->DrawBezier(points, showPoint, color, width);
}



//矩阵类函数
CLianXi2Doc::Matrix::Matrix(int m, int n)
{
	if (m < 0 || n < 0)
	{
		cout << "矩阵大小不能为负\n";
		return;
	}
	rowNum = m;
	colNum = n;
	item = new double[m*n];
	for (int i = 0; i < m*n; i++)
	{
		item[i] = 0;
	}
}

//也可用二维数组初始化
CLianXi2Doc::Matrix::Matrix(double* items, int m, int n)
{
	rowNum = m;
	colNum = n;
	item = new double[m*n];
	for (int i = 0; i < colNum*rowNum; i++)
	{
		item[i] = items[i];
	}
}
CLianXi2Doc::Matrix::Matrix(int n)
{
	rowNum = colNum = n;
	item = new double[n*n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j)
				set(i, j, 1.0);
			else
				set(i, j, 0);
		}
	}
}
CLianXi2Doc::Matrix::Matrix(const Matrix &M)
{
	colNum = M.colNum;
	rowNum = M.rowNum;
	//这里不能对指针直接赋值,复制对求逆、转置等操作会影响原矩阵
	item = new double[colNum*rowNum];
	for (int i = 0; i < colNum*rowNum; i++)
	{
		item[i] = M.item[i];
	}
}
CLianXi2Doc::Matrix& CLianXi2Doc::Matrix::operator=(const Matrix & M)
{
	colNum = M.colNum;
	rowNum = M.rowNum;
	if (item != nullptr) delete[] item;
	item = new double[colNum*rowNum];
	for (int i = 0; i < colNum*rowNum; i++)
	{
		item[i] = M.item[i];
	}
	return *this;
}

CLianXi2Doc::Matrix::~Matrix()
{
	delete[] item;
}
double CLianXi2Doc::Matrix::get(int i, int j) const
{
	return item[i*colNum + j];
}
void CLianXi2Doc::Matrix::set(int i, int j, double value)
{
	item[i*colNum + j] = value;
}
void CLianXi2Doc::Matrix::RowSwap(int i, int j, double multiply)
{
	if (j == -1)
	{
		for (int k = 0; k < colNum; k++)
		{
			set(i, k, multiply*get(i, k));
		}
	}
	else
	{
		for (int k = 0; k < colNum; k++)
		{
			set(j, k, multiply*get(i, k) + get(j, k));
		}
	}
}
void CLianXi2Doc::Matrix::RowSwap(int i, int j)
{
	Matrix _copy = *this;
	for (int k = 0; k < colNum; k++)
	{
		double swap = _copy.get(j, k);
		set(j, k, _copy.get(i, k));
		set(i, k, swap);
	}
}
CLianXi2Doc::Matrix CLianXi2Doc::Matrix::Trans() const
{
	Matrix _copy = *this;
	_copy.rowNum = this->colNum;
	_copy.colNum = this->rowNum;
	for (int i = 0; i < _copy.rowNum; i++)
	{
		for (int j = 0; j < _copy.colNum; j++)
		{
			_copy.set(i, j, get(j, i));
		}
	}
	return _copy;
}
int CLianXi2Doc::Matrix::getRowNum() const
{
	return rowNum;
}
int CLianXi2Doc::Matrix::getColNum() const
{
	return colNum;
}

CLianXi2Doc::Matrix CLianXi2Doc::Matrix::operator +(const Matrix &m)
{
	if (m.colNum != colNum || m.rowNum != rowNum) return *this;
	Matrix _copy = *this;
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			_copy.set(i, j, get(i, j) + m.get(i, j));
		}
	}
	return _copy;
}
CLianXi2Doc::Matrix CLianXi2Doc::Matrix::operator -(const Matrix &m)
{
	if (m.colNum != colNum || m.rowNum != rowNum) return *this;
	Matrix _copy = *this;
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			_copy.set(i, j, get(i, j) - m.get(i, j));
		}
	}
	return _copy;
}
CLianXi2Doc::Matrix CLianXi2Doc::Matrix::operator *(const double f)
{
	Matrix _copy = *this;
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			_copy.set(i, j, get(i, j)*f);
		}
	}
	return _copy;
}
CLianXi2Doc::Matrix CLianXi2Doc::Matrix::operator *(const Matrix &m)
{
	if (colNum != m.rowNum)
	{
		cout << "无法相乘！";
		return *this;
	}
	Matrix _copy(rowNum, m.getColNum());
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < m.colNum; j++)
		{
			double sum = 0;
			for (int k = 0; k < m.rowNum; k++)
			{
				sum += get(i, k)*m.get(k, j);
			}
			_copy.set(i, j, sum);
		}
	}
	return _copy;
}
CLianXi2Doc::Matrix CLianXi2Doc::Matrix::Inverse()
{
	Matrix _copy = *this;
	//变换结果
	Matrix result(colNum);
	if (colNum != rowNum)
	{
		cout << "矩阵不可逆！" << endl;
		return *this;
	}
	for (int i = 0; i < rowNum; i++)
	{
		int MaxRow = i;
		//首先找到第i列的绝对值最大的数，并将该行和第i行互换
		double max = abs(_copy.get(i, i));
		for (int j = i; j < colNum; j++)
		{
			if (abs(_copy.get(j, i)) > max)
			{
				max = abs(_copy.get(j, i));
				MaxRow = j;
			}
		}
		//交换j，i两行
		if (MaxRow != i)
		{
			result.RowSwap(i, MaxRow);
			_copy.RowSwap(i, MaxRow);
		}
		//将第i行做初等行变换，将第一个非0元素化为1
		double r = 1.0 / _copy.get(i, i);
		_copy.RowSwap(i, -1, r);
		result.RowSwap(i, -1, r);
		//消元
		for (int j = 0; j < rowNum; j++)
		{
			if (j == i) continue;
			r = -_copy.get(j, i);
			_copy.RowSwap(i, j, r);
			result.RowSwap(i, j, r);
		}
	}
	//result.FlowOver();
	return result;
}

void CLianXi2Doc::Matrix::FlowOver()
{
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			if (abs(get(i, j)) <= 1e-8) set(i, j, 0);
		}
	}
}
