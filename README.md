# VisionInspector 👁️
**MFC와 OpenCV를 활용한 제조 공정용 머신 비전(Machine Vision) 검사 소프트웨어**

## 📝 프로젝트 소개 (Introduction)
**VisionInspector**는 스마트 팩토리 환경을 가정한 윈도우 기반 영상 처리 애플리케이션입니다. 
Microsoft Foundation Class (MFC)의 강력한 UI 제어 능력과 OpenCV의 영상 처리 알고리즘을 결합하여, 실제 산업 현장에서 사용되는 **비전 검사 장비(Vision Inspection System)**의 핵심 기능을 구현하는 것을 목표로 합니다.

## 🛠 개발 환경 (Environment)
* **OS:** Windows 10/11
* **IDE:** Visual Studio 2017
* **Language:** C++ (MSVC)
* **Framework:** MFC (Dialog-based)
* **Library:** OpenCV 4.8

## 📅 개발 로드맵 (Roadmap)
> 이 프로젝트는 아래 4단계(Phase)에 걸쳐 점진적으로 고도화될 예정입니다.
<br>자세한 체크리스트는 [`docs/ROADMAP.md`](docs/ROADMAP.md) 참고

### Phase 1: 고성능 이미지 뷰어 구현 (Image Viewer)
- 다양한 포맷(JPG, BMP, PNG) 이미지 파일 로드 및 디코딩
- **GDI+ / CImage**를 활용한 고속 렌더링 구현
- Double Buffering을 적용하여 화면 깜빡임(Flickering) 없는 뷰어 제작
- 화면 크기에 맞춘 비율 유지 리사이징(Aspect Ratio Resizing) 알고리즘 적용

### Phase 2: OpenCV 엔진 연동 (OpenCV Integration)
- **OpenCV 환경 설정** 및 라이브러리 링킹
- `cv::Mat` 데이터 구조와 MFC `CImage`(혹은 `BITMAPINFO`) 간의 상호 변환 인터페이스 구축
- 기초 영상 처리 필터 적용
    - Grayscale 변환
    - 이진화 (Thresholding) 처리 및 임계값 동적 제어

### Phase 3: 사용자 인터랙션 및 ROI 분석 (Interaction & ROI)
- 마우스 이벤트를 활용한 **관심 영역(ROI, Region of Interest)** 설정 기능
- 좌표계 변환 (Screen Coordinate ↔ Image Coordinate) 구현
- 선택된 ROI 영역에 대한 실시간 통계 분석 (히스토그램, 평균 밝기 등) 팝업 출력

### Phase 4: 비동기 검사 알고리즘 (Async Inspection)
- **Multi-threading**을 활용한 UI와 연산 처리의 분리 (Non-blocking UI)
- 가상의 검사 시나리오 구현 (예: 원형 제품의 엣지 검출 및 불량 판정)
- 검사 결과 데이터 로그 관리 및 자동 판정 로직 구현

## 📂 폴더 구조 (Directory Structure)
*(프로젝트 생성 후 구체화 예정)*

## 변경 이력
- [`CHANGELOG.md`](./docs/CHANGELOG.md)
