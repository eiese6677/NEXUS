# NEXUS Language Specification

Version: 0.1

## Status

This document defines the normative specification of the NEXUS programming language. It is intended to be the authoritative reference for implementers of the lexer, parser, type checker, bytecode generator, and virtual machine. This document defines language rules and semantics; it does not prescribe implementation strategy.

---

# 1. 언어 개요

## 1.1 언어 목적

NEXUS는 현대적인 범용 프로그래밍 언어로 설계되며, 다음 목표를 가진다.

- 정적 타입 기반의 안전한 프로그래밍
- 타입 추론으로 코드 가독성 향상
- 명시적 타입 표기와 명확한 타입 검사를 모두 지원
- 컴파일 타임 타입 검사와 바이트코드 기반 실행
- 읽기 쉬운 문법과 명확한 오류 메시지 제공

## 1.2 특징

- 세미콜론을 사용하지 않는다
- 중괄호를 사용한 블록 구문을 지원한다
- Kotlin과 유사한 `: Type` 타입 표기법을 지원한다
- 변수 타입은 명시적으로 또는 추론적으로 선언할 수 있다
- `Any` 타입을 지원한다
- 명시적 캐스팅은 `as` 연산자로 지원한다
- 컴파일 타임에 대부분의 타입 오류를 발견한다

## 1.3 철학

NEXUS는 Python과 같은 가독성 있는 문법을 지향하되, 타입 안정성과 명시성을 더 강화한 언어로 설계된다. 구현자는 다음 원칙을 준수해야 한다.

- 문법은 명확하고 예측 가능해야 한다
- 타입 오류는 가능한 한 빠르게 보고해야 한다
- 암시적 형변환은 최소화해야 한다
- 오류 메시지는 원인과 수정 방향을 명확히 제시해야 한다

---

# 2. 기본 문법

## 2.1 변수 선언

변수 선언은 `let` 키워드를 사용한다.

```nexus
let x = 10
let y: Int = 20
let name: String = "NEXUS"
```

규칙:

- `let`으로 선언된 변수는 선언 이후 값이 재할당될 수 있다
- 변수의 타입은 선언 시 명시하거나 추론할 수 있다
- 선언된 변수의 타입은 변경할 수 없다
- 선언과 동시에 초기값이 필요하다

## 2.2 상수

상수 선언은 `const` 키워드를 사용한다.

```nexus
const PI: Float = 3.14159
const GREETING: String = "Hello"
```

규칙:

- 상수는 한 번 초기화되면 값을 변경할 수 없다
- 상수는 선언 시 반드시 초기값을 가져야 한다
- 상수의 타입은 명시하거나 추론할 수 있다

## 2.3 주석

주석은 두 가지 형태를 지원한다.

```nexus
// 한 줄 주석

/*
여러 줄 주석
여러 줄로 설명할 수 있다
*/
```

규칙:

- 주석은 코드와 독립적으로 사용될 수 있다
- 주석은 구현자에게 참고 정보를 제공하지만 실행에는 영향을 주지 않는다

## 2.4 식별자 규칙

식별자는 다음 규칙을 따른다.

- 첫 문자로 영문자 또는 `_`를 사용할 수 있다
- 이후 문자로 영문자, 숫자, `_`를 사용할 수 있다
- 대소문자를 구분한다
- 예약어는 식별자로 사용할 수 없다

예시:

```nexus
let userName = "Ada"
let _count = 1
let value2 = true
```

## 2.5 예약어

다음 토큰은 예약어로 정의되며 식별자로 사용할 수 없다.

```text
let const if else while for break continue return fn
import module namespace as
true false
Int Float Bool Char String Void Any
print input len type try catch throw
Array List Map
```

---

# 3. 타입 시스템

## 3.1 모든 타입 설명

NEXUS는 다음 기본 타입을 가진다.

- `Int`: 정수형. 32비트 범위의 정수를 표현한다
- `Float`: 실수형. IEEE-754 기본 실수를 표현한다
- `Bool`: 논리형. `true` 또는 `false`만 허용한다
- `Char`: 하나의 문자. 유니코드 문자 하나를 표현한다
- `String`: 문자열
- `Void`: 반환값이 없는 함수의 반환 타입
- `Any`: 모든 타입의 상위 타입

## 3.2 타입 추론

NEXUS는 타입 추론을 지원한다.

```nexus
let x = 10
let y = 3.14
let flag = true
let text = "hello"
```

규칙:

- `let` 선언에서 초기값이 제공되면 타입은 초기값에 따라 추론된다
- 추론된 타입은 이후 변경할 수 없다
- 명시적 타입 표기가 함께 주어지면 해당 타입이 우선한다
- 타입 추론이 불가능한 경우 컴파일 오류가 발생한다

## 3.3 Any

`Any`는 모든 타입 값이 저장 가능한 최상위 타입이다.

```nexus
let value: Any = 42
let another: Any = "text"
let boolValue: Any = true
```

규칙:

- `Any` 변수에는 어떤 타입의 값도 저장할 수 있다
- `Any`에 저장된 값을 다른 타입 변수에 직접 대입하는 것은 허용되지 않는다
- `Any`에서 구체 타입으로 복원하려면 명시적 캐스팅이 필요하다

## 3.4 캐스팅

명시적 캐스팅은 `as` 연산자로 표현한다.

```nexus
let x: Int = 10
let y: Float = x as Float
let text: String = "65"
let code: Int = text as Int
```

규칙:

- 캐스팅은 명시적으로만 허용된다
- `as`는 값의 타입을 다른 타입으로 변환한다
- 캐스팅이 불가능한 경우 컴파일 타임 또는 런타임 오류가 발생한다

## 3.5 형변환 규칙

NEXUS는 암시적 형변환을 최소화한다.

규칙:

- `Int`에서 `Float`으로의 암시적 변환은 허용될 수 있다
- 그 외의 암시적 형변환은 허용하지 않는다
- `Bool`과 숫자형 사이의 암시적 변환은 허용하지 않는다
- 문자열과 숫자형 사이의 암시적 변환은 허용하지 않는다
- `Any`에서 구체 타입으로의 변환은 명시적 캐스팅이어야 한다

예시:

```nexus
let a: Int = 3
let b: Float = a
```

```nexus
let c: Int = 5
let d: String = c  // 오류
```

---

# 4. 연산자

## 4.1 산술 연산자

다음 산술 연산자를 지원한다.

- `+` 덧셈
- `-` 뺄셈
- `*` 곱셈
- `/` 나눗셈
- `%` 나머지

```nexus
let sum = a + b
let diff = a - b
let product = a * b
let quotient = a / b
let remainder = a % b
```

## 4.2 비교 연산자

- `==` 동등
- `!=` 비동등
- `<` 작음
- `<=` 작거나 같음
- `>` 큼
- `>=` 크거나 같음

```nexus
let equal = (a == b)
let notEqual = (a != b)
```

## 4.3 논리 연산자

- `&&` 논리 AND
- `||` 논리 OR
- `!` 논리 NOT

```nexus
let ok = (a > 0) && (b > 0)
let result = !flag
```

## 4.4 비트 연산자

- `&` 비트 AND
- `|` 비트 OR
- `^` 비트 XOR
- `~` 비트 NOT
- `<<` 좌측 시프트
- `>>` 우측 시프트

```nexus
let mask = x & 0xFF
let shifted = x << 1
```

## 4.5 대입 연산자

- `=` 할당

```nexus
let x = 1
x = 2
```

규칙:

- 대입 연산은 값을 저장하며 표현식의 결과는 없다
- 대입 연산은 우변의 값이 좌변의 타입과 호환되어야 한다

## 4.6 우선순위

연산자 우선순위는 다음 순서로 정의한다.

1. 괄호 `()`
2. 단항 연산자 `!`, `~`, `-`
3. 곱셈/나눗셈/나머지 `*`, `/`, `%`
4. 덧셈/뺄셈 `+`, `-`
5. 시프트 `<<`, `>>`
6. 비교 연산자 `<`, `<=`, `>`, `>=`
7. 동등 비교 `==`, `!=`
8. 비트 AND `&`
9. 비트 XOR `^`
10. 비트 OR `|`
11. 논리 AND `&&`
12. 논리 OR `||`
13. 대입 `=`

---

# 5. 제어문

## 5.1 if

`if` 문은 조건식이 `Bool`이어야 한다.

```nexus
if (x > 0) {
  print("positive")
}
```

규칙:

- 조건식은 반드시 `Bool` 타입이어야 한다
- 조건식이 `true`이면 블록이 실행된다
- 조건식이 `false`이면 블록이 건너뛰어진다

## 5.2 else

`else`는 `if` 문과 함께 사용한다.

```nexus
if (x > 0) {
  print("positive")
} else {
  print("non-positive")
}
```

## 5.3 while

`while` 문은 조건이 참인 동안 반복한다.

```nexus
let i = 0
while (i < 5) {
  print(i)
  i = i + 1
}
```

규칙:

- 조건식은 `Bool`이어야 한다
- 반복문 내에서 변수 값이 변경되어 조건이 종료되어야 한다

## 5.4 for

`for` 문은 컬렉션의 각 원소를 순회한다.

```nexus
let values = [1, 2, 3, 4]
for (value in values) {
  print(value)
}
```

규칙:

- `for`는 배열, 리스트, 문자열, 맵의 순회에 사용될 수 있다
- `for`의 루프 변수는 블록 내에서만 유효하다
- `for` 루프의 반복 대상은 순회 가능한 컬렉션이어야 한다

## 5.5 break

`break`는 반복문을 즉시 종료한다.

```nexus
while (true) {
  if (x > 10) {
    break
  }
  x = x + 1
}
```

## 5.6 continue

`continue`는 현재 반복을 종료하고 다음 반복으로 넘어간다.

```nexus
for (value in values) {
  if (value == 0) {
    continue
  }
  print(value)
}
```

## 5.7 return

`return`은 함수에서 값을 반환하거나 함수 실행을 종료한다.

```nexus
fn add(a: Int, b: Int): Int {
  return a + b
}

fn greet(): Void {
  print("Hello")
  return
}
```

규칙:

- 함수가 `Void`를 반환하는 경우 `return`은 인수 없이 사용할 수 있다
- 비-`Void` 함수는 반드시 값을 반환해야 한다
- `return`은 함수 본문에서만 사용할 수 있다

---

# 6. 함수

## 6.1 선언

함수 선언은 `fn` 키워드를 사용한다.

```nexus
fn add(a: Int, b: Int): Int {
  return a + b
}
```

규칙:

- 함수 이름은 식별자 규칙을 따른다
- 함수 본문은 중괄호로 감싼다
- 함수는 선언 전에 호출될 수 있다

## 6.2 매개변수

함수 매개변수는 반드시 타입을 명시해야 한다.

```nexus
fn greet(name: String): Void {
  print("Hello, " + name)
}
```

규칙:

- 매개변수 이름은 식별자 규칙을 따른다
- 각 매개변수는 이름과 타입이 필요하다
- 기본값 매개변수는 지원하지 않는다

## 6.3 반환형

함수의 반환형은 반드시 명시해야 한다.

```nexus
fn square(x: Int): Int {
  return x * x
}
```

규칙:

- 반환형은 `Void` 또는 다른 타입이어야 한다
- 함수 본문에서 반환값의 타입이 선언된 반환형과 일치해야 한다

## 6.4 오버로딩 여부

NEXUS는 함수 오버로딩을 지원하지 않는다.

규칙:

- 같은 이름의 함수는 하나만 선언할 수 있다
- 동일한 이름의 함수를 중복 선언하면 컴파일 오류가 발생한다

## 6.5 재귀

재귀 함수는 지원한다.

```nexus
fn factorial(n: Int): Int {
  if (n <= 1) {
    return 1
  }
  return n * factorial(n - 1)
}
```

규칙:

- 재귀 호출은 허용된다
- 무한 재귀는 런타임 실행 중 스택 오버플로우로 이어질 수 있다

---

# 7. 배열 및 컬렉션

## 7.1 Array

`Array<T>`는 고정 길이의 동종 컬렉션이다.

```nexus
let values: Array<Int> = [1, 2, 3]
```

규칙:

- 배열의 길이는 생성 시 고정된다
- 배열의 요소 타입은 모두 동일해야 한다
- 인덱스는 0부터 시작한다

## 7.2 List

`List<T>`는 가변 길이의 동종 컬렉션이다.

```nexus
let names: List<String> = ["Ada", "Grace"]
```

규칙:

- 리스트는 요소를 동적으로 추가하거나 제거할 수 있다
- 리스트의 요소 타입은 모두 동일해야 한다

## 7.3 Map

`Map<K, V>`는 키-값 쌍을 저장한다.

```nexus
let scores: Map<String, Int> = {
  "Ada": 100,
  "Grace": 95
}
```

규칙:

- 맵의 키 타입과 값 타입은 각각 지정된다
- 키는 고유해야 한다

## 7.4 인덱싱

배열, 리스트, 문자열, 맵은 인덱싱을 지원한다.

```nexus
let first = values[0]
let name = names[1]
let char = text[2]
let score = scores["Ada"]
```

규칙:

- 인덱스는 정수형이어야 한다
- 범위를 벗어난 인덱스는 런타임 오류가 발생한다

## 7.5 길이

`len` 표준 함수는 컬렉션의 길이를 반환한다.

```nexus
let size = len(values)
```

규칙:

- 배열, 리스트, 문자열, 맵 등에서 길이를 구할 수 있다
- 길이는 정수형으로 반환된다

## 7.6 반복

컬렉션은 `for` 문으로 반복할 수 있다.

```nexus
for (item in names) {
  print(item)
}
```

---

# 8. 문자열

## 8.1 문자열

문자열은 `String` 타입으로 표현한다.

```nexus
let message: String = "Hello, NEXUS"
```

규칙:

- 문자열은 유니코드 문자의 연속이다
- 문자열은 변경 불가능한 값이다
- 문자열 연결은 `+` 연산자로 수행한다

## 8.2 이스케이프

문자열은 다음 이스케이프 시퀀스를 지원한다.

- `\n` 줄바꿈
- `\t` 탭
- `\\` 백슬래시
- `\"` 큰따옴표
- `\'` 작은따옴표

```nexus
let text = "line 1\nline 2"
```

## 8.3 문자열 보간

문자열 보간은 `${expression}` 형식으로 수행한다.

```nexus
let name = "Ada"
let greeting = "Hello, ${name}"
```

규칙:

- 보간식은 문자열 내부에서 평가된다
- 보간식의 결과는 문자열로 변환되어 삽입된다

---

# 9. 스코프

## 9.1 지역 변수

함수 내부에서 선언된 변수는 지역 변수이다.

```nexus
fn example(): Void {
  let x = 1
  print(x)
}
```

규칙:

- 지역 변수는 선언된 블록과 그 하위 블록에서만 접근 가능하다

## 9.2 전역 변수

파일 수준에서 선언된 변수는 전역 변수이다.

```nexus
let globalValue = 10
```

규칙:

- 전역 변수는 모듈 전체에서 접근 가능하다
- 전역 변수는 선언 이후 프로그램 전체에서 참조될 수 있다

## 9.3 블록 스코프

블록 `{}`은 새로운 스코프를 만든다.

```nexus
let x = 1
{
  let x = 2
  print(x)
}
print(x)
```

규칙:

- 내부 블록의 변수는 외부 변수와 이름이 같아도 된다
- 내부 블록의 선언은 외부 스코프에 영향을 주지 않는다

## 9.4 Shadowing 허용 여부

NEXUS는 Shadowing을 허용한다.

규칙:

- 내부 스코프에서 외부 스코프의 이름과 같은 이름의 변수를 선언할 수 있다
- 내부 스코프에서 참조되는 이름은 가장 가까운 스코프의 선언을 따른다
- 구현자는 경고를 제공할 수 있지만 필수는 아니다

---

# 10. 예외

## 10.1 오류 처리 방식

NEXUS는 예외 기반 오류 처리 모델을 사용한다.

```nexus
try {
  let x = 1 / 0
} catch (e) {
  print("error: " + e)
}
```

규칙:

- `try` 블록은 예외를 포착할 수 있다
- `catch` 블록은 예외 객체를 수신한다
- 예외가 발생하지 않으면 `catch` 블록은 실행되지 않는다

## 10.2 컴파일 오류

컴파일 오류는 소스 코드가 언어 규칙을 위반한 경우 발생한다.

예시:

- 잘못된 타입 대입
- 함수의 반환형 불일치
- 선언되지 않은 식별자 사용
- 예약어 사용

규칙:

- 컴파일 오류는 프로그램이 바이트코드로 변환되기 전에 보고되어야 한다
- 오류 메시지는 오류 위치와 원인을 포함해야 한다

## 10.3 런타임 오류

런타임 오류는 실행 중에 발생하는 오류이다.

예시:

- 0으로 나누기
- 범위를 벗어난 인덱싱
- 잘못된 캐스팅
- 잘못된 연산

규칙:

- 런타임 오류는 예외로 처리되어야 한다
- 예외가 처리되지 않으면 프로그램은 종료된다

---

# 11. 표준 라이브러리

NEXUS의 표준 라이브러리는 기본 입출력과 기본 연산을 제공한다.

## 11.1 print

```nexus
print("Hello")
print(42)
```

규칙:

- `print`는 값을 표준 출력에 표시한다
- 문자열이 아닌 값도 문자열로 변환되어 출력될 수 있다

## 11.2 input

```nexus
let name = input()
```

규칙:

- `input`은 사용자 입력을 문자열로 반환한다
- 입력이 없으면 빈 문자열을 반환할 수 있다

## 11.3 len

```nexus
let size = len("NEXUS")
```

규칙:

- `len`은 문자열, 배열, 리스트, 맵의 길이를 반환한다

## 11.4 type

```nexus
let t = type(42)
```

규칙:

- `type`은 인수의 런타임 타입 이름을 문자열로 반환한다

## 11.5 기타 기본 함수

다음 함수는 기본 표준 라이브러리에서 제공될 수 있다.

- `abs(x)`: 절댓값
- `round(x)`: 반올림
- `toString(x)`: 값을 문자열로 변환

---

# 12. 파일 구조

## 12.1 import

모듈 간의 기능 사용은 `import` 문으로 선언한다.

```nexus
import math
```

규칙:

- `import`는 다른 모듈의 심볼을 가져온다
- 가져온 심볼은 현재 파일에서 사용할 수 있다
- 중복 import는 허용될 수 있으나 경고를 줄 수 있다

## 12.2 module

모듈 선언은 파일 상단에 작성한다.

```nexus
module math.utils
```

규칙:

- 모듈 이름은 점으로 구분된 식별자 문자열이어야 한다
- 하나의 파일은 하나의 모듈을 정의할 수 있다

## 12.3 namespace

네임스페이스는 관련 심볼을 그룹화하는 데 사용한다.

```nexus
namespace algorithms
```

규칙:

- 네임스페이스는 선택적이다
- 네임스페이스는 모듈 내부에서 유효하다
- 동일 이름의 심볼이 다른 네임스페이스에 존재할 수 있다

---

# 13. 스타일 가이드

## 13.1 권장 코드 스타일

- 함수와 변수 이름은 `camelCase`를 사용한다
- 상수 이름은 `UPPER_CASE`를 사용한다
- 파일 이름은 `snake_case`를 사용한다
- 함수와 블록은 명확하게 구분되어야 한다

예시:

```nexus
fn calculateTotal(price: Int, tax: Int): Int {
  let total = price + tax
  return total
}
```

## 13.2 들여쓰기

- 들여쓰기 너비는 2 공백을 권장한다
- 탭은 사용하지 않는 것을 권장한다

## 13.3 공백

- 연산자 주변에는 공백을 사용한다
- 괄호와 구문 사이에는 가독성을 위해 공백을 사용한다
- 불필요한 공백은 피한다

## 13.4 파일명 규칙

- 파일 확장자는 `.nx`를 사용한다
- 파일 이름은 소문자와 밑줄만 사용한다
- 예: `math_utils.nx`, `string_helpers.nx`

---

# 14. 향후 확장 예정 기능

다음 기능은 향후 언어 사양에 포함될 예정이며, 현재 명세의 핵심 범위에는 포함되지 않는다.

## 14.1 Generic

제네릭 타입을 지원할 예정이다.

```nexus
fn identity<T>(value: T): T {
  return value
}
```

## 14.2 Nullable

`Nullable<T>` 또는 `?` 기반의 널 가능 타입을 도입할 예정이다.

```nexus
let maybeName: String? = null
```

## 14.3 Union Type

여러 타입을 조합하는 유니온 타입을 지원할 예정이다.

```nexus
let value: Int | String
```

## 14.4 Interface

인터페이스 기반의 추상 타입을 도입할 예정이다.

```nexus
interface Printable {
  fn print(): Void
}
```

## 14.5 Class

객체 지향 프로그래밍을 위한 클래스 모델을 도입할 예정이다.

```nexus
class Person {
  let name: String
}
```

## 14.6 Pattern Matching

패턴 매칭 문법을 도입할 예정이다.

```nexus
match value {
  case 1 -> print("one")
  case _ -> print("other")
}
```

## 14.7 Package Manager

외부 라이브러리와 모듈을 관리하기 위한 패키지 매니저를 도입할 예정이다.

```nexus
package add "math-utils"
```

---

# 부록 A. 구현자에게 대한 권고사항

구현자는 다음 사항을 준수해야 한다.

- 문법은 세미콜론 없이 해석되어야 한다
- 블록 구문은 중괄호를 사용해야 한다
- 타입 검사는 컴파일 타임에 수행되어야 한다
- `Any` 타입과 명시적 캐스팅 규칙을 정확히 구현해야 한다
- 오류 메시지는 사용자가 원인을 이해할 수 있도록 구체적이어야 한다
- 바이트코드 생성 및 VM 실행 시 언어 규칙과 일관되어야 한다
