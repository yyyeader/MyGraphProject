#pragma once


// ColorAndWidthDLG 对话框

class ColorAndWidthDLG : public CDialogEx
{
	DECLARE_DYNAMIC(ColorAndWidthDLG)

public:
	ColorAndWidthDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ColorAndWidthDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = ColorAndWidth };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCColorButton GColor;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedMfccolorbutton1();
};
