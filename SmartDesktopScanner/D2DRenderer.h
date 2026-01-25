/**
 * @file D2DRenderer.h
 * @brief Direct2D �� DirectWrite ���ҽ��� �����ϰ� �������� �����ϴ� Ŭ����
 * @details
 * - MFC ������ �ڵ�(HWND)�� �޾� �ش� ������ ���� ����� ����
 * - OpenCV Mat �����͸� Direct2D Bitmap���� ��ȯ�Ͽ� ���
 */

#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <opencv2/opencv.hpp>

#define CROSSHAIR_SIZE 20

class D2DRenderer
{
public:
	D2DRenderer();
	~D2DRenderer();

	// �ʱ�ȭ �� ���ҽ� ����
	bool Initialize(HWND hWnd);
	void Resize(int width, int height);
	void ReleaseResources();

	// �׸��� �Լ�
	void Draw(const cv::Mat &mat, const CString &overlayText);

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

