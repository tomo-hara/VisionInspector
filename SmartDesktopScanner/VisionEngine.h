/**
 * @file VisionEngine.h
 * @brief ��ķ ���� �� OpenCV ���� ó�� �˰������� ����ϴ� Ŭ���� ���
 * @author tomo
 * @date 2026-01-25
 * * @details
 * - cv::VideoCapture�� �����Ͽ� �ϵ���� �������� ĸ��ȭ��
 * - Grayscale, Canny Edge �� ���� ������ �����ϰ� ����� ��ȯ
 * - UI �ڵ�(MFC)�� ������ �и��Ǿ�� ��
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

	// ���� ������ �ε� (���� ���� ����)
	bool GetProcessedFrame(cv::Mat &outFrame, bool bGray, bool bCanny);

private:
	cv::VideoCapture m_Capture;
	cv::Mat m_FrameBuffer;
};

