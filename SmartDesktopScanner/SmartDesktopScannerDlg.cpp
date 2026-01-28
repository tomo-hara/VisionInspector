
// SmartDesktopScannerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SmartDesktopScanner.h"
#include "SmartDesktopScannerDlg.h"
#include "afxdialogex.h"

// [추가] 로깅 제어용 헤더
#include <opencv2/core/utils/logger.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "d2d1.lib") // Direct2D 라이브러리 링크
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "opencv_core480d.lib")
#pragma comment(lib, "opencv_video480d.lib")
#pragma comment(lib, "opencv_videoio480d.lib")
#pragma comment(lib, "opencv_imgcodecs480d.lib")
#pragma comment(lib, "opencv_imgproc480d.lib")

// CSmartDesktopScannerDlg 대화 상자

CSmartDesktopScannerDlg::CSmartDesktopScannerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMARTDESKTOPSCANNER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSmartDesktopScannerDlg::~CSmartDesktopScannerDlg()
{
	m_Engine.CloseCamera();
	m_Renderer.ReleaseResources();
}

void CSmartDesktopScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_wndView);
}

BEGIN_MESSAGE_MAP(CSmartDesktopScannerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSmartDesktopScannerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSmartDesktopScannerDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_GRAY_BTN, &CSmartDesktopScannerDlg::OnBnClickedGrayBtn)
	ON_BN_CLICKED(IDC_CANNY_BTN, &CSmartDesktopScannerDlg::OnBnClickedCannyBtn)
	ON_BN_CLICKED(IDC_REGISTER_BTN, &CSmartDesktopScannerDlg::OnBnClickedRegisterBtn)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CSmartDesktopScannerDlg 메시지 처리기

BOOL CSmartDesktopScannerDlg::OnInitDialog()
{
	// [추가] OpenCV 내부 로깅 및 추적 끄기 (거짓 메모리 누수 차단)
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	if (!m_Renderer.Initialize(m_wndView.GetSafeHwnd())) {
		AfxMessageBox(L"Renderer Init Failed!");
		return FALSE;
	}

	if (m_Engine.OpenCamera(0)) {
		SetTimer(1001, 33, NULL);
	} else {
		AfxMessageBox(L"Camera Init Failed!");
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSmartDesktopScannerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSmartDesktopScannerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSmartDesktopScannerDlg::OnBnClickedOk() { /*CDialogEx::OnOK();*/ }
void CSmartDesktopScannerDlg::OnBnClickedCancel() {	CDialogEx::OnCancel(); }

void CSmartDesktopScannerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1001) {
		cv::Mat frame;
		if (m_Engine.GetProcessedFrame(frame, m_bUseGrayscale, m_bUseCanny)) {
			CString strInfo;
			strInfo.Format(L"Smart Scanner v2.1 (Refactored)\nRes: %3d x %3d\nFilter: %s",
				frame.cols, frame.rows,
				m_bUseCanny ? L"Canny" : (m_bUseGrayscale ? L"Gray" : L"None"));

			/*m_Renderer.Draw(frame, strInfo);*/
			m_Renderer.Draw(frame, strInfo, m_ROIs, m_bIsDragging, m_ptDragStart, m_ptDragCurrent);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

// 깜빡임 방지
BOOL CSmartDesktopScannerDlg::OnEraseBkgnd(CDC *pDC)
{
	return TRUE; // 배경 지우기 생략

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CSmartDesktopScannerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (!m_wndView.GetSafeHwnd()) return;

	int nButtonAreaHeight = 50;
	int nViewHeight = std::max(10, cy - nButtonAreaHeight);

	m_wndView.MoveWindow(0, 0, cx, nViewHeight);

	CWnd *pBtnGray = GetDlgItem(IDC_GRAY_BTN);
	if (pBtnGray) {
		pBtnGray->MoveWindow(10, cy - 40, 100, 30);
	}
	CWnd *pBtnCanny = GetDlgItem(IDC_CANNY_BTN);
	if (pBtnCanny) {
		pBtnCanny->MoveWindow(120, cy - 40, 100, 30);
	}

	m_Renderer.Resize(cx, nViewHeight);
}

void CSmartDesktopScannerDlg::OnBnClickedGrayBtn()
{
	m_bUseGrayscale = !m_bUseGrayscale;
	// 현재 구조상 Canny 우선이므로
	// Canny 처리하려면 `m_bUseCanny = false; 추가`

	// [디버깅용] 출력창(Output Window)에서 확인
	TRACE(_T("Gray Mode Clicked! Current State: %d\n"), m_bUseGrayscale);
}
void CSmartDesktopScannerDlg::OnBnClickedCannyBtn() { m_bUseCanny = !m_bUseCanny; }

void CSmartDesktopScannerDlg::OnBnClickedRegisterBtn()
{
	if (m_ROIs.empty()) {
		AfxMessageBox(L"먼저 마우스로 ROI를 그리시오.");
		return;
	}
}

cv::Rect CSmartDesktopScannerDlg::ScreenToImageRect(CRect screenRect)
{
	CRect rcView;
	m_wndView.GetClientRect(&rcView);
	if (rcView.IsRectEmpty()) return cv::Rect(0, 0, 0, 0);

	int nImgWidth = m_Engine.GetWidth();
	int nImgHeight = m_Engine.GetHeight();
	if (nImgWidth <= 0 || nImgHeight <= 0) return cv::Rect(0, 0, 0, 0);

	double scaleX = (double)nImgWidth / rcView.Width();
	double scaleY = (double)nImgHeight / rcView.Height();

	int x = (int)(screenRect.left * scaleX);
	int y = (int)(screenRect.top * scaleY);
	int w = (int)(screenRect.Width() * scaleX);
	int h = (int)(screenRect.Height() * scaleY);

	x = std::max(0, std::min(x, nImgWidth));
	y = std::max(0, std::min(y, nImgHeight));
	w = std::min(w, nImgWidth - x);
	h = std::min(h, nImgHeight - y);

	return cv::Rect(x, y, w, h);
}

void CSmartDesktopScannerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rcView;
	m_wndView.GetWindowRect(&rcView);
	ScreenToClient(&rcView);

	if (rcView.PtInRect(point)) {
		m_bIsDragging = true;
		SetCapture();

		m_ptDragStart = cv::Point(point.x - rcView.left, point.y - rcView.top);
		m_ptDragCurrent = m_ptDragStart;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CSmartDesktopScannerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsDragging)
	{
		CRect rcView;
		m_wndView.GetWindowRect(&rcView);
		ScreenToClient(&rcView);

		int x = std::max(rcView.left, std::min((long)point.x, rcView.right));
		int y = std::max(rcView.top, std::min((long)point.y, rcView.bottom));

		m_ptDragCurrent = cv::Point(x - rcView.left, y - rcView.top);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CSmartDesktopScannerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsDragging)
	{
		ReleaseCapture();
		m_bIsDragging = false;

		CRect screenRect(
			std::min(m_ptDragStart.x, m_ptDragCurrent.x),
			std::min(m_ptDragStart.y, m_ptDragCurrent.y),
			std::max(m_ptDragStart.x, m_ptDragCurrent.x),
			std::max(m_ptDragStart.y, m_ptDragCurrent.y)
		);

		if (screenRect.Width() > 10 && screenRect.Height() > 10)
		{
			cv::Rect imgRect = ScreenToImageRect(screenRect);

			int newID = (int)m_ROIs.size() + 1;
			VisionROI newROI(newID, imgRect, "ROI_" + std::to_string(newID));

			newROI.group.UpdateFromRect(imgRect);

			m_ROIs.push_back(newROI);

			TRACE("New ROI Added: [ID:%d] Rect(%d,%d,%d,%d)\n", newID, imgRect.x, imgRect.y, imgRect.width, imgRect.height);
		}
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}
