﻿#pragma once


// CFunDlg 대화 상자
#include <opencv2/opencv.hpp>
#include <thread>
using namespace std;
using namespace cv;

class CFunDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFunDlg)

public:
	CFunDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFunDlg();
	void DrawMat();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif
private:
	int Mat2CImage(Mat* mat, CImage& img);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Pic;

	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	BOOL mIndex;
	CSliderCtrl mSlider;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
};
