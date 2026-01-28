/**
 * @file D2DRenderer.h
 * @brief Direct2D 및 DirectWrite 리소스를 관리하고 렌더링을 전담하는 클래스
 * @details
 * - MFC 윈도우 핸들(HWND)을 받아 해당 영역에 그리기 수행
 * - OpenCV Mat 데이터를 Direct2D Bitmap으로 변환하여 출력
 */
#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <opencv2/opencv.hpp>
#include "VisionStruct.h"

#define CROSSHAIR_SIZE 20

class D2DRenderer
{
public:
	D2DRenderer();
	~D2DRenderer();

	bool Initialize(HWND hWnd);
	void Resize(int width, int height);
	void ReleaseResources();

	// OpenCV Mat 데이터를 Direct2D로 그리는 함수
	/*void Draw(const cv::Mat &mat, const CString &overlayText);*/
	void Draw(const cv::Mat &mat, const CString &overlayText,
		const std::vector<VisionROI> &rois,
		bool isDragging, cv::Point dragStart, cv::Point dragCurr);
	

private:
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	
	HWND m_hTargetWnd = nullptr;
	ID2D1Factory *m_pFactory = nullptr;
	ID2D1HwndRenderTarget *m_pRenderTarget = nullptr;
	ID2D1Bitmap *m_pBitmap = nullptr;
	// �������� �۾��� ���� Ŭ����
	ID2D1SolidColorBrush *m_pBrush = nullptr;
	IDWriteFactory *m_pDWriteFactory = nullptr;
	IDWriteTextFormat *m_pTextFormat = nullptr;
};

