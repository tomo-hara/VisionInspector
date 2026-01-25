r
// SmartDesktopScannerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SmartDesktopScanner.h"
#include "SmartDesktopScannerDlg.h"
#include "afxdialogex.h"

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
	// 리소스 정리
	DiscardDeviceResources();
	if (m_pDWriteFactory) {
		m_pDWriteFactory->Release();
		m_pDWriteFactory = nullptr;
	}
	if (m_pDirect2dFactory) {
		m_pDirect2dFactory->Release();
		m_pDirect2dFactory = nullptr;
	}
	if (m_VideoCapture.isOpened()) {
		m_VideoCapture.release();
	}
}

void CSmartDesktopScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_wndView);
}

//inline void ConvertToGray(const cv::Mat &mat)
//{
//	cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
//}

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
END_MESSAGE_MAP()

// CSmartDesktopScannerDlg 메시지 처리기

BOOL CSmartDesktopScannerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// Direct2D Factory 생성
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	if (FAILED(hr)) {
		AfxMessageBox(_T("Direct2D Factory 생성 실패!"));
		return FALSE;
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	if (SUCCEEDED(hr)) {
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Malgun Gothic", NULL,
			DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			20.0f, L"ko-kr", &m_pTextFormat
		);
	}

	// 웹캠 연결 시도 (0번 카메라)
	if (m_VideoCapture.open(0)) {
		// 타이머 설정 (33ms = 약 30fps)
		SetTimer(1001, 33, NULL);
	}
	else {
		AfxMessageBox(_T("웹캠을 찾을 수 없습니다."));
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

// [3] 리소스 관리 함수
HRESULT CSmartDesktopScannerDlg::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (!m_pRenderTarget) {
		//RECT rc;
		//GetClientRect(&rc);
		RECT rc;
		m_wndView.GetClientRect(&rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_wndView.GetSafeHwnd(), size),
			&m_pRenderTarget
		);

		if (SUCCEEDED(hr)) {
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_pBrushYellow);
		}
	}
	return hr;
}void CSmartDesktopScannerDlg::DiscardDeviceResources()
{
    if (m_pBitmap) { m_pBitmap->Release(); m_pBitmap = nullptr; }
    if (m_pBrushYellow) { m_pBrushYellow->Release(); m_pBrushYellow = nullptr; }
    if (m_pRenderTarget) { m_pRenderTarget->Release(); m_pRenderTarget = nullptr; }
}

// [4] 그리기 로직 (핵심)
void CSmartDesktopScannerDlg::DrawMatToD2D(const cv::Mat & srcMat)
{
    if (srcMat.empty()) return;
	if (!m_wndView.GetSafeHwnd()) return;
	// 원본 보호를 위한 복제본 생성
	cv::Mat matProc = srcMat.clone();

	try {
		//// 필터 적용
		//ProcessImage(srcMat);

		// (A) 그레이스케일 처리
		if (m_bUseGrayscale) {
			// [핵심 수정] 3채널 -> 1채널 변환 시 '제자리 변환(In-place)'을 피함
			if (matProc.channels() != 1) {
				cv::Mat tempGray;
				cv::cvtColor(matProc, tempGray, cv::COLOR_BGR2GRAY);
				matProc = tempGray; // 변환 후 교체
			}
		}

		// (B) Canny Edge 처리
		if (m_bUseCanny) {
			// Canny는 1채널 입력이 필수
			if (matProc.channels() != 1) {
				cv::Mat tempGray;
				cv::cvtColor(matProc, tempGray, cv::COLOR_BGR2GRAY);
				matProc = tempGray;
			}

			// Canny 수행 (결과도 1채널)
			cv::Mat tempEdge;
			cv::Canny(matProc, tempEdge, 50, 150);
			matProc = tempEdge; // 결과 교체
		}
	}
	catch (cv::Exception &e) {
		TRACE(L"OpenCV Error: %S\n", e.what());
		return;
	}

    if (FAILED(CreateDeviceResources())) return;


    // BGR -> BGRA 변환 (Direct2D 호환)
    cv::Mat renderMat;
	if (srcMat.channels() == 1) {
		cv::cvtColor(matProc, renderMat, cv::COLOR_GRAY2BGRA);
	} else if (srcMat.channels() == 3) {
		cv::cvtColor(matProc, renderMat, cv::COLOR_BGR2BGRA);
	} else {
		renderMat = matProc;
	}

    HRESULT hr = S_OK;
    D2D1_SIZE_U size = D2D1::SizeU(renderMat.cols, renderMat.rows);

    // 비트맵 생성 또는 업데이트
    if (m_pBitmap) {
        D2D1_SIZE_F existingSize = m_pBitmap->GetSize();
        if (existingSize.width != size.width || existingSize.height != size.height) {
            m_pBitmap->Release();
            m_pBitmap = nullptr;
        }
    }

    if (!m_pBitmap) {
        D2D1_BITMAP_PROPERTIES props;
        props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
        props.dpiX = 96.0f; props.dpiY = 96.0f;
        hr = m_pRenderTarget->CreateBitmap(size, renderMat.data, static_cast<UINT32>(renderMat.step), &props, &m_pBitmap);
    } else {
        hr = m_pBitmap->CopyFromMemory(NULL, renderMat.data, static_cast<UINT32>(renderMat.step));
    }

    if (FAILED(hr)) return;

    // 렌더링
    m_pRenderTarget->BeginDraw();
	D2D1_SIZE_F targetSize = m_pRenderTarget->GetSize();
    D2D1_RECT_F rect = D2D1::RectF(0, 0, targetSize.width, targetSize.height);
    m_pRenderTarget->DrawBitmap(m_pBitmap, rect);

	// 화면 중앙 격자 그리기
	float cx = targetSize.width / 2.0f;
	float cy = targetSize.height / 2.0f;
	m_pRenderTarget->DrawLine(D2D1::Point2F(cx - 20, cy), D2D1::Point2F(cx + 20, cy), m_pBrushYellow, 2.0f);
	m_pRenderTarget->DrawLine(D2D1::Point2F(cx, cy - 20), D2D1::Point2F(cx, cy + 20), m_pBrushYellow, 2.0f);

	if (m_pTextFormat && m_pBrushYellow) {
		CString strInfo;
		strInfo.Format(L"Smart Scanner v1.0\nResolution : %dx%d\nnFilter: %s",
			srcMat.cols, srcMat.rows,
			m_bUseCanny ? L"Canny Edge" : m_bUseGrayscale ? L"Grayscale" : L"Original");

		m_pRenderTarget->DrawTextW(
			strInfo,
			strInfo.GetLength(),
			m_pTextFormat,
			D2D1::RectF(10, 10, 500, 200), // LT
			m_pBrushYellow
		);
	}

    hr = m_pRenderTarget->EndDraw();

    if (hr == D2DERR_RECREATE_TARGET) DiscardDeviceResources();
}

void CSmartDesktopScannerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1001) {
		cv::Mat frame;
		m_VideoCapture >> frame;
		if (!frame.empty()) {
			DrawMatToD2D(frame);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

// [5] 깜빡임 방지
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
	int nViewHeight = cy - nButtonAreaHeight;

	if (nViewHeight < 10) nViewHeight = 10;

	m_wndView.MoveWindow(0, 0, cx, nViewHeight);

	if (m_pRenderTarget) {
		D2D1_SIZE_U size = D2D1::SizeU(cx, nViewHeight);
		m_pRenderTarget->Resize(size);
	}

	CWnd *pBtnGray = GetDlgItem(IDC_GRAY_BTN);
	if (pBtnGray) {
		pBtnGray->MoveWindow(10, cy - 40, 100, 30);
	}
}

void CSmartDesktopScannerDlg::OnBnClickedGrayBtn()
{
	m_bUseGrayscale = !m_bUseGrayscale;
	// 현재 구조상 Canny 우선이므로
	// Canny 처리하려면 `m_bUseCanny = false; 추가`

	// [디버깅용] 출력창(Output Window)에서 확인
	TRACE(_T("Gray Mode Clicked! Current State: %d\n"), m_bUseGrayscale);
}

void CSmartDesktopScannerDlg::OnBnClickedCannyBtn()
{
	m_bUseCanny = !m_bUseCanny;
}


void CSmartDesktopScannerDlg::ProcessImage(const cv::Mat &mat)
{
	if (mat.empty()) return;

	// 1. 흑백 변환
	if (m_bUseGrayscale) {
		if (mat.channels() == 3) {
			cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
		}
	}

	// 2. 엣지 검출
	if (m_bUseCanny) {
		if (mat.channels() == 3) {
			cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
		}
		cv::Canny(mat, mat, 50, 150);
	}
}