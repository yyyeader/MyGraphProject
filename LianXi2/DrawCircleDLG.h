#pragma once


// DrawCircleDLG 对话框

class DrawCircleDLG : public CDialogEx
{
	DECLARE_DYNAMIC(DrawCircleDLG)

public:
	DrawCircleDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DrawCircleDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = CircleDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMidcircle();
	afx_msg void OnBnClickedBreshhamcircle();
	afx_msg void OnBnClickedZfcircle();
	afx_msg void OnBnClickedMdrawcircle();
};
