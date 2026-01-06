# Changelog
## [Released]
### Added
- 

## [Unreleased]
### Added
- **ROI (Region of Interest) Selection:** 마우스 드래그를 이용한 관심 영역 설정 기능 구현
- **Coordinate Conversion:** 화면 좌표(Screen)와 이미지 좌표(Image) 간의 변환 로직(`GetImageRect`) 구현
- **Double Buffering:** GDI 메모리 DC를 활용하여 화면 깜빡임(Flickering) 방지
- **Visualization:** ROI 상태(Drag/Confirmed)에 따른 점선/실선 시각화

### Fixed
- **Crash on Reload:** 이미지 재로드 시 GDI 객체 초기화 순서 오류로 인한 충돌 해결
- **Coordinate Mismatch:** ROI 사각형이 마우스 포인터와 어긋나는 현상 수정 (Picture Control 기준 좌표 보정)
- **Rendering Failure:** `OnPaint` 리팩토링 시 비트맵 선택 해제 시점 오류로 인한 화면 미출력 현상 수정