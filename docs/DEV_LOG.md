# 🛠️ 개발 일지 (Development Log)

## 📅 2026-01-05: ROI 기능 구현 및 트러블 슈팅

<details>
<summary> 접기 / 펼치기 </summary>
<div markdown="1">

### ✅ 구현 현황 (Current Status)
* 마우스 드래그를 통한 ROI(관심 영역) 지정 기능 구현 완료.
* `ScreenToClient`와 비율 계산을 통해 화면 좌표를 실제 이미지 좌표로 변환 성공.
* `SetROP2(R2_NOT)` 모드를 활용한 `Rubber-banding` 적용.

### ⚠️ 발견된 문제점 (Known Issues)
1.  **화면 깜박임 (Screen Flickering):**
    * 마우스 드래그 시 `OnMouseMove`에서 사각형을 지우고 다시 그리는 과정이 육안으로 식별되어 화면이 미세하게 깜박임.
    * 직접 DC(Device Context)에 그리는 방식의 한계.

2.  **잔상 문제 (Artifacts):**
    * 새로운 ROI를 지정하려 클릭해도, 이전에 그려진 박스 잔상이 지워지지 않고 그대로 남아있음.
    * 여러 개의 박스가 화면에 겹쳐 보여 혼란을 줌.

### 🚀 개선 계획 (Refactoring Plan)
1.  **더블 버퍼링 (Double Buffering) 도입:**
    * 메모리 DC(`CreateCompatibleDC`)를 생성하여 비트맵에 먼저 그림을 완성한 후, 화면에 한 번에 전송(`BitBlt`)하여 깜박임 제거.

2.  **화면 갱신 로직 개선:**
    * `OnLButtonDown` 이벤트 발생 시 원본 이미지를 강제로 다시 그려(Redraw) 이전 ROI 잔상을 제거.
    * `OnLButtonUp` 이벤트 발생 시 확정된 영역을 실선으로 명확히 표시.
    
</div>
</details>

---

<details>
<summary> 접기 / 펼치기 </summary>
<div markdown="2">

### ✅ 구현 현황 (Current Status)
1.  **더블 버퍼링 (Double Buffering) 도입:**
    * 메모리 DC(`CreateCompatibleDC`)를 생성하여 비트맵에 먼저 그림을 완성한 후, 화면에 한 번에 전송(`BitBlt`)하여 깜박임 제거.

2.  **화면 갱신 로직 개선:**
    * `OnLButtonDown` 이벤트 발생 시 원본 이미지를 강제로 다시 그려(Redraw) 이전 ROI 잔상을 제거.
    * `OnMouseMove` 이벤트 발생 시 확정 후보 영역을 점선으로 명확히 표시.
    * `OnLButtonUp` 이벤트 발생 시 확정된 영역을 실선으로 명확히 표시.

### ⚠️ 발견된 문제점 (Known Issues)

1. 메모리 누수 (Memory Leak)
    * **증상:** 프로그램 종료 시 디버그 출력 창에 `"Detected memory leaks!"` 경고 발생.
    * **원인 추정:** * `OnMouseMove` 이벤트는 마우스 이동 시 초당 수백 번 발생함.
        * 핸들러 내부에서 `DrawMat` 호출 시 `CreateCompatibleBitmap` 등 GDI 객체 생성과 해제 과정에서 누수 발생 추정.
        * 혹은 `OnMouseMove` 내의 임시 객체 처리가 완벽하지 않음.

2. 화면 깜박임 (Screen Flickering)
    * **증상:** 마우스를 드래그할 때 ROI 사각형과 배경 이미지가 미세하게 깜박거림.
    * **원인:** **과도한 갱신:** 마우스가 1픽셀만 움직여도 전체 이미지(`StretchDIBits`)를 화면 DC에 직접 다시 그림.
    * **렌더링 지연:** 무거운 '이미지 그리기' 작업과 가벼운 '사각형 그리기' 작업이 순차적으로 실행되면서 부하가 발생함.

### 🚀 개선 계획 (Refactoring Plan)
1.  **구조 리팩토링 (Structural Refactoring):**
    * 현재 구현된 Double Buffering 로직을 유지하되, 불필요한 임시 객체 생성을 최소화하도록 개선.
</div>
</details>