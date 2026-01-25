/**
 * @file SmartDesktopScannerDlg.h
 * @brief 애플리케이션의 메인 다이얼로그 (View & Controller 역할)
 * @details
 * - VisionEngine(Model)과 D2DRenderer(View)를 소유하고 관리
 * - 사용자 입력(버튼, 리사이징) 이벤트를 처리
 */

#pragma once

#include <d2d1.h>
#include <algorithm>

#include "D2DRenderer.h"
#include "VisionEngine.h"

// CSmartDesktopScannerDlg 대화 상자
class CSmartDesktopScannerDlg : public CDialogEx
{
// 생성입니다.
public:
	CSmartDesktopScannerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

private:
	//==============================//
	/*기존 코드*/
	//ID2D1Factory *m_pDirect2dFactory = nullptr;
	//ID2D1HwndRenderTarget *m_pRenderTarget = nullptr;
	//ID2D1Bitmap *m_pBitmap = nullptr;
	//cv::VideoCapture m_VideoCapture; // 웹캠 제어용
	//ID2D1SolidColorBrush *m_pBrushYellow = nullptr;
	//IDWriteFactory *m_pDWriteFactory = nullptr;
	//IDWriteTextFormat *m_pTextFormat = nullptr;
	/*대체 코드*/
	// 핵심 모듈 개체
	D2DRenderer m_Renderer;
	VisionEngine m_Engine;
	//==============================//

	bool m_bUseGrayscale = false;
	bool m_bUseCanny = false;
	CStatic m_wndView;

	//==============================//
	/*기존 코드*/
	//HRESULT CreateDeviceResources();
	//void DiscardDeviceResources();
	//void DrawMatToD2D(const cv::Mat &mat);
	//void ProcessImage(const cv::Mat &mat);
	//==============================//

public:
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedGrayBtn();
	afx_msg void OnBnClickedCannyBtn();
};
