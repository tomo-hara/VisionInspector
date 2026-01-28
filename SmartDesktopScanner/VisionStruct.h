/**
 * @file VisionStruct.h
 * @brief 프로젝트 전반에서 사용하는 ROI 및 검사 관련 데이터 구조체 정의
 */
#pragma once
#include <opencv2/opencv.hpp>
#include <string>

enum class EdgeOrientation {
	Horizontal,
	Vertical,
	Any
};

struct LinePairSettings {
	double expectedWidth = 100.0;
	double tolerance = 20.0;
	int threshold = 50;
	bool isDarkToBright = true;
};

struct GroupSettings {
	std::string groupName = "DefaultGroup";

	cv::Point2f ptCenter;
	cv::Point2f ptLeftTop;
	cv::Point2f ptRightBottom;

	float ratioBase = 0.5f;
	float ratioSub = 0.75f;

	void UpdateFromRect(const cv::Rect &rect) {
		ptLeftTop = cv::Point2f((float)rect.x, (float)rect.y);
		ptRightBottom = cv::Point2f((float)(rect.x + rect.width), (float)(rect.y + rect.height));
		ptCenter = (ptLeftTop + ptRightBottom) * 0.5f;
	}
};

struct VisionROI {
	int id = -1;
	std::string name;
	cv::Rect rect;
	bool isSelected = false;

	EdgeOrientation orientation = EdgeOrientation::Horizontal;
	LinePairSettings linePairInput;
	GroupSettings group;

	VisionROI(int _id, cv::Rect _rect, std::string _name = "ROI")
		: id(_id), rect(_rect), name(_name)
	{
		group.UpdateFromRect(_rect);
	}
};