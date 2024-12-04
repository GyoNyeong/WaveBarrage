# WaveBarrage

LOL PVE / 벰서라이크 집중포화 모작 프로젝트

<br>

---

<br>

## 🗂️ **프로젝트 개요**

| **진행 기간** | 2024.11.04 ~ 2024.11.25 / 22일 |
|---------------|---------------------------------|
| **팀 규모**   | 팀 프로젝트 총 3명 / 개발자: 최무성, 서지호, 박교녕 |
| **개발 환경** | UnrealEngine 5.4 ver, C++, GitHub, Notion |
| **PPT 링크** | https://www.miricanvas.com/v/13wpdm3 |
| **YouTube 데모 영상** | https://youtu.be/R9QI2_x66TI |

<br>

---

<br>

## 📂 **개발 내용**

<div style="display: flex; justify-content: space-between; align-items: center;">
    <div style="text-align: center; flex: 1;">
        <h3>WBPrototype Ver01</h3>
        <img src="https://github.com/user-attachments/assets/779813b3-89fc-48fc-8fbf-2ca8bf804910" alt="WBPrototype Ver01" width="400">
    </div>
    <div style="text-align: center; flex: 1;">
        <h3>WBAlpha Ver02</h3>
        <img src="https://github.com/user-attachments/assets/32b6d79b-b728-4055-80e7-a2a74291c6aa" alt="WBAlpha Ver02" width="400">
    </div>
</div>

<br>

---

<br>

## __나의 작업__

1. 무기 3종 과 발사체 구현.
    
    -FTimerHandle을 통해 공격 간격과 발사체 생성 간격을 제어. 
    
    -Fire 함수가 실행되면 SpawnProjectile 함수가 호출
    
    -무기의 주요 기능은 SpawnProjectile을 통해 발사체 actor를 생성. 
    
    -BeginPlay에서 HasAuthority를 사용해 서버에서만 Fire 함수의 타이머가 실행.
    
    -발사체 스폰의 동기화를 위해 Replicate를 true로 설정
    
    -플레이어의 소지 무기를 다른 플레이어와 공유해야 하므로 Replicate를 true로 설정
    
    -발사체의 오버랩 이벤트는 서버에서 처리하도록 설정
    
    - WhirlwindBlades
        
        ![Whirlwind](https://github.com/user-attachments/assets/1301cdc2-6bf2-4aef-9c87-f302555b2302)

        -발사체 위치를 동기화하기 위해, 서버에서 원운동을 계산하는 로직을 실행한 뒤, 클라이언트로 위치 정보를 전달하는 Multicast 함수를 호출하는 방식으로 동작
        
        -네트워크 사용량을 효율적으로 조절하기 위해, 위치 정보를 클라이언트로 전달하는 함수를 타이머를 사용해 일정 주기(예: 0.016초(60FPS), 0.033초(30FPS))마다 호출하도록 구현
        
    - Boomerang
        
        ![Boomerang](https://github.com/user-attachments/assets/e2880e51-28d4-4725-8efe-b5870b360be5)

        -발사체 수를 제어하기 위해, SpawnProjectile 함수가 실행될 때마다 정수형 변수의 값을 1씩 증가. 이 값이 발사체 수 변수와 같거나 커지면 함수 호출을 중단하고, 변수를 0으로 초기화
        
        -발사체 움직임은 가속도 효과를 구현하기위해  Tick함수에서 HasAuthority를 사용하여 서버에서 처리
        
        -움직임의 동기화를 위해 Replicate Movement를 True로 설정
        
    - PoisonFootprint
        
       ![PoisonFootprint](https://github.com/user-attachments/assets/64d1aaff-3a86-421c-a359-670487aaba8b)
        
        -Overlap된 몬스터는 배열에 추가되며, Timer를 사용해 ApplyDamage를 주기적으로 호출
        
        -몬스터와 Timer를 맵(Map) 구조로 관리
        
        -몬스터가 죽거나 Overlap 상태가 해제되면 Timer를 초기화하고 몬스터를 배열에서 제거
        
    
2. 카드 선택 시스템.
    
    ![Card](https://github.com/user-attachments/assets/8955a2cf-a76a-4608-b514-0e51d2c074e0)
    
    -캐릭터 레벨업 시 10종의 카드 중 랜덤으로 3장의 카드가 생성되고 그 중 1장을 선택
    
    -메인 위젯은 서버와 통신을 위해 PlayerController에서 생성하고, PlayerController를 통해 레벨업 이벤트가 호출되면 메인위젯에서 카드 위젯 생성
    
    -메인 위젯의 클래스 배열에 저장된 카드 클래스들 중 3개를 생성
    
    -카드 생성 전 로컬캐릭터의 상태를 탐색하여 일정 조건(예: 무기의 레벨이 5)을 충족하는 클래스들은 배열에서 제거하여 생성 중지
    
    -카드 클릭시 EventDispatcher를 사용해 메인 위젯에서는 생성된 카드를 모두 삭제
    
    -카드 위젯은 생성시에 WeaponDataBase나 PlayerState를 통해 화면에 표시할 정보를 업데이트
    
    -카드 선택시 서버로 무기나 스텟의 정보를 변경하는 함수를 호출. 이 함수는 클라이언트에 정보를 업데이트 하도록 지시
    
    -업데이트 된 정보를 가지고 메인 위젯의 UI변경
    
    - 2-1. ApplyCardEffect (정보를 업데이트 하는 함수)에 대하여
        
        -함수가 호출되면 카드가 가지고있는 인덱스라는 정수형 변수를 참조
        
        -인덱스를 스위치문으로 각 정수마다 해당하는 동작이 실행되도록 구현
        
    - 2-2. 게임의 일시 중지와 재시작
        
        -PlayerState에 Enum으로 Player의 상태를 관리
        
        -카드 생성시와 선택시 상태를 변경
        
        -GameState에 Enum을 체크해 게임의 재시작을 판단하는 함수를 구현
        
        -PlayerController에서 클라이언트에서 서버에 자신의 상태를 변경하고 GameState에서 체크하도록 요청하는 함수 구현
        
        -메인 위젯에서 카드 선택시 PlayerController의 함수를 호출
        
3. UI작업.
    
    3-1. 플레이어 UI
    
    -GameState에서 Reaplicate된 Level,Exp,Hp 등을 WBP의 바인딩 기능을 사용해서 UI업데이트
    
    -UI를 통해 무기의 소지 여부나 무기의 레벨을 알 수 있도록 작업
    
    3-2. 몬스터 UI
    
    -엘리트 몬스터나 보스 몬스터의 체력을 몬스터의 머리 위에 표시
    
    -몬스터 생성시 몬스터가 HP에 관한 위젯을 생성에 viewport에 추가
    
    -몬스터의 HP는 Replicate로 동기화
    
    ![동기화](https://github.com/user-attachments/assets/2f10e20e-944d-4dea-95db-5d235485360b)

## 작업 간 애로사항 및 작업 후 느낀점

1. 네트워크 환경에서, 동작하고 그를 서버 및 다른 클라이언트와 동기화시키는 과정의 문제
    
    -클라이언트에서 함수 실행시 그 결과가 다른 클라이언트와 공유되지 않음이 원인
    
    -네트워크 환경에서는 반드시 서버를 거쳐 동작을 하고 그를 클라이언트에 전달하는 과정을 수행해야한다를 알게 됨
    
    -Unreal에서 Actor의 스폰은 동기화를 위해서는 반드시 서버를 통해서 이루어져야 함을 알게 됨
    
    -변수가 Replicate가 되지않으면 값 전달이 제대로 이루어지지않는것을 알게 됨
    
2. Boomerang의 움직임 동기화 문제
    
    -부메랑 움직임은 Tick에서 AddActorWorldOffset함수를 통해 동작하는데 멀티플레이 실행시 클라이언트 에서 움직일 때 버벅거리거나 제대로 움직이지 않는 현상 발생
    
    -Actor의 움직임과는 별개로 위치 정보를 동기화시켜야 서버와 클라이언트간 정보의 차이가 발생하지 않는 것을 알게 됨
    
    -가속도 효과를 위해 Tick함수에서 로직을 구현한 것이 네트워크 사용량이 너무 많아 문제가 될수도 있음을 알게 됨
    
    -이를 통해 Whirlwind처럼 위치정보를 업데이트 하는 함수를 구현한다거나 하는 방식으로 네트워크 사용량을 개발자가 조절하도록 하는 것의 중요성을 깨달음
    
3. GameMode에서 구현한 기능은 클라이언트에서 동작하지 않던 문제
    
    -팀원이 GameMode에 구현한 ApplyCardEffect함수를 클라이언트에서 블루프린트로  호출하니 호스트쪽에서 두번 호출되던 문제 발생
    
    -Unreal에서 GameMode에서 구현한 동작들은 무조건 서버에서 동작하도록 설계되어있기 떄문이었음
    
    -함수를 PlayerController로 옮긴 뒤, 클라이언트에서 서버로 함수의 호출을 요청하는 구조를 만들어 원하는데로 클라이언트에서만 실행하도록 조정
    
4. ApplyCardEffect 함수 내의 무기 스폰이 제대로 되지않던 문제
    
    -ApplyCardEffect 내에는 무기가 존재하지않으면 무기를 스폰시키는 기능이 있다. 처음에는 이 기능이 함수와 마찬가지로 클라이언트에서 실행되고 있었음
    
    -이 경우 클라이언트 간 생성된 무기가 동기화되고 있지 않는 현상이  발생. 문제점을 알아보니 Actor의 스폰은 서버에서 해야 Replicate가 되는걸 알게됨
    
    -서버에 무기를 스폰시키는 요청을 하는 함수를 만들어 이 함수를 호출하는 방식으로 문제를 해결


