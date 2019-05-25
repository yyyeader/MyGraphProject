#pragma once


// TCDLG 对话框

class TCDLG : public CDialogEx
{
	DECLARE_DYNAMIC(TCDLG)

public:
	TCDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TCDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = TianChongDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCColorButton TianChongColor;
	afx_msg void OnBnClickedOk();
};
