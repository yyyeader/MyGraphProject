#pragma once


// DrawLineDLG 对话框

class DrawLineDLG : public CDialogEx
{
	DECLARE_DYNAMIC(DrawLineDLG)

public:
	DrawLineDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DrawLineDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = LineDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDrawline();
	afx_msg void OnBnClickedMidline();
	afx_msg void OnBnClickedBreshham();
	afx_msg void OnBnClickedMdrawline();
};
