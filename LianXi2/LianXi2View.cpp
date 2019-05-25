
// LianXi2View.cpp: CLianXi2View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LianXi2.h"
#endif

#include "LianXi2Doc.h"
#include "LianXi2View.h"
#include "DrawLineDLG.h"
#include "DrawCircleDLG.h"
#include "DrawEllipseDLG.h"
#include "ColorAndWidthDLG.h"
#include "TCDLG.h"
#include "MainFrm.h"
#include "queue"
#include "map"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLianXi2View

IMPLEMENT_DYNCREATE(CLianXi2View, CView)

BEGIN_MESSAGE_MAP(CLianXi2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLianXi2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
//	ON_WM_RBUTTONUP()
	ON_COMMAND(DrawLine, &CLianXi2View::OnDrawline)
	ON_COMMAND(DrawPoint, &CLianXi2View::OnDrawpoint)
	ON_COMMAND(DrawCircle, &CLianXi2View::OnDrawcircle)
	ON_COMMAND(DrawEllipse, &CLianXi2View::OnDrawellipse)
	ON_COMMAND(DrawPolygon, &CLianXi2View::OnDrawpolygon)
	ON_COMMAND(Clear, &CLianXi2View::OnClear)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(TianChong, &CLianXi2View::OnTianchong)
	ON_COMMAND(Cut, &CLianXi2View::OnCut)
	ON_WM_RBUTTONUP()
	ON_COMMAND(FileSave, &CLianXi2View::OnFilesave)
	ON_COMMAND(OpenFile, &CLianXi2View::OnOpenfile)
	ON_COMMAND(TranslateMove, &CLianXi2View::OnTranslatemove)
	ON_COMMAND(RotateMove, &CLianXi2View::OnRotatemove)
	ON_COMMAND(ScaleMove, &CLianXi2View::OnScalemove)
	ON_COMMAND(SymmetryMove, &CLianXi2View::OnSymmetrymove)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(Select, &CLianXi2View::OnSelect)
	ON_COMMAND(SetWidthAndColor, &CLianXi2View::OnSetwidthandcolor)
	ON_COMMAND(Bezier, &CLianXi2View::OnBezier)
END_MESSAGE_MAP()

// CLianXi2View 构造/析构

CLianXi2View::CLianXi2View() noexcept
{
	// TODO: 在此处添加构造代码
	saveTag = FALSE;
	m_RectTracker.m_rect.SetRect(0, 0, 0, 0);
	m_RectTracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside;
	m_IsChosen = FALSE;
}

//常量
const double eps = 1e-8;
const double pi = 3.1415926;

//1:点  2:线  3:圆  4:椭圆  5;多边形  6：填充  7:裁剪  8:裁剪后拖动  9:标记图形变化  10:画线确定起点
//11:选中 12:平移变换 13:旋转变换 14:缩放变换 15:对称变换 16:画圆确定起点 17:画椭圆确定起点 18:画多边形确定起点
//19:平移变换确定起点 20:旋转变换确定起点 21:缩放变换确定起点 22;贝塞尔曲线 23:画贝塞尔曲线确定起点
int Mflag=0;				//用于判断当前状态
int Mwidth=1;				//线宽
int Mmethod = 0;			//画图方案
COLORREF Mcolor;			//颜色
MyGraph *Mypt = nullptr;	//鼠标选中图形对象指针
COLORREF oldColor;			//鼠标选中图形对象的颜色
int MyType = 0;				//鼠标选中图形对象的类型
CPoint MLst0, MLst1,MPst;
vector<CPoint>MPoints;		//记录画多边形时选中的顶点
vector<CPoint>BPoints;		//贝塞尔曲线控制顶点




CLianXi2View::~CLianXi2View()
{
}

BOOL CLianXi2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}
// CLianXi2View 绘图

void CLianXi2View::OnDraw(CDC* pDC)
{
	CLianXi2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (EntName.Compare(_T("bmp")) == 0) {
		OnClear();
		ShowBitmap(pDC, BmpName);               //显示图片
		EntName = "";
	}
	else {
		auto st = pDoc->graphs.begin();
		auto en = pDoc->graphs.end();
		while (st != en) {
			(*st)->draw();
			st++;
		}
	}
	// TODO: 在此处为本机数据添加绘制代码
}
// CLianXi2View 打印

void CLianXi2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLianXi2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CLianXi2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CLianXi2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CLianXi2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CLianXi2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CLianXi2View 诊断

#ifdef _DEBUG
void CLianXi2View::AssertValid() const
{
	CView::AssertValid();
}

void CLianXi2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLianXi2Doc* CLianXi2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLianXi2Doc)));
	return (CLianXi2Doc*)m_pDocument;
}
#endif //_DEBUG

int dcmp(double x) { if (fabs(x) < eps)return 0; return (x > 0) ? 1 : -1; }
Vector operator + (Vector A, Vector B) { return Vector(A.x + B.x, A.y + B.y); }
Vector operator - (Vector A, Vector B) { return Vector(A.x - B.x, A.y - B.y); }
Vector operator * (Vector A, double B) { return Vector(A.x*B, A.y*B); }
Vector operator / (Vector A, double B) { return Vector(A.x / B, A.y / B); }
bool operator == (const Vector A, const Vector B) {return dcmp(A.x - B.x) == 0 && dcmp(A.y - B.y) == 0;}

//求阶乘
long long fac(long long n) {
	long long sum = 1;
	for (int i = 1; i <= n; i++) {
		sum*=i;
	}
	return sum;
}

//计算向量点积
double Dot(Vector A, Vector B) { return 1.0*A.x*B.x + A.y*B.y; } 

//计算向量长度
double Length(Vector A) { return sqrt(Dot(A, A)); }

//计算向量叉积
double Cross(Vector A, Vector B) { return 1.0*A.x*B.y - B.x*A.y; }

//计算两点间距离
double dist(CPoint A, CPoint B) { return sqrt(pow(1.0*A.x - B.x, 2) + pow(1.0*A.y - B.y, 2)); }

//根据线段两端点计算点到线段距离
double DistanceToSegment(CPoint P, CPoint A, CPoint B) {
	if (A == B)return Length(P - A);
	CPoint v1 = B - A, v2 = P - A, v3 = P - B;
	if (dcmp(Dot(v1, v2)) < 0)return Length(v2);           //第二类第一小类
	else if (dcmp(Dot(v1, v3)) > 0)return Length(v3);      //第二类第二小类
	else return fabs(Cross(v1, v2)) / Length(v1);
}

//判断线段是否选中
bool CheckSelect(MyLine line,int x,int y) {
	
	CPoint P(x, y), A=line.getSp(), B=line.getEp();
	return DistanceToSegment(P, A, B) <= min(5,Mwidth);
}

//判断圆是否选中
bool CheckSelect(MyCircle circle, int x, int y) {
	CPoint P(x, y), A=circle.getCenter();
	return abs(dist(P, A) - circle.getR()) <= min(5, Mwidth);
}

//判断椭圆是否选中
bool CheckSelect(MyEllipse ellipse, int x, int y) {
	CPoint P(x, y), A = ellipse.getCenter();
	return abs(dist(P, A) -ellipse.getAl()) <= min(5, Mwidth);
}

//判断多边形是否选中
bool CheckSelect(MyPolygon poly, int x, int y) {
	CPoint P(x, y);
	vector<CPoint> points = poly.getPoints();
	int size = points.size();
	for (int i = 0; i < size; i++) {
		CPoint A = points[i];
		CPoint B = points[(i+1)%size];
		if(DistanceToSegment(P, A, B) <= min(5, Mwidth)) return true;
	}
	return false;
}

//确定鼠标点击的点是否选中对象
void CLianXi2View::FindSelect(CPoint point) {
	CLianXi2Doc* pDoc = GetDocument();
	auto st = pDoc->graphs.begin();
	auto en = pDoc->graphs.end();
	int type = 0;
	while (st != en) {
		type = (*st)->getType();
		if (type == 1) {
			auto pt = dynamic_cast<MyLine *>(*st);
			if (pt&&CheckSelect(*pt, point.x, point.y)) break;
		}
		if (type == 2) {
			auto pt = dynamic_cast<MyCircle *>(*st);
			if (pt&&CheckSelect(*pt, point.x, point.y)) break;
		}
		if (type == 3) {
			auto pt = dynamic_cast<MyEllipse *>(*st);
			if (pt&&CheckSelect(*pt, point.x, point.y)) break;
		}
		if (type == 4) {
			auto pt = dynamic_cast<MyPolygon *>(*st);
			if (pt&&CheckSelect(*pt, point.x, point.y)) break;
		}
		type = 0;
		st++;
	}
	if (Mypt) {
		ColorRecover();
		Mypt = nullptr;
	}
	if (type != 0) {
		oldColor = (*st)->getColor();
		(*st)->setColor(RGB(255, 0, 0));
		(*st)->draw();
		MyType = type;
		Mypt = (*st);
	}
	/*
	else if (Mflag != 11) {
		Mflag=0;
	}
	*/
}

//恢复被选中图形原来的颜色
void CLianXi2View::ColorRecover() {
	if (Mypt == nullptr) return;
	Mypt->setColor(oldColor);
	Mypt->draw();
}

//获取被选中图形的质心
CPoint GetCPos() {
	int cx = 0, cy = 0;
	if (Mypt) {
		if (MyType == 1) {
			MyLine *pt = dynamic_cast<MyLine *>(Mypt);
			CPoint sp = pt->getSp(), ep = pt->getEp();
			cx = (sp.x + ep.x) / 2;
			cy = (sp.y + ep.y) / 2;
		}
		if (MyType == 2) {
			MyCircle *pt = dynamic_cast<MyCircle *>(Mypt);
			cx = pt->getCenter().x;
			cy = pt->getCenter().y;
		}
		if (MyType == 3) {
			MyEllipse *pt = dynamic_cast<MyEllipse *>(Mypt);
			cx = pt->getCenter().x;
			cy = pt->getCenter().y;
		}
		if (MyType == 4) {
			MyPolygon *pt = dynamic_cast<MyPolygon *>(Mypt);
			vector<CPoint> points = pt->getPoints();
			for (int i = 0; i < points.size(); i++) {
				cx += points[i].x;
				cy += points[i].y;
			}
			cx /= points.size();
			cy /= points.size();
		}
	}
	return CPoint(cx, cy);
}

//获取旋转后的点
CPoint CLianXi2View::GetRotatePoint(CPoint point, double thrta, int x, int y) {
	CPoint t = point, t1 = point;
	thrta = pi / 180.0*thrta;
	t.x = x + (t1.x - x)*cos(thrta) - (t1.y - y)*sin(thrta);
	t.y = y + (t1.x - x)*sin(thrta) + (t1.y - y)*cos(thrta);
	return t;
}

//计算旋转角度（求两向量夹角）
double CLianXi2View::Calthrta(CPoint p1, CPoint p2, CPoint p3) {
	Vector s1 = Vector(p1.x - p3.x, p1.y - p3.y);
	Vector s2 = Vector(p2.x - p3.x, p2.y - p3.y);
	double mul = Dot(s1, s2);
	double thrta = acos(mul / (Length(s1)*Length(s2))) / pi * 180;
	double t = Cross(s1, s2) > 0 ? 1 : -1;
	return t * thrta;
}

//画点
void CLianXi2View::putPixel(int x, int y, int width, COLORREF color)
{
	CPen pen(PS_SOLID, width, color);
	CClientDC dc(this);
	CPen* pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(CPoint(x,y));
	dc.LineTo(CPoint(x,y));
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
}

//获取点的颜色
COLORREF CLianXi2View::getPixel(CDC &dc,int x, int y)
{
	return dc.GetPixel(round(x), round(y));
}

//CPen画线
void CLianXi2View::DefaultDrawLine(int x0, int y0, int x1, int y1, int width, COLORREF color) {
	CPen pen(PS_SOLID, width, color);
	CClientDC dc(this);
	CPen* pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(CPoint(x0, y0));
	dc.LineTo(CPoint(x1, y1));
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
}

//DDA画线
void CLianXi2View::drawline(int x0, int y0, int x1, int y1, int width, COLORREF color)
{
	double dx, dy, n, k;
	dx = x1 - x0;
	dy = y1 - y0;
	if (abs(dx) > abs(dy))
		n = abs(dx);
	else
		n = abs(dy);
	double xinc = (double)dx / n;
	double yinc = (double)dy / n;
	double x = x0;
	double y = y0;
	for (k = 1; k <= n; k++) {
		putPixel(x, y, width, color);
		x += xinc;
		y += yinc;
	}
}

//中点画线
void CLianXi2View::MidDrawline(int x0, int y0, int x1, int y1, int width, COLORREF color)
{
	// TODO: 在此添加命令处理程序代码
	int x = x0, y = y0;
	int a = y0 - y1;
	int b = x1 - x0;
	int cx = (b >= 0 ? 1 : (b = -b, -1));
	int cy = (a <= 0 ? 1 : (a = -a, -1));
	int d, d1, d2;
	if (-a <= b) { // 斜率绝对值 <= 1
		d = a + a + b;
		d1 = a + a;
		d2 = a + a + b + b;
		while (x != x1) {
			if (d < 0) {
				y += cy;
				d += d2;
			}
			else {
				d += d1;
			}
			x += cx;
			putPixel(x, y, width, color);
		}
	}
	else { // 斜率绝对值 > 1
		d = a + b + b;
		d1 = b + b;
		d2 = a + a + b + b;
		while (y != y1) {
			if (d < 0) {
				d += d1;
			}
			else {
				x += cx;
				d += d2;
			}
			y += cy;
			putPixel(x, y, width, color);
		}
	}
}

//Bresenham画线
void CLianXi2View::BresenhamLine(int x0, int y0, int x1, int y1, int width, COLORREF color)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int ux = ((dx > 0) << 1) - 1;
	int uy = ((dy > 0) << 1) - 1;
	int x = x0, y = y0, eps;

	eps = 0; dx = abs(dx); dy = abs(dy);
	if (dx > dy) {
		for (x = x0; x != x1; x += ux) {
			putPixel(x, y, width, color);
			eps += dy;
			if ((eps << 1) >= dx) {
				y += uy; eps -= dx;
			}
		}
	}
	else {
		for (y = y0; y != y1; y += uy) {
			putPixel(x, y, width, color);
			eps += dx;
			if ((eps << 1) >= dy) {
				x += ux; eps -= dy;
			}
		}
	}
	return;
}

//画圆的八个对称点
void CLianXi2View::CirPot(int x0, int y0, int x, int y, int width,COLORREF color) 
{

	putPixel(x0 + x, y0 + y, width, color);
	putPixel(x0 + y, y0 + x, width, color);
	putPixel(x0 + y, y0 - x, width, color);
	putPixel(x0 + x, y0 - y, width, color);
	putPixel(x0 - x, y0 - y, width, color);
	putPixel(x0 - y, y0 - x, width, color);
	putPixel(x0 - y, y0 + x, width, color);
	putPixel(x0 - x, y0 + y, width, color);
}

//CPen+Arc画圆
void CLianXi2View::DefaultDrawCircle(int x0, int y0, int radius, int width, COLORREF color) {
	CPen pen(PS_SOLID, width, color);
	CClientDC dc(this);
	CPen* pOldPen = (CPen *)dc.SelectObject(&pen);
	dc.SelectObject(&pen);
	dc.Arc(CRect(x0 - radius, y0 - radius, x0 + radius, y0 + radius), CPoint(0, 0), CPoint(0, 0));
	dc.SelectObject(&pOldPen);
	pen.DeleteObject();
}

//Bresnham画圆
void CLianXi2View::OnBresenhamcircle(int x0, int y0, int r, int width, COLORREF color)
{
	// TODO: 在此添加命令处理程序代码
	double d;
	d = 1.25 - r;
	int x = 0;
	int y = r;
	for (x = 0; x <= y; x++) {
		CirPot(x0, y0, x, y, width, color);
		if (d < 0)
			d += 2 * x + 3;
		else {
			d += 2 * (x - y) + 5;
			y--;
		}
	}
}

//正负判定画圆
void CLianXi2View::PNCircle(int x, int y, int r, int width,COLORREF color) {
	int tx = 0, ty = r, f = 0;
	while (tx <= ty)
	{
		CirPot(x, y, tx, ty, width, color);
		if (f <= 0)
			f = f + 2 * tx + 1, tx++;
		else
			f = f - 2 * ty + 1, ty--;
	}
}

//中点画圆
void CLianXi2View::OnMidpointcircle(int x0, int y0, int r, int width, COLORREF color)
{
	// TODO: 在此添加命令处理程序代码
	int h;
	int x = 0;
	int y = int(r);
	h = 1 - int(r);
	CirPot(x0, y0, x, y, width, color);
	while (x < y) {
		if (h < 0)
			h += 2 * x + 2;
		else {
			h += 2 * (x - y) + 5;
			y--;
		}
		x++;
		CirPot(x0, y0, x, y, width, color);
	}
}

//画椭圆的四个对称点
void CLianXi2View::EllipsePot(int x_center, int y_center, int x, int y, int width, COLORREF color,double angle)
{
	
	CPoint t1 = GetRotatePoint(CPoint(x_center + x, y_center + y), angle, x_center, y_center);
	CPoint t2 = GetRotatePoint(CPoint(x_center - x, y_center + y), angle, x_center, y_center);
	CPoint t3 = GetRotatePoint(CPoint(x_center + x, y_center - y), angle, x_center, y_center);
	CPoint t4 = GetRotatePoint(CPoint(x_center - x, y_center - y), angle, x_center, y_center);
	putPixel(t1.x, t1.y, width, color);
	putPixel(t2.x, t2.y, width, color);
	putPixel(t3.x, t3.y, width, color);
	putPixel(t4.x, t4.y, width, color);
}

//CPen+Arc画椭圆
void CLianXi2View::DefaultDrawEllipse(int x_center, int y_center, int al, int bl, int width, COLORREF color, double angle) {
	CPen pen(PS_SOLID, width, color);
	CClientDC dc(this);
	CPen* pOldPen = (CPen *)dc.SelectObject(&pen);
	dc.SelectObject(&pen);
	CPoint p1 = GetRotatePoint(CPoint(x_center - al, y_center - bl), angle, x_center, y_center);
	CPoint p2 = GetRotatePoint(CPoint(x_center + al, y_center + bl), angle, x_center, y_center);
	dc.Arc(CRect(p1.x, p1.y, p2.x, p2.y), CPoint(0, 0), CPoint(0, 0));
	dc.SelectObject(&pOldPen);
	pen.DeleteObject();
}

//中点画椭圆法
void CLianXi2View::MidDrawEllipse(int x_center, int y_center, int rx, int ry, int width, COLORREF color,double angle)
{
	// TODO: 在此添加命令处理程序代码
	int p;
	int rx2 = rx * rx, ry2 = ry * ry;
	int x = 0, y = ry;
	int px = 0, py = 2 * rx2 * y;
	EllipsePot(x_center, y_center, x, y, width, color, angle);
	//区域1
	p = round(ry2 - (rx2*ry) + (0.25*rx2));
	while (px < py) {
		x++;
		px += 2 * ry2;
		if (p < 0) p += ry2 + px;
		else {
			y--;
			py -= 2 * rx2;
			p += ry2 + px - py;
		}
		EllipsePot(x_center, y_center, x, y, width, color, angle);
	}
	//区域2
	p = round(ry2*(x + 0.5)*(x + 0.5) + rx2 * (y - 1)*(y - 1) - rx2 * ry2);
	while (y > 0) {
		y--;
		py -= 2 * rx2;
		if (p > 0) p += rx2 - py;
		else {
			x++;
			px += 2 * ry2;
			p += rx2 - py + px;
		}
		EllipsePot(x_center, y_center, x, y, width, color, angle);
	}
}

//Bresenham画椭圆圆
void CLianXi2View::BresenhamEllipse(int xc, int yc, int a, int b, int width, COLORREF color,double angle)
{
	int sqa = a * a;
	int sqb = b * b;
	int x = 0;
	int y = b;
	int d = 2 * sqb - 2 * b * sqa + sqa;
	EllipsePot(xc, yc, x, y, width, color,angle);
	int P_x = round((double)sqa / sqrt((double)(sqa + sqb)));
	while (x <= P_x){
		if (d < 0){
			d += 2 * sqb * (2 * x + 3);
		}
		else{
			d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
			y--;

		}
		x++;
		EllipsePot(xc, yc, x, y, width, color, angle);
	}
	d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
	while (y >= 0){
		EllipsePot(xc, yc, x, y, width, color, angle);
		y--;
		if (d < 0){
			x++;
			d = d - 2 * sqa * y - sqa + 2 * sqb * x + 2 * sqb;
		}
		else{
			d = d - 2 * sqa * y - sqa;
		}
	}
}

//画多边形
void CLianXi2View::Drawpolygon(vector<CPoint> points, int width,COLORREF color) {
	int size = points.size();
	for (int i = 0; i < size; i++) {
		CPoint p1 = points[i], p2 = points[(i + 1) % size];
		DefaultDrawLine(p1.x, p1.y, p2.x, p2.y, width, color);
	}
}

//填充
void CLianXi2View::TChong(int x, int y, COLORREF color)
{
	CDC *pDC = GetDC();
	CBrush cbrush;
	cbrush.CreateSolidBrush(color);
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&cbrush);
	COLORREF crColor = pDC->GetPixel(x,y);
	pDC->ExtFloodFill(x, y, crColor, FLOODFILLSURFACE);
	pDC->SelectObject(pOldBrush);
	cbrush.DeleteObject();
	ReleaseDC(pDC);
}

//橡皮筋画线
void CLianXi2View::InverLine(CPoint pFrom, CPoint pTo) {
	CPen pen(PS_DOT, 1, RGB(0, 0, 0));
	CClientDC dc(this);
	int nOldMode = dc.SetROP2(R2_NOT);
	CPen* pOldPen = (CPen *)dc.SelectObject(&pen);
	dc.SelectObject(&pen);
	dc.MoveTo(pFrom.x, pFrom.y);
	dc.LineTo(pTo.x, pTo.y);
	dc.SetROP2(nOldMode);
	dc.SelectObject(&pOldPen);
	pen.DeleteObject();
}

//橡皮筋画圆
int CLianXi2View::InverCircle(CPoint pFrom, CPoint pTo) {
	CPen pen(PS_DOT, 1, RGB(0, 0, 0));
	CClientDC dc(this);
	int nOldMode = dc.SetROP2(R2_NOT);
	CPen* pOldPen = (CPen *)dc.SelectObject(&pen);
	dc.SelectObject(&pen);
	int radius = dist(CPoint(pFrom.x, pFrom.y), CPoint(pTo.x, pTo.y));
	dc.Arc(CRect(pFrom.x - radius, pFrom.y - radius, pFrom.x + radius, pFrom.y + radius), CPoint(0, 0), CPoint(0, 0));
	dc.SetROP2(nOldMode);
	dc.SelectObject(&pOldPen);
	pen.DeleteObject();
	return radius;
}

//橡皮筋画椭圆
pair<int,int> CLianXi2View::InverEllipse(CPoint pFrom, CPoint pTo) {
	CPen pen(PS_DOT, 1, RGB(0, 0, 0));
	CClientDC dc(this);
	int nOldMode = dc.SetROP2(R2_NOT);
	CPen* pOldPen = (CPen *)dc.SelectObject(&pen);
	dc.SelectObject(&pen);
	int al = abs(pFrom.x - pTo.x);
	int bl = abs(pFrom.y - pTo.y);
	dc.Arc(CRect(MLst0.x - al, MLst0.y - bl, MLst0.x + al, MLst0.y + bl), CPoint(0, 0), CPoint(0, 0));
	dc.SetROP2(nOldMode);
	dc.SelectObject(&pOldPen);
	pen.DeleteObject();
	return pair<int,int>(al,bl);
}

//平移变换
void CLianXi2View::Pan(int tx, int ty) {
	if (Mypt) {
		if (MyType == 1) {
			MyLine *pt = dynamic_cast<MyLine *>(Mypt);
			CPoint sp = pt->getSp(), ep = pt->getEp();
			sp.x = sp.x + tx;
			sp.y = sp.y + ty;
			ep.x = ep.x + tx;
			ep.y = ep.y + ty;
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setSp(sp);
			pt->setEp(ep);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		if (MyType == 2) {
			MyCircle *pt = dynamic_cast<MyCircle *>(Mypt);
			CPoint center = pt->getCenter();
			center.x += tx;
			center.y += ty;
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setCenter(center);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		if (MyType == 3) {
			MyEllipse *pt = dynamic_cast<MyEllipse *>(Mypt);
			CPoint center = pt->getCenter();
			center.x += tx;
			center.y += ty;
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setCenter(center);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		if (MyType == 4) {
			MyPolygon *pt = dynamic_cast<MyPolygon *>(Mypt);
			vector<CPoint> points = pt->getPoints();
			for (int i = 0; i < points.size(); i++) {
				points[i].x += tx;
				points[i].y += ty;
			}
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setColor(RGB(255, 0, 0));
			pt->setPoints(points);
			pt->draw();
		}
		Invalidate(0);
	}
}

//旋转变换
void CLianXi2View::Rotate(double thrta, int cx, int cy) {
	if (Mypt) {
		if (MyType == 1) {
			MyLine *pt = dynamic_cast<MyLine *>(Mypt);
			CPoint sp = pt->getSp(), ep = pt->getEp();
			sp = GetRotatePoint(sp, thrta, cx, cy);
			ep = GetRotatePoint(ep, thrta, cx, cy);
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setSp(sp);
			pt->setEp(ep);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		if (MyType == 2) {
			;
		}
		if (MyType == 3) {
			MyEllipse *pt = dynamic_cast<MyEllipse *>(Mypt);
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setAngle((int)(thrta + pt->getAngle() + 180) % 180);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		if (MyType == 4) {
			MyPolygon *pt = dynamic_cast<MyPolygon *>(Mypt);
			vector<CPoint> points = pt->getPoints();
			for (int i = 0; i < points.size(); i++) {
				points[i] = GetRotatePoint(points[i], thrta, cx, cy);
			}
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setColor(RGB(255, 0, 0));
			pt->setPoints(points);
			pt->draw();
		}
		Invalidate(0);
	}
}

//缩放变换
void CLianXi2View::Scale(int cx, int cy, double times) {
	if (Mypt) {
		if (MyType == 1) {
			MyLine *pt = dynamic_cast<MyLine *>(Mypt);
			CPoint sp = pt->getSp(), ep = pt->getEp();
			sp.x = cx + (sp.x - cx)*times;
			sp.y = cy + (sp.y - cy)*times;
			ep.x = cx + (ep.x - cx)*times;
			ep.y = cy + (ep.y - cy)*times;
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setSp(sp);
			pt->setEp(ep);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		if (MyType == 2) {
			MyCircle *pt = dynamic_cast<MyCircle *>(Mypt);
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setR(pt->getR()*times);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		if (MyType == 3) {
			MyEllipse *pt = dynamic_cast<MyEllipse *>(Mypt);
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setAl(pt->getAl()*times);
			pt->setBl(pt->getBl()*times);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		if (MyType == 4) {
			MyPolygon *pt = dynamic_cast<MyPolygon *>(Mypt);
			vector<CPoint> points = pt->getPoints();
			for (int i = 0; i < points.size(); i++) {
				points[i].x = cx + (points[i].x - cx)*times;
				points[i].y = cy + (points[i].y - cy)*times;
			}
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setPoints(points);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		Invalidate(0);
	}
}

//对称对换
void CLianXi2View::DuiChen(int cx, int cy) {
	if (Mypt) {
		if (MyType == 1) {
			MyLine *pt = dynamic_cast<MyLine *>(Mypt);
			CPoint sp = pt->getSp(), ep = pt->getEp();
			sp.x = 2 * cx - sp.x;
			ep.x = 2 * cx - ep.x;
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setSp(sp);
			pt->setEp(ep);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		if (MyType == 2) {
			;
		}
		if (MyType == 3) {
			MyEllipse *pt = dynamic_cast<MyEllipse *>(Mypt);
			double angle = pt->getAngle();
			if (abs(angle) > 90) {
				if (angle < 0) {
					Rotate(-2 * (180 - abs(angle)), cx, cy);
				}
				else {
					Rotate(2 * (180 - abs(angle)), cx, cy);
				}
			}
			else {
				if (angle < 0) {
					Rotate(-2 * (90 - abs(angle)), cx, cy);
				}
				else {
					Rotate(2 * (90 - abs(angle)), cx, cy);
				}
			}
		}
		if (MyType == 4) {
			MyPolygon *pt = dynamic_cast<MyPolygon *>(Mypt);
			vector<CPoint> points = pt->getPoints();
			for (int i = 0; i < points.size(); i++) {
				points[i].x = 2 * cx - points[i].x;
			}
			pt->setColor(RGB(255, 255, 255));
			pt->draw();
			pt->setPoints(points);
			pt->setColor(RGB(255, 0, 0));
			pt->draw();
		}
		Invalidate(0);
	}
}

//画贝塞尔曲线
void CLianXi2View::DrawBezier() {
	CClientDC pDC(this);
	int n = BPoints.size() - 1;//n阶贝塞尔
	pDC.MoveTo(BPoints[0].x, BPoints[0].y);
	for (int i = 1; i <= n; i++) {
		pDC.LineTo(BPoints[i].x, BPoints[i].y);
		TRACE("%d %d\n", BPoints[i].x, BPoints[i].y);
	}
	pDC.MoveTo(BPoints[0].x, BPoints[0].y);
	for (double t = 0; t <= 1; t += 0.001) {
		double rx = 0, ry = 0;
		for (int i = 0; i <= n; i++) {
			double qt = fac(n) / (fac(i)*fac(n - i));
			CPoint p = BPoints[i];
			rx += qt * pow(t, i)*pow(1 - t, n - i)*p.x;
			ry += qt * pow(t, i)*pow(1 - t, n - i)*p.y;
		}
		pDC.LineTo(rx, ry);
	}
}

//显示图片
void CLianXi2View::ShowBitmap(CDC *pDC, CString BmpName)
{
	//定义bitmap指针 调用函数LoadImage装载位图
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

	/*************************************************************************/
	/* 1.要装载OEM图像，则设此参数值为0  OBM_ OEM位图 OIC_OEM图标 OCR_OEM光标
	/* 2.BmpName要装载图片的文件名
	/* 3.装载图像类型:
	/*   IMAGE_BITMAP-装载位图 IMAGE_CURSOR-装载光标 IMAGE_ICON-装载图标
	/* 4.指定图标或光标的像素宽度和长度 以像素为单位
	/* 5.加载选项:
	/*   IR_LOADFROMFILE-指明由lpszName指定文件中加载图像
	/*   IR_DEFAULTSIZE-指明使用图像默认大小
	/*   LR_CREATEDIBSECTION-当uType参数为IMAGE_BITMAP时,创建一个DIB项
	/**************************************************************************/

	if (m_bitmap.m_hObject)
	{
		m_bitmap.Detach();           //切断CWnd和窗口联系
	}
	m_bitmap.Attach(m_hBitmap);      //将句柄HBITMAP m_hBitmap与CBitmap m_bitmap关联

									 //边界
	CRect rect;
	GetClientRect(&rect);

	//图片显示(x,y)起始坐标
	int m_showX = 0;
	int m_showY = 0;
	int m_nWindowWidth = rect.right - rect.left;   //计算客户区宽度
	int m_nWindowHeight = rect.bottom - rect.top;  //计算客户区高度

												   //定义并创建一个内存设备环境DC
	CDC dcBmp;
	if (!dcBmp.CreateCompatibleDC(pDC))   //创建兼容性的DC
		return;

	BITMAP m_bmp;                          //临时bmp图片变量
	m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中

	CBitmap *pbmpOld = NULL;
	dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境

										   //图片显示调用函数stretchBlt
	pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);

	dcBmp.SelectObject(pbmpOld);           //恢复临时DC的位图
	DeleteObject(&m_bitmap);               //删除内存中的位图
	dcBmp.DeleteDC();                      //删除CreateCompatibleDC得到的图片DC
}

//保存当前画布为图片
void CLianXi2View::OnFilesave()
{
	// TODO: 在此添加命令处理程序代码
	Invalidate(FALSE);
	CClientDC dc(this);
	CRect rect;
	BOOL  showMsgTag;                  //是否要弹出”图像保存成功对话框" 
	GetClientRect(&rect);                  //获取画布大小
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	//创建兼容位图
	HDC hdc = CreateCompatibleDC(dc);      //创建兼容DC，以便将图像保存为不同的格式
	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);
	//将位图选入DC，并保存返回值 
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
	//将屏幕DC的图像复制到内存DC中
	CImage image;
	image.Attach(hbitmap);                //将位图转化为一般图像
	if (!saveTag)                          //如果图像是第一次被写入,则打开对话框
	{
		saveTag = TRUE;
		showMsgTag = TRUE;
		CString  strFilter = _T("位图文件(*.bmp)|*.bmp|JPEG 图像文件|*.jpg|GIF图像文件|*.gif|PNG图像文件|*.png|其他格式(*.*)|*.*||");
		CFileDialog dlg(FALSE, _T("bmp"), _T("iPaint1.bmp"), NULL, strFilter);
		if (dlg.DoModal() != IDOK)
			return;

		CString strFileName;          //如果用户没有指定文件扩展名，则为其添加一个
		CString strExtension;
		strFileName = dlg.m_ofn.lpstrFile;
		if (dlg.m_ofn.nFileExtension = 0)               //扩展名项目为0
		{
			switch (dlg.m_ofn.nFilterIndex) {
			case 1:
				strExtension = "bmp"; break;
			case 2:
				strExtension = "jpg"; break;
			case 3:
				strExtension = "gif"; break;
			case 4:
				strExtension = "png"; break;
			default:
				break;
			}
			strFileName = strFileName + "." + strExtension;
		}
		saveFilePath = strFileName;     //saveFilePath为视类中的全局变量,类型为CString
	}
	else {
		showMsgTag = FALSE;
	}
	//AfxMessageBox(saveFilePath);               //显示图像保存的全路径(包含文件名)
	HRESULT hResult = image.Save(saveFilePath);     //保存图像
	if (FAILED(hResult)) {
		MessageBox(_T("保存图像文件失败！"));
	}
	else {
		if (showMsgTag)
			MessageBox(_T("文件保存成功！"));
	}
	image.Detach();
	SelectObject(hdc, hOldMap);
}

//打开图片文件
void CLianXi2View::OnOpenfile()
{
	// TODO: 在此添加命令处理程序代码
	CString filter;
	filter = "位图文件(*.bmp)|*.bmp|其他格式(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	//按下确定按钮 dlg.DoModal() 函数显示对话框
	if (dlg.DoModal() == IDOK)
	{
		BmpName = dlg.GetPathName();     //获取文件路径名   如D:\pic\abc.bmp
		EntName = dlg.GetFileExt();      //获取文件扩展名
		EntName.MakeLower();             //将文件扩展名转换为一个小写字符
		TRACE("sufshjkdgsjkdg");
		Invalidate(1);                    //调用该函数就会调用OnDraw重绘画图
	}
}

//以下为菜单栏事件
void CLianXi2View::OnDrawline()
{
	// TODO: 在此添加命令处理程序代码
	DrawLineDLG dlg;
	dlg.DoModal();
}

void CLianXi2View::OnDrawpoint()
{
	// TODO: 在此添加命令处理程序代码
	Mflag = 1;
	Mmethod = 0;
}

void CLianXi2View::OnDrawcircle()
{
	// TODO: 在此添加命令处理程序代码
	DrawCircleDLG dlg;
	dlg.DoModal();
}

void CLianXi2View::OnDrawellipse()
{
	// TODO: 在此添加命令处理程序代码
	DrawEllipseDLG dlg;
	dlg.DoModal();
}

void CLianXi2View::OnDrawpolygon()
{
	// TODO: 在此添加命令处理程序代码
	Mflag = 5;
	Mmethod = 0;
}

void CLianXi2View::OnClear()
{
	// TODO: 在此添加命令处理程序代码
	Invalidate(1);
	CLianXi2Doc* pDoc = GetDocument();
	auto st = pDoc->graphs.begin();
	auto en = pDoc->graphs.end();
	while (st != en) {
		delete (*st);
		st++;
	}
	pDoc->graphs.clear();
	Mypt = nullptr;
	MPoints.clear();
	Mflag = 0;
}

void CLianXi2View::OnCut()
{
	// TODO: 在此添加命令处理程序代码
	Mflag = 7;
}

void CLianXi2View::OnSelect()
{
	// TODO: 在此添加命令处理程序代码
	Mflag = 11;
}

void CLianXi2View::OnTianchong()
{
	// TODO: 在此添加命令处理程序代码
	TCDLG dlg;
	dlg.DoModal();
}

void CLianXi2View::OnTranslatemove()
{
	// TODO: 在此添加命令处理程序代码
	Mflag = 12;
}

void CLianXi2View::OnRotatemove()
{
	// TODO: 在此添加命令处理程序代码
	Mflag = 13;
}

void CLianXi2View::OnScalemove()
{
	// TODO: 在此添加命令处理程序代码
	Mflag = 14;
}

void CLianXi2View::OnSymmetrymove()
{
	// TODO: 在此添加命令处理程序代码
	CPoint cp = GetCPos();
	DuiChen(cp.x,cp.y);
	Mflag = 15;
	/*
	ColorRecover();
	Mypt = nullptr;
	Mflag = 0;
	*/
}

void CLianXi2View::OnSetwidthandcolor()
{
	// TODO: 在此添加命令处理程序代码
	ColorAndWidthDLG dlg;
	dlg.DoModal();
}

void CLianXi2View::OnBezier()
{
	// TODO: 在此添加命令处理程序代码
	Mflag = 22;
}

//以下为鼠标信息处理事件
void CLianXi2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CLianXi2Doc* pDoc = GetDocument();
	CDC *pDC = GetDC();
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (Mflag == 1) {
		MyGraph *pt = new MyPoint(10,point,Mcolor,Mwidth);
		pt->draw();
		pDoc->graphs.push_back(pt);
	}
	else if (Mflag == 2) {
		Mflag = 10;
	}
	else if (Mflag == 3) {
		Mflag = 16;
	}
	else if (Mflag == 4) {
		Mflag = 17;
	}
	else if (Mflag == 5) {
		MPst = point;
		MPoints.push_back(point);
		Mflag = 18;
	}
	else if (Mflag == 6) {
		TChong(point.x, point.y, Mcolor);
	}
	else if (Mflag == 7) {
		CClientDC dc(this);
		m_RectTracker.TrackRubberBand(this, point, TRUE);
		m_RectTracker.Draw(pDC);
		Mflag = 8;
	}
	else if (Mflag == 8) {
		CRect rect_old = m_RectTracker.m_rect;
		int judge = m_RectTracker.HitTest(point);
		if (judge < 0) { //光标位置在选框外部，重置选择框
			m_RectTracker.TrackRubberBand(this, point, TRUE);
			m_RectTracker.Draw(pDC);
			Mflag= 7;
		}
		else {
			m_RectTracker.Track(this, point, TRUE);
			m_RectTracker.Draw(pDC);
			int x = m_RectTracker.m_rect.TopLeft().x, y = m_RectTracker.m_rect.TopLeft().y, rx = rect_old.TopLeft().x, ry = rect_old.TopLeft().y;
			int width = m_RectTracker.m_rect.Width(), height = m_RectTracker.m_rect.Height();
			BitBlt(pDC->m_hDC, x, y, width, height, pDC->m_hDC, rx, ry, SRCCOPY);
		}
		rect_old.top--; rect_old.bottom++; rect_old.left--; rect_old.right++;
		pDC->FillSolidRect(rect_old, RGB(255, 255, 255));
	}
	else if (Mflag == 11) {
		FindSelect(point);
	}
	else if (Mflag == 18) {
		InverLine(CPoint(MLst0.x, MLst0.y), CPoint(MLst1.x, MLst1.y));
		if (dist(CPoint(MLst1.x, MLst1.y), CPoint(MPst.x, MPst.y)) < 3 * Mwidth) {
			DefaultDrawLine(MLst0.x, MLst0.y, MPst.x, MPst.y, Mwidth, Mcolor);
			MyGraph *pt = new MyPolygon(MPoints, Mcolor, Mwidth, Mmethod);
			pDoc->graphs.push_back(pt);
			Mflag = 5;
			MPoints.clear();
		}
		else {
			DefaultDrawLine(MLst0.x, MLst0.y, MLst1.x, MLst1.y, Mwidth, Mcolor);
			MPoints.push_back(MLst1);
		}
	}
	else if (Mflag == 22) {
		Mflag=23;
		MPst = point;
		BPoints.push_back(point);
	}
	else if (Mflag == 23) {
		InverLine(CPoint(MLst0.x, MLst0.y), CPoint(MLst1.x, MLst1.y));
		DefaultDrawLine(MLst0.x, MLst0.y, MLst1.x, MLst1.y, 1, RGB(0,0,0));
		BPoints.push_back(MLst1);
	}
	ReleaseDC(pDC);
	MLst0 = MLst1 = point;
	CView::OnLButtonDown(nFlags, point);
}

void CLianXi2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (Mflag == 10) {
		InverLine(MLst0, MLst1);
		MLst1 = point;
		InverLine(MLst0, MLst1);
	}
	else if (Mflag == 16) {
		InverCircle(MLst0, MLst1);
		MLst1 = point;
		InverCircle(MLst0, MLst1);
	}
	else if (Mflag == 17) {
		InverEllipse(MLst0, MLst1);
		MLst1 = point;
		InverEllipse(MLst0, MLst1);
	}
	else if (Mflag == 18) {
		InverLine(MLst0, MLst1);
		MLst1 = point;
		InverLine(MLst0, MLst1);
	}
	else if (Mflag == 23) {
		InverLine(MLst0, MLst1);
		MLst1 = point;
		InverLine(MLst0, MLst1);
	}
	CView::OnMouseMove(nFlags, point);
}

void CLianXi2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CLianXi2Doc* pDoc = GetDocument();
	if (Mflag == 7) {
		m_RectTracker.m_rect.SetRect(10, 10, 100, 100);
	}
	else if (Mflag == 10) {
		Mflag = 2;
		InverLine(MLst0, MLst1);
		MyGraph *pt = new MyLine(MLst0, MLst1, Mcolor, Mwidth, Mmethod);
		pt->draw();
		pDoc->graphs.push_back(pt);
	}
	else if (Mflag == 12) {
		if (Mypt) {
			int dx = point.x - MLst0.x;
			int dy = point.y - MLst0.y;
			Pan(dx, dy);
			/*
			ColorRecover();
			Mypt = nullptr;
			Mflag = 0;
			*/
		}
	}
	else if (Mflag == 13) {
		if (Mypt) {
			double thrta = 0;
			CPoint cp = GetCPos();
			thrta = Calthrta(MLst0, point, cp);
			Rotate(thrta, cp.x, cp.y);
			/*
			ColorRecover();
			Mypt = nullptr;
			Mflag = 0;
			*/
		}
	}
	else if (Mflag == 14) {
		if (Mypt) {
			CPoint cp = GetCPos();
			double times = dist(point, cp) / dist(MLst0, cp);
			Scale(cp.x, cp.y, times);
			/*
			ColorRecover();
			Mypt = nullptr;
			Mflag = 0;
			*/
		}
	}
	else if (Mflag == 16) {
		int radius = InverCircle(MLst0, MLst1);
		MyGraph *pt = new MyCircle(radius, MLst0, Mcolor, Mwidth, Mmethod);
		pt->draw();
		pDoc->graphs.push_back(pt);
		Mflag = 3;
	}
	else if (Mflag == 17) {
		pair<int,int>pr=InverEllipse(MLst0, MLst1);
		MyGraph *pt = new MyEllipse(pr.first, pr.second, MLst0, Mcolor, Mwidth, Mmethod);
		pt->draw();
		pDoc->graphs.push_back(pt);
		Mflag = 4;
	}
	CView::OnLButtonUp(nFlags, point);
}

void CLianXi2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*
	CLianXi2Doc* pDoc = GetDocument();
	*/
	if (Mflag >= 12 && Mflag <= 15) {
		FindSelect(point);
	}
	if (Mflag == 23) {
		Mflag = 22;
		DrawBezier();
		BPoints.clear();
	}
	CView::OnRButtonDown(nFlags, point);
}


