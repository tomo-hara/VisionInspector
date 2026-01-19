
// SmartDesktopScannerDlg.h: 헤더 파일
//

#pragma once

// [추가 1] Direct2D 및 OpenCV 헤더
#include <d2d1.h>
#include <opencv2/opencv.hpp>

// CSmartDesktopScannerDlg 대화 상자
class CSmartDesktopScannerDlg : public CDialogEx
{
// 생성입니다.
public:
	CSmartDesktopScannerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// [추가 2] Direct2D 리소스 및 함수 선언
private:
	ID2D1Factory *m_pDirect2dFactory = nullptr;
	ID2D1HwndRenderTarget *m_pRenderTarget = nullptr;
	ID2D1Bitmap *m_pBitmap = nullptr;
	cv::VideoCapture m_VideoCapture; // 웹캠 제어용

	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	void DrawMatToD2D(const cv::Mat &mat);

public:
	// 소멸자 (리소스 해제용) -> 클래스 마법사나 수동으로 추가 필요할 수 있음
	virtual ~CSmartDesktopScannerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMARTDESKTOPSCANNER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
};
