
// LianXi2Doc.h: CLianXi2Doc 类的接口
//


#pragma once
#include "vector"
#include "list"
#include "memory"

class CLianXi2Doc : public CDocument
{
protected: // 仅从序列化创建
	CLianXi2Doc() noexcept;
	DECLARE_DYNCREATE(CLianXi2Doc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CLianXi2Doc();

	//图形相关类或结构体声明
	class MyGraph {
	protected:
		COLORREF color;//图形颜色
		int width;	   //线宽
		int method;	   //画图形方法
		int type;	   //图形类型
	public:
		MyGraph() {}
		MyGraph(COLORREF _color, int _width, int _method) {
			color = _color;
			width = _width;
			method = _method;
			type = 0;
		}
		COLORREF getColor() { return color; }
		int getWidth() { return width; }
		int getmethod() { return method; }
		int getType() { return type; }
		void setColor(int _color) { color = _color; }
		void setWidth(int _width) { width = _width; }
		void setmethod(int _method) { method = _method; }
		virtual void draw() {}
	};

	class MyLine :public MyGraph {
	private:
		CPoint sp;	//起始端点
		CPoint ep;	//结束端点
		//备注 画线方法(method),0:默认 1:DDA 2:中点画线 3:Brashham
	public:
		MyLine() {}
		MyLine(CPoint _sp, CPoint _ep, COLORREF _color = RGB(255, 255, 255), int _width = 2, int method = 0) 
			:MyGraph(_color, _width, method) {
			sp = _sp;
			ep = _ep;
			type = 1;
		}
		CPoint getSp() { return sp; }
		CPoint getEp() { return ep; }
		void setSp(CPoint _sp) { sp = _sp; }
		void setEp(CPoint _ep) { ep = _ep; }
		virtual void draw();
	};

	class MyCircle :public MyGraph {
	private:
		int r;			//半径
		CPoint center;	//圆心
	public:
		//备注 画圆方法(method),0:默认 1:中点画圆 2:Breshham 3:PN判正负画圆
		MyCircle() {}
		MyCircle(int _r, CPoint _center, COLORREF _color = RGB(255, 255, 255), int _width = 2, int _method = 0) 
			:MyGraph(_color, _width, _method) {
			r = _r;
			center = _center;
			type = 2;
		}
		int getR() { return r; }
		CPoint getCenter() { return center; }
		void setR(int _r) { r = _r; }
		void setCenter(CPoint _center) { center = _center; }
		virtual void draw();
	};

	class MyEllipse :public MyGraph{
	private:
		int al;			//长轴
		int bl;			//短轴
		CPoint center; //椭圆心
		double angle;   //旋转角度
	public:
		//备注 画椭圆方法(method)，0:默认 1:中点画椭圆  2:Breshham
		MyEllipse() {}
		MyEllipse(int _al, int _bl, CPoint _center, COLORREF _color = RGB(255, 255, 255), int _width = 2, int _method = 2) 
			:MyGraph(_color, _width, _method) {
			al = _al;
			bl = _bl;
			center = _center;
			angle = 0;
			type = 3;
		}
		int getAl() { return al; }
		int getBl() { return bl; }
		double getAngle() { return angle; }
		CPoint getCenter() { return center; }
		void setAl(int _al) { al = _al; }
		void setBl(int _bl) { bl = _bl; }
		void setCenter(CPoint _center) { center = _center; }
		void setAngle(double _angle) { angle = _angle; }
		virtual void draw();
	};

	class MyPolygon :public MyGraph {
	private:
		std::vector<CPoint>points;//多边形顶点
	public:
		MyPolygon() {}
		MyPolygon(std::vector<CPoint> &_points, COLORREF _color = RGB(255, 255, 255), int _width = 2, int _method = 0)
			:MyGraph(_color, _width, _method) {
			points = _points;
			type = 4;
		}
		void setPoints(const std::vector<CPoint> & _points) { points = _points; }
		std::vector<CPoint> getPoints() { return points; }
		virtual void draw();
	};
	 
	class MyPoint :public MyGraph {
	private:
		int r;		    //大小
		CPoint center;	//圆心
	public:
		MyPoint() {}
		MyPoint(int _r, CPoint _center, COLORREF _color = RGB(255, 255, 255), int _width = 2, int _method = 0)
			:MyGraph(_color, _width, _method) {
			r = _r;
			center = _center;
			type = 5;
		}
		int getR() { return r; }
		CPoint getCenter() { return center; }
		void setR(int _r) { r = _r; }
		void setCenter(CPoint _center) { center = _center; }
		virtual void draw();
	};

	class MyBesizer :public MyGraph{
	private:
		std::vector<CPoint>points;
		bool showPoint;
	public:
		MyBesizer(std::vector<CPoint>_points, bool _showPoint, COLORREF _color = RGB(255, 255, 255), int _width = 2, int _method = 0)
			:MyGraph(_color, _width, _method) {
			points = _points;
			showPoint = _showPoint;
			type = 6;
		}
		void setPoints(std::vector<CPoint>_points) { points = _points; }
		std::vector<CPoint> getPoints() { return points; }
		void setShowPoint(bool _showPoint) {showPoint = _showPoint;}
		bool getShowPoint() { return showPoint; }
		virtual void draw();
	};

	//矩阵类
	class Matrix
	{
	public:
		Matrix(int m, int n);                 //构建一个m*n的全零矩阵
		Matrix(int n);                        //构建一个n*n的单位矩阵
		Matrix(const Matrix &);               //拷贝构造函数，深拷贝
		Matrix(double* items, int m, int n);  //根据数组拷贝一个矩阵
		~Matrix();
		int getRowNum() const;                //返回矩阵的行数
		int getColNum() const;                //返回矩阵的列数

		Matrix Trans() const;                      //将矩阵转置

		double get(int i, int j) const;            //返回矩阵第i行j列元素
		void set(int i, int j, double val);        //设置矩阵第i行j列元素

		Matrix operator +(const Matrix &m);         //两个矩阵相加
		Matrix operator -(const Matrix &m);         //两个矩阵相减
		Matrix operator *(const Matrix &m);         //两个矩阵相乘
		Matrix operator *(const double f);          //矩阵乘以常数
		Matrix& operator=(const Matrix& m);
		Matrix Inverse();


	private:
		double *item;        //指向矩阵首元素
		int rowNum;          //矩阵行数
		int colNum;          //矩阵列数

	private:
		//矩阵初等行变换
		//如果j=-1,则对i扩大multiply倍
		//如果j在取值范围内，则将第i行扩大multiply倍加到j行
		void RowSwap(int i, int j, double multiply);
		//交换两行
		void RowSwap(int i, int j);
		void FlowOver();
	};

	//用于存储当前所有对象指针
	std::list<MyGraph* >graphs;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
