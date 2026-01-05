## Roadmap (Milestones)
### Phase 1: 고성능 이미지 뷰어 구현 (Image Viewer)
- [X] 다양한 포맷(JPG, BMP, PNG) 이미지 파일 로드 및 디코딩
- [ ] **GDI+ / CImage**를 활용한 고속 렌더링 구현
- [X] Double Buffering을 적용하여 화면 깜빡임(Flickering) 없는 뷰어 제작
- [X] 화면 크기에 맞춘 비율 유지 리사이징(Aspect Ratio Resizing) 알고리즘 적용

### Phase 2: OpenCV 엔진 연동 (OpenCV Integration)
- [X] **OpenCV 환경 설정** 및 라이브러리 링킹
- [X] `cv::Mat` 데이터 구조와 MFC `CImage`(혹은 `BITMAPINFO`) 간의 상호 변환 인터페이스 구축
- [ ] 기초 영상 처리 필터 적용
    - Grayscale 변환
    - 이진화 (Thresholding) 처리 및 임계값 동적 제어

### Phase 3: 사용자 인터랙션 및 ROI 분석 (Interaction & ROI)
- [X] 마우스 이벤트를 활용한 **관심 영역(ROI, Region of Interest)** 설정 기능
- [X] 좌표계 변환 (Screen Coordinate ↔ Image Coordinate) 구현
- [ ] 선택된 ROI 영역에 대한 실시간 통계 분석 (히스토그램, 평균 밝기 등) 팝업 출력

### Phase 4: 비동기 검사 알고리즘 (Async Inspection)
- [ ] **Multi-threading**을 활용한 UI와 연산 처리의 분리 (Non-blocking UI)
- [ ] 가상의 검사 시나리오 구현 (예: 원형 제품의 엣지 검출 및 불량 판정)
- [ ] 검사 결과 데이터 로그 관리 및 자동 판정 로직 구현