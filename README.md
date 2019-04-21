# 운영체제 PA#1: MFQ 스케줄링 시뮬레이션

이 레포지토리는 운영체제의 첫번째 과제인 **MFQ Scheduling Simulation** 프로그래밍 과제입니다. 본 README에서는 과제의 개요, 입출력 파일 형식, 빌드 및 실행 방법, 프로젝트의 구조 등에 대해 서술합니다.

## 개요

본 과제는 다음과 같이 4-level로 구성된 MFQ 상에서의 프로세스 스케줄링 과정을 시뮬레이팅하는 것이 목표입니다.

1. Q<sub>0</sub>는 time slice가 2인 RR 방식을 사용합니다.
2. Q<sub>1</sub>은 time slice가 6인 RR 방식을 사용합니다.
3. Q<sub>2</sub>는 preemptive한 SRTN 방식을 사용합니다. Preemption은 I/O burst가 종료되어 Q<sub>2</sub>로 진입하는 프로세스에 의해서만 발생하며, 이 경우 다음 스케줄링 대상은 진입한 프로세스로 고정됩니다.
4. Q<sub>3</sub>는 FCFS 방식으로 구현합니다.
5. 스케줄링의 우선순위는 Q<sub>0</sub> > Q<sub>1</sub> > Q<sub>2</sub> > Q<sub>3</sub>이며, 기본적으로 **타 프로세스에 의한 preemption은 발생하지 않습니다.** 유일한 예외는 상술한 3의 경우입니다.

예를 들어, Q<sub>2</sub>의 P<sub>1</sub>이 실행되는 상황에서 P<sub>2</sub>가 Q<sub>1</sub>에 진입하더라도 preemption은 발생하지 않지만, P<sub>3</sub>가 Q<sub>2</sub>에 진입하고 그 시점에서 P<sub>1</sub>보다 우선순위가 더 높은 경우 preemption이 발생하며, 이 때 다음에 실행될 프로세스는 P<sub>2</sub>가 아닌 P<sub>3</sub>입니다.

프로그램을 실행 시 ① Gantt chart 형태의 각 프로세스의 실행 순서 및 시간 분포, ② 각 프로세스의 최종 Turnaround / Waiting time, ③ 평균 Turnaround / Waiting time이 출력되어야 합니다.

## 입출력 파일 형식

### 입력 (input.txt)

다음은 입력 파일의 예시입니다.

> 4
> 4 1 2 3 10 12 15 16 31
> 7 1 3 2 14 15 24
> 2 2 6 1 10
> 10 2 8 2 14 56 5

입력 파일은 시뮬레이션할 프로세스들을 설명하며, 다음과 같이 구성됩니다.

1. 파일 내 첫 번째 숫자는 파일에 등장하는 프로세스 개수를 의미합니다. 이후 입력에서는 한 줄에 한 프로세스에 대한 정보를 서술합니다.
2. 각 줄의 첫번째 숫자는 PID로, 각 프로세스의 고유한 식별자를 의미하는 정수입니다. (이후 PID = k로 가정하여 프로세스를 편의상 P<sub>k</sub>라고 하겠습니다.)
3. 두번째 숫자는 P<sub>k</sub>가 처음에 위치할 큐의 번호(0~3)입니다.
4. 세번째 숫자는 P<sub>k</sub>가 처음으로 큐에 추가되는 시각(도달 시각)을 나타내는 0 이상의 정수입니다.
5. 네번째 숫자는 P<sub>k</sub>의 CPU & I/O burst 사이클의 횟수를 나타내는 자연수입니다. 엄밀히는 CPU burst의 등장 횟수를 의미합니다.
6. 이후 CPU burst와 I/O burst에 해당하는 시간이 번갈아 자연수로 등장합니다. 항상 CPU burst가 먼저 등장하여 CPU burst로 끝납니다. 따라서 5의 숫자가 !n인 경우 이 단계에서 등장하는 숫자는 2n-1개 입니다.

### 출력 (output.txt)

다음은 위의 입력에 따른 출력의 일부입니다.

> 1 2 3 4 5
> |
> |
> &nbsp;|
> &nbsp;|
> |
> |
> ...
> &nbsp;|
> &nbsp;|
> &nbsp;|
> P1: 269 120
> P2: 473 105
> P3: 83 33
> P4: 293 263
> P5: 74 34
> avg: 238.40 111.00

출력 파일은 시뮬레이션 결과 및 분석에 대해 서술하며, 그 구성은 다음과 같습니다.

1. 가장 첫 줄의 숫자들은 PID를 나타내며, i번째 숫자는 i번째 열에 해당되는 프로세스의 PID를 나타냅니다.
2. 두번째 줄부터는 세로로 출력된 Gantt Chart입니다. k번째 줄은 (k-1)시 에서의 스케줄링 상태를 나타내며, 공백을 n개 두고 `|`가 있는 경우 1에서 (n+1)번째로 등장한 PID에 해당되는 프로세스가 실행되었음을 의미합니다. `|`가 없는 줄은 해당 시점에서 CPU가 IDLE 상태였음을 뜻합니다.
3. 프로세스의 개수만큼 `Pk: X y`가 출력됩니다. k는 PID를 의미하며, x, y는 각각 P<sub>k</sub>의 TT와 WT를 의미합니다.
4. 마지막 줄에는 `avg: X Y`가 출력됩니다. X, Y는 각각 전체 프로세스의 평균 TT와 WT를 의미합니다.

## 프로그램 구동 방법

### 빌드

이 프로젝트는 CMake를 이용해 관리됩니다. [CMakeLists.txt](CMakeLists.txt) 파일을 확인하면 3개의 target이 있는 것을 확인할 수 있으며, 그 중 `main`이 온전한 프로그램입니다. 별도의 컴파일러나 옵션은 명시되지 않은 매우 원시적인 형태이기에, 큰 어려움 없이 빌드할 수 있습니다.

만약 빌드가 되지 않는다면, 테스트용 프로그램인 `queue_test`나 `proc_test`를 빌드해보시기 바랍니다. 각각은 전체 프로그램에 쓰인 모듈의 일부만 포함한 프로그램입니다. 일부 모듈이 문제일 경우 이를 통해 어느 모듈이 문제인지 알 수 있습니다. 다음은 각각에 포함된 모듈의 종류를 설명합니다.

|모듈|`option`|`Process`|`Scheduler`|`ProcQueue`|`MFQ`|
|:----------:|:-----:|:-------:|:---------:|:---------:|:---:|
|`main`|<span style="color:limegreen"><span style="color:limegreen">✔</span></span>|<span style="color:limegreen"><span style="color:limegreen">✔</span></span>|<span style="color:limegreen"><span style="color:limegreen">✔</span></span>|<span style="color:limegreen"><span style="color:limegreen">✔</span></span>|<span style="color:limegreen"><span style="color:limegreen">✔</span></span>|
|`queue_test`|<span style="color:limegreen">✔</span>|<span style="color:limegreen">✔</span>|<span style="color:limegreen">✔</span>|<span style="color:limegreen">✔</span>|<span style="color:red">✘</span>|
|`proc_test` |<span style="color:limegreen">✔</span>|<span style="color:limegreen">✔</span>|<span style="color:red">✘</span>|<span style="color:red">✘</span>|<span style="color:red">✘</span>|

혹은 컴파일러나 환경을 확인해보세요. 본 프로젝트는 [MinGW-w64](https://mingw-w64.org/doku.php)를 이용해 Windows 10 상에서 컴파일 및 테스트가 이루어 졌습니다.

### 실행

본 프로그램은 기본적으로 input.txt와 output.txt를 각각 입출력 파일의 이름으로 취급합니다. 다음은 일반적인 실행 커맨드를 보여줍니다.

``` ShellSession
$ main

    Elapsed time: 2
    Proceeded process: 1


    Elapsed time: 2
    Proceeded process: 2

    ...

```

위와 같이 플래그가 선언되지 않은 상태로 실행되면 input.txt에서 입력을 받아 output.txt에 상술한 결과를 출력하며, 위와 같은 **콘솔 출력**을 내보내게 됩니다. 기본 상태에서 콘솔은 각 단계에서 스케줄링이 일어난 시간과 실행된 프로세스의 PID를 출력합니다.

이 프로그램에는 `option` 모듈에 정의된 여러 플래그를 사용할 수 있습니다. 그 목록은 다음과 같습니다.

* **`-v`, `--verbose`: Verbose 모드**
  더욱 상세한 콘솔 출력을 생성합니다. 현재 프로그램에 적용된 옵션, 입출력 파일 이름, 프로그램 초기화 및 프로세스 스케줄링 과정이 모두 출력됩니다.
  출력하는 분량이 상당히 많기 때문에 Output Redirecting을 통해 파일에 출력하여 로그처럼 확인하는 방법을 추천합니다.
* **`-s`, `--silent`: Silent 모드**
  에러를 제외한 프로그램의 모든 콘솔 출력을 제거합니다. 콘솔 창을 더럽히지 않고 출력 파일을 생성하고 싶을 때 사용하는 플래그입니다.
  **이 옵션은 Verbose 옵션보다 우선적으로 적용됩니다!** 즉, 두 모드가 동시에 켜져 있으면 콘솔 출력이 사라집니다.
* **`-o`: 출력 파일 설정**
  `-o` 플래그 직후에 나타나는 문자열을 출력 파일 이름으로 지정합니다.
* **입력 파일 설정**
  플래그가 지정되지 않고 나타나는 문자열은 입력 파일 이름으로 인식됩니다. 단, 해당 파일 명은 `-`으로 시작할 수 없습니다. 이는 해당 문자가 플래그를 식별자이기 때문입니다. (출력 파일은 무관합니다.)

각 플래그의 등장 순서는 크게 상관 없으나, **플래그를 중복으로 선언하면 경고 또는 오류가 발생할 수 있습니다.** Verbose 모드와 Silent 모드는 경고 메세지가 나오되 실행은 계속되지만, 입출력 파일 지정의 경우 오류가 발생하여 프로그램이 종료됩니다.

또한, 위에서 정의되지 않은 플래그, 즉 의미 없는 플래그가 입력된 경우에도 경고가 뜨며 프로그램이 계속 진행됩니다. 이러한 플래그 관련 메세지는 Silent 모드와 무관하게 항상 출력됩니다.

<sub>주의: 현 버전에서는 한 `-` 문자에 여러 플래그를 선언하는 문법이 지원되지 않습니다. 따라서 정의되지 않은 플래그 경고가 발생합니다.</sub>

## 프로젝트 구조

본 프로젝트는 5개의 헤더와 6개의 구현체로 구성된 `main` 프로그램과 부분 모듈을 테스트하는 `*_test` 프로그램으로 구성되어 있습니다. 각각의 프로그램은 독립적이나 프로젝트의 구조가 다소 복잡한 관계로, 본 문단에서 세부적인 내용을 서술합니다.

### 1. 프로세스 모듈

**<sub>See Also: [`process.h`](process.h), [`process.c`](process.c)</sub>**

#### 프로세스 구조체 (`Process`)

`Process` 구조체는 프로세스가 갖춘 속성의 집합체로, 다음과 같이 구성됩니다. **볼드체**로 된 것은 입력에 의해 결정되는 속성, *이탤릭체*로 된 것은 프로그램 실행 중 수정되는 속성입니다.

* **`pid`**: 프로세스의 식별자(ID)입니다.
* **`arrival`**: 프로세스가 시작되는 시점입니다.
* *`term`*: 프로세스가 종료되는 시점입니다.
* *`wait`*: 프로세스가 종료될 때까지 CPU burst 상태에서 대기한 시간입니다.
* **_`queue_idx`_**: 프로세스가 현재 위치하거나(CPU burst) 앞으로 위치할(I/O burst) 큐의 인덱스입니다.
* **`num_cycles`**: CPU burst, I/O burst의 총 개수입니다. 입력 파일에서의 사이클 개수 `n`에 대해, `2n`으로 계산됩니다.
* *`current_cycle`*: 현재 해당되는 burst의 인덱스 입니다. 홀수인 경우 CPU burst, 짝수인 경우 I/O burst에 해당됩니다. (`0`인 경우는 예외적으로 arrival time입니다.)
* **_`cycles`_**: arrival time과 burst cycle로 이루어진 배열입니다. 일반적으로 `Process` 구조체를 사용할 때 이 속성은 후술할 매크로 함수를 통해 접근하는 것이 좋습니다.

위에서 `current_cycle`과 `cycles`는 서로 짝을 이룹니다. 일반적으로 현재 사이클의 시간을 조사하기 위해서는 다음과 같이 접근합니다.

``` C
Process* p = SOME_PROCESS;
p->cycles[ p->current_cycle ]; // time of current cycle
```

하지만 이러한 표기법은 굉장히 불편하고 실수하기 쉽기에, 이 모듈에서는 다음과 같은 매크로 함수들을 제공합니다. 인자는 모두 `Process*` 타입의 `p`이며, 대상은 `p`가 가리키는 `Process` 객체입니다.

* `CUR_CYCLE`
  현재 사이클의 시간을 가져오는 함수입니다.
* `NEXT_CYCLE`
  다음 사이클의 시간을 가져오는 함수입니다.
* `PROC_END`
  해당 `Process`가 **종료**되었는지를 조사합니다. 여기서 **종료**는 더 이상 진행할 사이클이 없음을 의미합니다.
* `IS_ACTIVE`
  현재 `Process`가 CPU burst에 진입했는지 조사합니다.

보다 복잡한 작업의 경우는 함수로 제공되며, 그 목록은 다음과 같습니다.

* `read_process(FILE*)`
  주어진 `FILE`에서 `Process` 하나에 대한 정보를 읽고 초기화하여 반환합니다. 반환값은 `Process*` 타입으로, 동적 할당을 통해 생성된 `Process`의 메모리 주소입니다.
* `free_process(Process*)`
  해당 `Process`에 대한 메모리를 해제합니다. 별도의 반환값은 없습니다.
* `print_process(Process*)`
  `Process`의 정보를 출력합니다. 별도의 반환값은 없습니다.

#### 프로세스 비교자 (`ProcCompare`) 및 인자 (`Factor`)

`ProcCompare`는 이 모듈에서 선언된 함수의 프로토타입으로, 두 개의 `Process`를 비교할 수 있습니다. 그 형태는 다음과 같습니다.

``` C
int ProcCompare (Process* a, Process* b);
```

위에서 `a`와 `b`의 **특정 인자**를 비교한 결과를 반환합니다. 음수인 경우 `a < b`, 양수인 경우 `a > b`, 0인 경우 `a == b`의 관계를 만족합니다.

<sub>**주의**: 이 타입은 `Process`의 우선순위가 아닌, 특정 인자의 대소를 비교합니다. 실제 우선순위는 상위 모듈에서 판별합니다.</sub>

`Factor`는 `ProcCompare`로 비교할 인자의 목록을 열거자(enum)로 나타낸 것입니다. 현재는 `NONE`와 `REMAIN`이 있으며, 그 중 `REMAIN`은 `Process`의 현재 사이클, 즉 `CUR_CYCLE`의 값을 비교합니다. `NONE`은 비교 없이 무조건 `0`을 반환합니다.

`get_compare` 함수는 `Factor`를 인자로 받아 그에 맞는 `ProcCompare`를 반환합니다. 실제로 반환되는 함수는 `process.c`에 `static`으로 선언되어 직접 접근할 수 없고, 오로지 함수 포인터로만 접근이 가능합니다.

### 2. 스케줄러 모듈

**<sub>See Also: [`scheduler.h`](scheduler.h), [`scheduler.c`](scheduler.c)</sub>**

이 모듈은 프로세스 모듈에 의존하여, **특정 `Process`에 할당되어야 할 시간을 계산하는 로직**을 결정합니다.

#### 스케줄러 구조체 (`Scheduler`)

`Scheduler`는 상술한 로직을 담는 구조체입니다. 이 구조체는 스케줄링을 처리하는 `schedule` 함수와 그 인자로 쓰이는 `arg`로 구성됩니다. 전자는 그 형태가 `int schedule (Process*, void*)`인데, 후자(`arg`)가 두번째 인자로 쓰입니다. 첫번째 인자에는 스케줄링 대상 `Process` 객체가 대입되어 할당 시간을 반환합니다. 단, 이 함수는 계산만을 수행하며 객체의 값을 수정하지는 않습니다.

#### 스케줄러 정책 (`Policy`)

`Policy`는 스케줄링 전략에 대응되는 열거자입니다. `Scheduler`를 생성하는 `get_scheduler`의 인자로 쓰입니다.

`Policy`는 `Scheduler.schedule` 함수를 매핑하는 데에 쓰입니다. 함수들은 `process.c`와 마찬가지로 `scheduler.c`에 `static`으로 선언되어 있으며, **`Policy`의 각 원소와 일대일로 대응됩니다.** `Scheduler` 객체의 초기화에 쓰인 `Policy`를 가져오기 위해서는 `get_policy(Scheduler*)`를 사용할 수 있습니다.

<sub>NOTE : `get_policy`는 SRTN의 preemption을 구현하는 데에 사용됩니다.</sub>

각 `Policy` 원소에는 그에 대한 설명과 더불어 `arg`로 어떤 것을 요구하는지 기재되어 있습니다. 초기화 단계에서 이를 참조하여 인수를 대입하면 됩니다.

### 3. 프로세스 큐 모듈

**<sub>See Also: [`queue.h`](queue.h), [`queue.c`](queue.c)</sub>**

이 모듈은 프로세스 모듈과 스케줄러 모듈에 의존하여, **독립적인 스케줄링 전략을 갖는 프로세스 큐**를 구현합니다.

#### 프로세스 큐 구조체 (`ProcQueue`)

`ProcQueue`는 연결리스트(`Element` 참조)의 형태로 구현된 단일 레벨 프로세스 큐를 나타내는 구조체입니다. 한 `ProcQueue`는 다음과 같은 속성을 갖습니다.

* `ProcQueue`에 속한 `Process`들을 이중연결리스트로 연결하여, 양쪽 끝에 해당하는 `head`와 `tail`을 저장합니다.
* `Process`들을 가장 우선순위에 따라 `head`부터 `tail`까지 순서대로 배치하기 위한 `ProcCompare` 타입의 `compare` 함수를 갖습니다.
  모든 `Process`들은 암시적으로 해당 큐에 삽입되는 시점에 대한 우선순위가 존재하며, 스케줄링 방법에 따라 다른 요인에 의한 순위를 먼저 고려해 우선순위 큐처럼 작동하기도 합니다. (e.g. `Policy = SRTN`) 이는 스케줄링 대상을 정확히 판별하기 위해 필수적입니다.
* 스케줄링 전략을 `scheduler`로 참조합니다. 위의 `compare` 속성은 `ProcQueue` 객체, 정확히는 `scheduler` 속성의 초기화에 쓰인 `Policy`에 의해 결정됩니다.

`ProcQueue`는 본 과제에 쓰인 모듈 중에서 가장 많은 함수를 제공하지만, 대부분 일반적인 큐에 대한 연산입니다. `enqueue`, `dequeue`, `peek`, `is_empty` 등이 그러합니다. 또한, 보다 효율적인 메모리 관리를 위해 `dequeue-n-enqueue` 작업을 한 번에 수행하는 `move_head` 함수와 큐의 순회 작업을 단순화하는 `iterate` 함수 등이 추가로 제공됩니다.

더불어, `ProcQueue`의 생성자인 `get_queue` 함수와 프로세스 큐의 본 역할을 수행하는 `schedule` 함수가 존재합니다. 후자의 경우, 반드시 `head`에 위치한 `Process`가 그 대상이 되며 실제 작업은 `scheduler` 객체에게 위임됩니다. 따라서, 이 함수에서도 객체의 수정은 발생하지 않습니다.

#### 프로세스 큐 순회 함수 (`IterFunc`)

`IterFunc` 타입 함수는 `ProcQueue`를 순회하며 그에 속한 `Process`들에 동일한 함수를 적용하기 위해 고려되었습니다. 첫번째 인자는 호출 대상 `Process` 객체를, 두번째 인자는 추가적인 인자를 의미합니다. (`Scheduler`와 동일한 맥락입니다.) 특히 첫번째 인자가 포인터이기 때문에, 내부의 수정 사항이 프로그램 전체에 그대로 반영됩니다.

`iterate` 함수의 인자로 `IterFunc`가 전달됩니다. 해당 함수는 주어진 `ProcQueue`를 `head`부터 `tail`의 순서대로 순회하며 전달된 함수를 호출합니다. 내부적으로, 최대 한 번의 `dequeue` 작업에 대응하기 위한 장치가 마련되어 있습니다.

다른 함수 타입과 달리 이 함수는 원래부터 외부에서 구현하게끔 설계된 타입입니다. 이는 함수적 프로그래밍(FP)의 아이디어를 빌려 더욱 다양한 작업을 추상화, 단순화하기 위한 것입니다.

### 4. MFQ 모듈 (`MFQ`)

**<sub>See Also: [`mfq.h`](mfq.h), [`mfq.c`](mfq.c)</sub>**

이 모듈은 프로세스 큐 모듈에 의존하여, 본 과제의 최종 목적인 **다단계 피드백 큐(Multi-level Feedback Queue, MFQ)의 작업을 수행합니다.** `MFQ`는 과제에 사용된 모듈 중 가장 규모가 작지만, 그와 반대로 가장 복잡한 로직을 갖고 있습니다.

`MFQ` 구조체는 말 그대로 MFQ 그 자체이며, level의 수에 해당되는 `num_queue`와 각 레벨에 따른 `ProcQueue`를 참조하는 `queues`, 그리고 I/O burst에 들어간 `Process`들이 위치할 `ready_queue`를 속성으로 갖습니다.

`get_mfq`는 주어진 정수에 맞는 개수의 level을 갖는 `MFQ`를 초기화합니다. 다만 이는 `queues`에 `ProcQueue`의 포인터를 저장할 수 있는 메모리 공간을 확보할 뿐 실제 `queues`의 원소를 초기화하지는 않습니다.

각 큐를 초기화하기 위해서는 `set_queue` 함수를 사용해야 합니다. 이 함수는 대상 `MFQ`의 전달된 level에 해당하는 `ProcQueue`를 주어진 `Policy`에 맞게 생성 및 초기화합니다. 이는 모든 level에 대해 한 번씩 호출되어야 합니다.

실제로 스케줄링 대상 `Process`과 시간을 판별하고 `MFQ` 내에 존재하는 모든 `Process`들을 관리하는 작업은 `proceed` 함수에서 일어납니다. 이 함수는 `MFQ` 객체에서 스케줄링을 진행하여 스케줄링 대상 `Process`와 시간을 전달받은 메모리 공간에 각각 저장합니다.

`proceed` 함수에서 `Process`와 `ProcQueue`를 대상으로 하는 모든 **수정** 작업이 일어난다고 봐도 과언이 아닙니다. 실제 `Process`의 속성을 직접 수정하는 함수는 `read_process` 를 제외하면 이것이 유일하며, `queues` 및 `ready_queue`에서 일어나는 모든 `Process`의 움직임을 제어합니다. 다만 이러한 모든 작업이 내부적으로 실행되기 때문에, Verbose 옵션 없이는 세부적인 내용을 알 수 없다는 것이 단점입니다. 그러나 스케줄링 자체가 결과적으로는 Gantt chart로 표현되는 것이 전부이기에, 이 구조가 충분히 합리적이라고 판단됩니다.

### 5. 옵션 모듈 (`option`)

**<sub>See Also: [`option.h`](option.h), [`option.c`](option.c)</sub>**

프로그램 내에서 중요한 로직을 담당하지는 않지만, 실행에 있어 커맨드로 입력되는 모든 옵션을 분석 및 적용하는 모듈이기에 따로 기재합니다.

이 모듈에서는 `main` 함수에 전달된 인자를 분석하고 그것을 실행 옵션으로 적용하는 `init` 함수를 제공합니다. 그리고 `extern`에 의해 전역적으로 참조할 수 있는 옵션 변수들을 선언합니다. 더불어 옵션에 의한 구문과 주요 구문을 구별할 수 있게 도와주는 매크로를 정의합니다.

전역 변수를 사용한 이유는 각 모듈이 동일한 옵션으로 제어될 수 있게 하기 위함입니다. 또한, 이를 별도의 헤더 및 구현체로 사용한 이유는 `모듈화`를 위한 것입니다.