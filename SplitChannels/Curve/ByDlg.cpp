// ByDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Curve.h"
#include "ByDlg.h"
#include "afxdialogex.h"


// CByDlg 대화 상자

IMPLEMENT_DYNAMIC(CByDlg, CDialogEx)

CByDlg::CByDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CByDlg::~CByDlg()
{
}

void CByDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CByDlg, CDialogEx)
END_MESSAGE_MAP()


// CByDlg 메시지 처리기
