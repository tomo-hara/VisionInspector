# Architecture Design - Smart Desktop Scanner

## 1. Overview
본 프로젝트는 **SoC (Separation of Concerns)** 원칙을 따르며, UI(MFC), 렌더링(Direct2D), 비전 로직(OpenCV)을 명확히 분리하여 유지보수성과 확장성을 확보합니다.

## 2. Module Structure

### 🖥️ UI Layer (View)
- **Class**: `CSmartDesktopScannerDlg`
- **Role**: 
  - 사용자 입력(버튼 클릭, 리사이징) 수신
  - `VisionEngine`과 `D2DRenderer` 간의 데이터 흐름 조율
  - **직접적인 Direct2D API 호출이나 OpenCV 알고리즘 수행 금지**

### 🎨 Rendering Layer (Presenter)
- **Class**: `D2DRenderer`
- **Role**:
  - Direct2D Factory 및 RenderTarget 수명 주기 관리
  - `cv::Mat` 데이터를 `ID2D1Bitmap`으로 변환 및 고속 렌더링
  - DirectWrite를 이용한 OSD(On-Screen Display) 처리

### 🧠 Core Layer (Model)
- **Class**: `VisionEngine`
- **Role**:
  - 웹캠 하드웨어 제어 (`cv::VideoCapture`)
  - 영상 처리 알고리즘 수행 (Grayscale, Canny, Blur 등)
  - 순수 C++ 및 OpenCV 의존성만 가지며, **UI/Windows API 의존성 최소화**

### 📂 Shared Data Structures
- **File**: `VisionStruct.h`
- **Role**:
  - `VisionROI`, `GroupSettings` 등 모듈 간(View <-> Model) 데이터 교환을 위한 공용 구조체 정의
  - 헤더 의존성을 낮추고 데이터 일관성을 유지

## 3. Data Flow
1. **Timer Tick** (33ms) -> Dialog
2. Dialog calls `VisionEngine::GetProcessedFrame()`
3. Engine captures & processes image -> returns `cv::Mat`
4. Dialog passes `cv::Mat` to `D2DRenderer::Draw()`
5. Renderer converts & draws to HWND

## 3. Data Flow
1. **User Interaction**: 사용자가 마우스로 ROI 드래그 -> `CSmartDesktopScannerDlg`가 좌표 변환(`ScreenToImageRect`) 후 `VisionROI` 생성
2. **Timer Tick** (33ms) -> Dialog
3. Dialog calls `VisionEngine::GetProcessedFrame()`
4. **Processing**: `VisionEngine`이 영상 획득 및 필터/매칭 연산 수행
5. Dialog passes `cv::Mat` & `ROI List` to `D2DRenderer::Draw()`
6. Renderer converts & draws Image + Overlays (ROI, Tracking Box) to HWND