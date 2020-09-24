#pragma once


// CByDlg 대화 상자

class CByDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CByDlg)

public:
	CByDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CByDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic mText;
};
