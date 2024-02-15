
// HeatAnlyzDlg.h: 头文件
//

#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;
// CHeatAnlyzDlg 对话框
class CHeatAnlyzDlg : public CDialogEx
{
// 构造
public:
	CHeatAnlyzDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEATANLYZ_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnLoadBMP;
	CButton m_btnLoadTXT;
	CButton m_btnSaveRST;

	CString m_strBMPFilePath;
	CString m_strTXTFilePath;

	CStatic m_staticBMPFilePath;
	CStatic m_staticTXTFilePath;
	
	Mat BMPimage;
	Mat TXTimage;
	Mat homography;
	std::vector<std::vector<double>> data;

	afx_msg void OnBnClickedButtonLoadBmp();
	afx_msg void OnBnClickedButtonLoadTxt();
	afx_msg void OnBnClickedButtonSaveRst();
	afx_msg void OnBnClickedButtonConfirm();
};
