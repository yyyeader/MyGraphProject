
// LianXi2View.h: CLianXi2View 类的接口
//

#pragma once
#include "LianXi2Doc.h"
#include "vector"
typedef CLianXi2Doc::MyGraph MyGraph;
typedef CLianXi2Doc::MyPoint MyPoint;
typedef CLianXi2Doc::MyLine MyLine;
typedef CLianXi2Doc::MyCircle MyCircle;
typedef CLianXi2Doc::MyEllipse MyEllipse;
typedef CLianXi2Doc::MyPolygon MyPolygon;
typedef CPoint Vector;

class CLianXi2View : public CView
{
protected: // 仅从序列化创建
	CLianXi2View() noexcept;
	DECLARE_DYNCREATE(CLianXi2View)

// 特性
public:

	//保存位图
	bool saveTag = false;
	CString saveFilePath = _T("D:\\");

	//打开位图
	CString BmpName;                               //保存图像文件文件名
	CString EntName;                               //保存图像文件扩展名
	CBitmap m_bitmap;                              //创建位图对象

	CRectTracker m_RectTracker;
	bool m_IsChosen;


	CLianXi2Doc* GetDocument() const;

	void FindSelect(CPoint point);

	void ColorRecover();

	void putPixel(int x, int y, int width, COLORREF color);

	COLORREF getPixel(CDC & dc, int x, int y);

	void DefaultDrawLine(int x0, int y0, int x1, int y1, int width, COLORREF color);

	void drawline(int x0, int y0, int x1, int y1, int width, COLORREF color);

	void MidDrawline(int x0, int y0, int x1, int y1, int width, COLORREF color);

	void BresenhamLine(int x0, int y0, int x1, int y1, int width, COLORREF color);

	void CirPot(int x0, int y0, int x, int y, int width, COLORREF color);

	void DefaultDrawCircle(int x0, int y0, int r, int width, COLORREF color);

	void OnBresenhamcircle(int x0, int y0, int r, int width, COLORREF color);

	void PNCircle(int x, int y, int r, int width, COLORREF color);

	void OnMidpointcircle(int x0, int y0, int r, int width, COLORREF color);

	void EllipsePot(int x_center, int y_center, int x, int y, int width, COLORREF color, double angle);

	void DefaultDrawEllipse(int x_center, int y_center, int al, int bl, int width, COLORREF color, double angle);

	void MidDrawEllipse(int x_center, int y_center, int rx, int ry, int width, COLORREF color, double angle);

	void BresenhamEllipse(int xc, int yc, int a, int b, int width, COLORREF color, double angle);

	void Drawpolygon(std::vector<CPoint> points, int width, COLORREF color);

	void TChong(int x, int y, COLORREF color);

	void InverLine(CPoint pFrom, CPoint pTo);

	int InverCircle(CPoint pFrom, CPoint pTo);

	std::pair<int, int> InverEllipse(CPoint pFrom, CPoint pTo);

	double Calthrta(CPoint p1, CPoint p2, CPoint p3);

	CPoint GetRotatePoint(CPoint point, double thrta, int x, int y);

	void Pan(int tx, int ty);

	void Rotate(double thrta, int cx, int cy);

	void Scale(int cx, int cy, double times);

	void DuiChen(int cx, int cy);

	void DrawBezier();

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);
	// 重写以绘制该视图
	void ShowBitmap(CDC * pDC, CString BmpName);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CLianXi2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawline();
	afx_msg void OnDrawpoint();
	afx_msg void OnDrawcircle();
	afx_msg void OnDrawellipse();
	afx_msg void OnDrawpolygon();
	afx_msg void OnClear();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTianchong();
	afx_msg void OnCut();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFilesave();
	afx_msg void OnOpenfile();
	afx_msg void OnTranslatemove();
	afx_msg void OnRotatemove();
	afx_msg void OnScalemove();
	afx_msg void OnSymmetrymove();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelect();
	afx_msg void OnSetwidthandcolor();
	afx_msg void OnBezier();
};

#ifndef _DEBUG  // LianXi2View.cpp 中的调试版本
inline CLianXi2Doc* CLianXi2View::GetDocument() const
   { return reinterpret_cast<CLianXi2Doc*>(m_pDocument); }
#endif

