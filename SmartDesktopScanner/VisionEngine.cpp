#include "pch.h"
#include "VisionEngine.h"


VisionEngine::VisionEngine() { }
VisionEngine::~VisionEngine() { CloseCamera(); }

bool VisionEngine::OpenCamera(int deviceID)
{
	return m_Capture.open(deviceID);
}

void VisionEngine::CloseCamera()
{
	if (m_Capture.isOpened()) m_Capture.release();
}

int VisionEngine::GetWidth() const { return (int)m_Capture.get(cv::CAP_PROP_FRAME_WIDTH); }
int VisionEngine::GetHeight() const { return (int)m_Capture.get(cv::CAP_PROP_FRAME_HEIGHT); }

bool VisionEngine::GetProcessedFrame(cv::Mat & outFrame, bool bGray, bool bCanny)
{
	if (!m_Capture.isOpened()) return false;

    cv::Mat srcMat;
    if (!m_Capture.read(srcMat)) return false;

    // 원본 보호 및 처리용 복제
	outFrame = srcMat.clone();

    // 1. Grayscale
    if (bGray) {
        if (outFrame.channels() != 1) {
            cv::Mat temp;
            cv::cvtColor(outFrame, temp, cv::COLOR_BGR2GRAY);
            outFrame = temp;
        }
    }

    // 2. Canny Edge
    if (bCanny) {
        // Canny는 1채널 필수
        if (outFrame.channels() != 1) {
            cv::Mat temp;
            cv::cvtColor(outFrame, temp, cv::COLOR_BGR2GRAY);
            outFrame = temp;
        }
        cv::Mat edge;
        cv::Canny(outFrame, edge, 50, 150);
        outFrame = edge;
    }

    return true;
}


