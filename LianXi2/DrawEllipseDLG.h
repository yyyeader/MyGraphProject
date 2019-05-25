#pragma once


// DrawEllipseDLG 对话框

class DrawEllipseDLG : public CDialogEx
{
	DECLARE_DYNAMIC(DrawEllipseDLG)

public:
	DrawEllipseDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DrawEllipseDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = EllipseDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void OnBnClickedDrawellipse();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBreshhamellipse();
	afx_msg void OnBnClickedMdrawellipse();
};
