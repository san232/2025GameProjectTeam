# BuffSlinger: WinAPI/DX11 Subwindow Buff Shooter

## Portfolio Metadata

- 제목: BuffSlinger: WinAPI/DX11 서브윈도우 버프 슈터
- 설명: WinAPI 기반 2D 로그라이트 슈터에 DX11 스프라이트 렌더러와 FMOD 사운드를 통합했다. 마우스를 따라다니는 별도 서브윈도우가 영역 내 엔티티에 버프/디버프를 적용하며, FSM 적 AI, 보스 패턴, 전략형 탄환 시스템을 구현했다.
- 개발 기간: 2025.11.20 ~ 2026.03.28 (GitHub 생성/최종 푸시 기준 추정)
- 태그: C++, WinAPI, DirectX11, FMOD, 2D Game, Game Framework, FSM, Strategy Pattern, Collision, Bullet Hell, Boss AI
- 역할: 클라이언트/게임플레이 프로그래머 (코드 분석 기반 추정)

## Overview

WinAPI 기반 C++ 2D 게임 프로젝트로, 메인 게임 윈도우와 별도의 서브윈도우를 함께 활용해 플레이 공간에 영향을 주는 버프/디버프 렌즈 메커닉을 구현했다. 프로젝트는 Visual Studio C++20, DirectX11 스프라이트 렌더링, FMOD 오디오 연동을 사용한다.

## Key Technical Highlights

### Subwindow Gameplay Mechanic

- `SubWindow`가 별도 `HWND`, DXGI `SwapChain`, `RenderTargetView`를 생성한다.
- `SubWindowController`가 서브윈도우를 마우스 기반으로 이동시켜 플레이어가 직접 버프 영역을 조작할 수 있게 한다.
- `SubWindowRenderer`는 메인 백버퍼의 해당 영역을 `CopySubresourceRegion`으로 복사한 뒤 색상 틴트를 오버레이해 별도 렌더 타깃과 메인 화면의 시각적 연결을 유지한다.
- `SubWindowManager`는 서브윈도우의 스크린 좌표를 메인 클라이언트 좌표로 변환하고, 영역 안에 들어온 엔티티를 프레임 단위로 추적해 `OnEnter`, `OnStay`, `OnExit` 효과를 적용한다.

### FSM 기반 엔티티 구조

- `StateMachine`이 현재 상태의 `Enter`, `Update`, `Exit` 흐름을 일관되게 관리한다.
- Player, Enemy, Boss의 Idle/Move/Attack/Hit/Dead 계열 상태를 분리해 애니메이션, 이동, 공격 타이밍을 독립적으로 조정할 수 있게 했다.

### Strategy Pattern Bullet System

- `IBulletStrategy` 인터페이스로 MirrorBoss의 탄환 발사 방식을 추상화했다.
- 고속탄, 유도탄, 부메랑탄, 분열탄을 전략 객체로 분리해 보스 패턴 확장 비용을 낮췄다.

### Collision and Boss Pattern

- `CollisionManager`는 Layer bitmask와 Collider pair key를 사용해 충돌 Enter/Stay/Exit 이벤트를 관리한다.
- `BossKnight`는 Charge/Dash 상태를 분리해 대시 방향 고정, 고속 이동, 넉백, 8방향 탄환 패턴을 구현한다.

## Representative Code

- `2025_winapi_framework_22/SubWindow.h`
- `2025_winapi_framework_22/SubWindow.cpp`
- `2025_winapi_framework_22/SubWindowManager.h`
- `2025_winapi_framework_22/SubWindowManager.cpp`
- `2025_winapi_framework_22/SubWindowRenderer.h`
- `2025_winapi_framework_22/SubWindowRenderer.cpp`
- `2025_winapi_framework_22/SubWindowController.h`
- `2025_winapi_framework_22/SubWindowController.cpp`
- `2025_winapi_framework_22/ISubWindowEffect.h`
- `2025_winapi_framework_22/BuffEffects.h`
- `2025_winapi_framework_22/State.h`
- `2025_winapi_framework_22/StateMachine.h`
- `2025_winapi_framework_22/StateMachine.cpp`
- `2025_winapi_framework_22/IBulletStrategy.h`
- `2025_winapi_framework_22/MirrorBossBulletStrategies.h`
- `2025_winapi_framework_22/BossKnight.cpp`
- `2025_winapi_framework_22/BossChargeState.cpp`
- `2025_winapi_framework_22/BossDashState.cpp`
- `2025_winapi_framework_22/CollisionManager.h`
- `2025_winapi_framework_22/CollisionManager.cpp`

## Links

- GitHub: https://github.com/san232/2025GameProjectTeam
- Representative image candidate: `Output/build/Resource/Texture/TitleScene.bmp`
