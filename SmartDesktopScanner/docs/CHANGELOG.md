# Changelog
## [Released]
### Added
- 

## [Unreleased]

## [v2.1.0] - 2026-01-29
### Added
- **Feature**: 마우스 드래그를 이용한 **ROI(관심 영역) 지정** 기능 추가
- **Structure**: 프로젝트 전반에서 공유되는 데이터 구조체 정의 (`VisionStruct.h`)
- **UI**: ROI 등록 및 추적 상태를 시각화하는 렌더링 로직 추가 (Rect Drawing)

### Changed
- **Renderer**: `Draw` 함수가 `VisionROI` vector와 드래그 상태를 처리하도록 확장됨

## [v2.0.0] - 2026-01-26
### Added
- **Refactoring**: MVC 패턴 적용 (VisionEngine, D2DRenderer 분리)
- **Feature**: DirectWrite를 이용한 실시간 출력 구현

## [v1.0.0] - 2026-01-20
### Added
- 최초 구현: OpenCV + Direct2D 연동 성공
