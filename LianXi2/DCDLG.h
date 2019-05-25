#pragma once


// DCDLG 对话框

class DCDLG : public CDialogEx
{
	DECLARE_DYNAMIC(DCDLG)

public:
	DCDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DCDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DuiChenDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
