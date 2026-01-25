#include "pch.h"
#include "D2DRenderer.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

D2DRenderer::D2DRenderer() { }
D2DRenderer::~D2DRenderer()
{
	DiscardDeviceResources();
	if (m_pTextFormat) m_pTextFormat->Release();
	if (m_pDWriteFactory) m_pDWriteFactory->Release();
	if (m_pFactory) m_pFactory->Release();
}

void D2DRenderer::Draw(const cv::Mat &mat, const CString &overlayText)
{
	if (mat.empty() || FAILED(CreateDeviceResources())) return;

	cv::Mat renderMat;
	if (mat.channels() == 1) cv::cvtColor(mat, renderMat, cv::COLOR_GRAY2BGRA);
	else if (mat.channels() == 3) cv::cvtColor(mat, renderMat, cv::COLOR_BGR2BGRA);
	else renderMat = mat;

	HRESULT hr = S_OK;
	D2D1_SIZE_U size = D2D1::SizeU(renderMat.cols, renderMat.rows);

	if (m_pBitmap) {
		D2D1_SIZE_F existingSize = m_pBitmap->GetSize();
		if (existingSize.width != size.width || existingSize.height != size.height) {
			m_pBitmap->Release(); m_pBitmap = nullptr;
		}
	}

	if (!m_pBitmap) {
		D2D1_BITMAP_PROPERTIES props;
		props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
		props.dpiX = 96.0f;
		props.dpiY = 96.0f;
		hr = m_pRenderTarget->CreateBitmap(size, renderMat.data, static_cast<UINT32>(renderMat.step), &props, &m_pBitmap);
	} else {
		hr = m_pBitmap->CopyFromMemory(NULL, renderMat.data, static_cast<UINT32>(renderMat.step));
	}

	if (FAILED(hr)) return;

	m_pRenderTarget->BeginDraw();

	D2D1_SIZE_F targetSize = m_pRenderTarget->GetSize();
	m_pRenderTarget->DrawBitmap(m_pBitmap, D2D1::RectF(0, 0, targetSize.width, targetSize.height));

	float cx = targetSize.width / 2.0f;
	float cy = targetSize.height / 2.0f;
	if (m_pBrush) {
		m_pRenderTarget->DrawLine(D2D1::Point2F(cx - CROSSHAIR_SIZE, cy), D2D1::Point2F(cx + CROSSHAIR_SIZE, cy), m_pBrush, 2.0f);
		m_pRenderTarget->DrawLine(D2D1::Point2F(cx , cy - CROSSHAIR_SIZE), D2D1::Point2F(cx , cy + CROSSHAIR_SIZE), m_pBrush, 2.0f);

		if (m_pTextFormat) {
			m_pRenderTarget->DrawText(overlayText, overlayText.GetLength(), m_pTextFormat,
				D2D1::RectF(10, 10, 500, 200), m_pBrush);
		}
	}

	if (m_pRenderTarget->EndDraw() == D2DERR_RECREATE_TARGET) {
		DiscardDeviceResources();
	}
}

bool D2DRenderer::Initialize(HWND hWnd)
{
	if (m_pFactory) {
		ReleaseResources();
		m_pFactory->Release();
		m_pFactory = nullptr;
	}
	m_hTargetWnd = hWnd;

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	if (FAILED(hr)) return false;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	if (SUCCEEDED(hr)) {
		m_pDWriteFactory->CreateTextFormat(L"Malgun Gothic", NULL,
			DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			20.0f, L"ko-kr", &m_pTextFormat);
	}
	
	return true;
}

void D2DRenderer::Resize(int width, int height)
{
	if (m_pRenderTarget) {
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

HRESULT D2DRenderer::CreateDeviceResources()
{
	if (m_pRenderTarget) return S_OK;
	if (!m_hTargetWnd) return E_FAIL;

	RECT rc;
	::GetClientRect(m_hTargetWnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	HRESULT hr = m_pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_hTargetWnd, size),
		&m_pRenderTarget);

	if (SUCCEEDED(hr)) {
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_pBrush);
	}

	return hr;
}

void D2DRenderer::ReleaseResources()
{
	DiscardDeviceResources();
}

void D2DRenderer::DiscardDeviceResources()
{
	if (m_pBitmap) { m_pBitmap->Release(); m_pBitmap = nullptr; }
	if (m_pBrush) { m_pBrush->Release(); m_pBrush = nullptr; }
	if (m_pRenderTarget) { m_pRenderTarget->Release(); m_pRenderTarget = nullptr; }
}
