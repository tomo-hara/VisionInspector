/**
 * @file VisionEngine.h
 * @brief 웹캠 제어 및 OpenCV 영상 처리 알고리즘을 담당하는 클래스 헤더
 * @author tomo
 * @date 2026-01-29
 * * @details
 * - cv::VideoCapture를 래핑하여 하드웨어 의존성을 캡슐화함
 * - Grayscale, Canny Edge 등 필터 연산을 수행하고 결과를 반환
 * - UI 코드(MFC)와 완전히 분리되어야 함
 */
#pragma once

#include <opencv2/opencv.hpp>

class VisionEngine
{
public:
	VisionEngine();
	~VisionEngine();

	bool OpenCamera(int deviceID = 0);
	void CloseCamera();

	int GetWidth() const;
	int GetHeight() const;

	// 현재 프레임 가져오기 (필터 적용 옵션 포함)
	bool GetProcessedFrame(cv::Mat &outFrame, bool bGray, bool bCanny);

private:
	cv::VideoCapture m_Capture;
	cv::Mat m_FrameBuffer;
	cv::Mat m_template;
};

