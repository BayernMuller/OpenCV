// FunDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Curve.h"
#include "FunDlg.h"
#include "afxdialogex.h"


// CFunDlg 대화 상자
#define WIN_SIZE 600
IMPLEMENT_DYNAMIC(CFunDlg, CDialogEx)

CFunDlg::CFunDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, mIndex(FALSE), mFlag(true), mCap(0), mThread(nullptr)
{
	
}

CFunDlg::~CFunDlg()
{
	if (mThread)
	{
		mFlag = false;
		mThread->join();
		delete mThread;
	}
	mCap.release();
}

void CFunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_Pic);
	DDX_Control(pDX, IDC_SLIDER2, mSlider);
}


BEGIN_MESSAGE_MAP(CFunDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &CFunDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CFunDlg::OnBnClickedStop)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO4, &CFunDlg::OnRadio)
END_MESSAGE_MAP()


// CFunDlg 메시지 처리기


BOOL CFunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	mSlider.SetRange(-100, 100);
	mSlider.SetPos(1);
	mCap.set(CAP_PROP_FRAME_WIDTH, 320);
	mCap.set(CAP_PROP_FRAME_HEIGHT, 240);
	mThread = new thread(threadFunc, this);
	return TRUE;
}


void CFunDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDown(nFlags, point);
}



void CFunDlg::DrawMat()
{
	CDC* pDC = m_Pic.GetDC();
	CRect rect;
	m_Pic.GetClientRect(rect);
	CImage img;
	Mat2CImage(&mImg, img);
	img.StretchBlt(pDC->m_hDC, 0, 0, rect.Width(), rect.Height());
}

int CFunDlg::Mat2CImage(Mat* mat, CImage& img)
{
	if (!mat || mat->empty())
		return -1;
	int nBPP = mat->channels() * 8;
	img.Create(mat->cols, mat->rows, nBPP);
	if (nBPP == 8)
	{
		static RGBQUAD pRGB[256];
		for (int i = 0; i < 256; i++)
			pRGB[i].rgbBlue = pRGB[i].rgbGreen = pRGB[i].rgbRed = i;
		img.SetColorTable(0, 256, pRGB);
	}
	uchar* psrc = mat->data;
	uchar* pdst = (uchar*)img.GetBits();
	int imgPitch = img.GetPitch();
	for (int y = 0; y < mat->rows; y++)
	{
		memcpy(pdst, psrc, mat->cols * mat->channels());//mat->step is incorrect for those images created by roi (sub-images!)
		psrc += mat->step;
		pdst += imgPitch;
	}
	return 0;
}

void CFunDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		
	}
}

void CFunDlg::OnBnClickedStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CFunDlg::OnBnClickedStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFunDlg::OnRadio(UINT id)
{
	mIndex = id - IDC_RADIO1;
}

unsigned int CFunDlg::threadFunc(CFunDlg* dlg)
{
	while (dlg->mFlag)
	{
		int index = dlg->mIndex;
		int bright = dlg->mSlider.GetPos();
		Mat& img = dlg->mImg;
		dlg->mCap.read(img);
		if (index)
		{
			Mat bgr[3];
			split(img, bgr);
			img = bgr[2 - (index - 1)];
		}
		img.convertTo(img, -1, 1, bright);
		CString str;
		str.Format(L"%d\n", bright);
		TRACE(str);
		dlg->DrawMat();
		this_thread::sleep_for(chrono::milliseconds(200));
		
	}
	return 0;
}

