# 🛠️ 트러블슈팅 및 환경 설정 가이드 (Troubleshooting & Setup Guide)

본 문서는 `VisionInspector` 프로젝트 개발 초기 환경 설정 과정에서 발생한 OpenCV 연동 오류와 그 해결 과정을 기록합니다.

## 📌 개발 환경 (Environment)
* **IDE:** Visual Studio 2017 (v141)
* **Platform:** x86
* **Library:** OpenCV 4.8.0 (Custom Build / Modular)

---

## 1. 링크 오류 (LNK2019, LNK2001)

### 🔴 증상 (Symptom)
빌드 시 다음과 같은 '외부 기호(Unresolved external symbol)' 오류가 다수 발생함.
> `LNK2019: unresolved external symbol "void __cdecl cv::fastFree(void *)" ...`
> `LNK2001: unresolved external symbol ...`

### 🔍 원인 (Cause)
1. **플랫폼 불일치:** 프로젝트는 기본 `x86`으로 설정되어 있었으나, OpenCV 4.x 이상은 `x64`만 지원함.
2. **라이브러리 불일치:** NuGet 패키지 제거 후 수동 설정을 진행하면서 `opencv_world480d.lib`를 링크했으나, 설치된 OpenCV는 모듈 분리형(core, imgproc 등)이라 해당 통합 파일이 존재하지 않음.

### ✅ 해결 (Solution)
1. **플랫폼 변경:** Visual Studio 상단 구성 관리자에서 `x86` → **`x64`**로 변경.
2. **링커 종속성 수정:**
   * **[프로젝트 속성]** > **[링커]** > **[입력]** > **[추가 종속성]**
   * 존재하지 않는 `opencv_world` 대신 실제 존재하는 모듈 라이브러리 목록을 명시.
   ```text
   opencv_core480d.lib
   opencv_imgcodecs480d.lib
   opencv_highgui480d.lib
   opencv_imgproc480d.lib

   ```

---

## 2. 헤더 포함 경로 오류 (C1083)

### 🔴 증상 (Symptom)
빌드 시 다음과 같은 'Fatal Error C1083' 오류가 발생함.
> `C1083: Cannot open include file: 'opencv2/opencv_modules.hpp': No such file or directory`

### 🔍 원인 (Cause)
1. **플랫폼 불일치:** 프로젝트는 기본 `x64`으로 설정되어 있었으나, 설치된 OpenCV 파일은 `x86`만 지원함.
- 포함 디렉터리 경로 설정 실수:
    - 설정한 경로: C:\opencv4.8
    - 소스 코드: #include <opencv2/opencv.hpp>
    - 컴파일러 해석: C:\opencv4.8 + \opencv2\opencv.hpp (경로 오류 발생)

### ✅ 해결 (Solution)
1. **플랫폼 변경:** Visual Studio 상단 구성 관리자에서 `x64` → **`x86`**으로 변경.
2. **링커 종속성 수정:**
   * **[프로젝트 속성]** > **[링커]** > **[입력]** > **[추가 종속성]**
        * 존재하지 않는 `opencv_world` 대신 실제 존재하는 모듈 라이브러리 목록을 명시.
        ```text
        opencv_core480d.lib
        opencv_imgcodecs480d.lib
        opencv_highgui480d.lib
        opencv_imgproc480d.lib
        ```
   * **[프로젝트 속성]** > **[VC++ 디렉터리]** > **[포함 디렉터리]** 경로를 한 단계 상위 폴더로 수정.
        * 수정 전: ...\include\opencv2
        * 수정 후: ...\include


   * **[프로젝트 속성]** > **[디버깅]** > **[환경]**
   * 복수 파일명 대신 **파일 경로**명시
   ```text
   PATH=C:\opencv4.x\x86\vc15\bin;%PATH%
   ```

---

## 3. 실행 시 DLL 누락 오류 (System Error)

### 🔴 증상 (Symptom)
빌드는 성공했으나 실행(F5) 시 시스템 오류 팝업 발생.
> "opencv_core480d.dll이(가) 없어 코드 실행을 진행할 수 없습니다."

### 🔍 원인 (Cause)
1. 실행 파일(.exe)이 구동될 때 동적 링크 라이브러리(.dll)를 참조해야 하는데, 해당 파일들이 실행 폴더나 시스템 PATH에 존재하지 않음.

### ✅ 해결 (Solution)
1. DLL 파일을 매번 실행 폴더로 복사하는 대신, VS 디버깅 환경 변수를 설정하여 해결.

- **[프로젝트 속성]** > **[디버깅]** > **[환경]** 탭 선택.
    - PATH 변수에 OpenCV bin 경로 추가. : `PATH=C:\opencv4.x\x86\vc15\bin;%PATH%`
