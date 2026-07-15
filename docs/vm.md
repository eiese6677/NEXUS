# NEXUS Virtual Machine Specification

Version: 0.1

This document defines the execution model of the NEXUS virtual machine. It describes the runtime architecture, memory model, execution loop, and error handling behavior required for implementing a compliant VM.

---

## 1. Overview

The NEXUS virtual machine is a stack-based runtime designed to execute bytecode generated from NEXUS source programs. The VM is responsible for:

- executing instructions in order
- managing program state
- evaluating runtime values
- handling function calls and returns
- reporting runtime errors

---

## 2. Runtime Components

### 2.1 Instruction Pointer

The instruction pointer (`ip`) indicates the current instruction being executed.

Rules:

- `ip` starts at zero.
- Each instruction advances `ip` according to its size.
- Jump instructions may set `ip` to an explicit target.

### 2.2 Operand Stack

The operand stack stores intermediate values used by instructions.

Rules:

- Values are pushed and popped according to instruction semantics.
- Arithmetic instructions consume operands from the stack.
- Function calls and returns use the stack to pass arguments and results.

### 2.3 Call Stack

The call stack stores frames for active function invocations.

Each frame contains:

- function identity
- local variable storage
- return address
- parent frame reference

### 2.4 Global Environment

Global variables are stored in a module-level environment accessible by all functions in the same module.

Rules:

- Global variables are initialized before execution begins.
- Global values are shared across calls.

### 2.5 Constant Pool

The constant pool stores literal values referenced by bytecode instructions.

Rules:

- String and numeric constants may be stored in the pool.
- Instructions refer to constants by index.

---

## 3. Value Model

The VM must support runtime values of the following kinds:

- Int
- Float
- Bool
- Char
- String
- Array
- List
- Map
- Void

The VM must preserve type information at runtime and reject invalid operations.

---

## 4. Execution Loop

The VM executes instructions in a loop:

```text
while not halted:
  instruction = code[ip]
  execute(instruction)
```

Rules:

- The loop must stop on `HALT`.
- A runtime error must terminate execution unless it is caught by a handler.
- The instruction pointer must be updated consistently after each instruction.

---

## 5. Function Execution

### 5.1 Call Semantics

When a function is called:

1. arguments are evaluated and pushed onto the stack
2. a new frame is created
3. arguments are assigned to local variables
4. execution enters the function body

### 5.2 Return Semantics

When `RETURN` is executed:

- a return value is popped from the stack if present
- the current frame is removed
- control returns to the caller

### 5.3 Recursion

Recursion is supported by creating a new frame for each recursive call.

Rules:

- each recursive call consumes a new frame
- stack overflow is a runtime failure if the implementation imposes a limit

---

## 6. Memory and Storage

### 6.1 Local Variables

Local variables are stored in the current frame.

Rules:

- access is by index or symbolic name resolved at compile time
- the frame must preserve variable lifetime for the duration of the function call

### 6.2 Global Variables

Global variables persist for the lifetime of the program execution.

### 6.3 Heap and Objects

The VM may use heap allocation for dynamic structures such as lists, maps, and strings.

Rules:

- the VM must preserve object identity where required
- object mutation must be observable by subsequent operations

---

## 7. Control Flow

### 7.1 Conditional Jump

Conditional jumps pop a boolean from the stack and branch accordingly.

### 7.2 Loops

Loops are implemented using jump instructions and condition checks.

### 7.3 Break and Continue

Break and continue are represented by control-flow jumps to the appropriate loop exit or next iteration target.

---

## 8. Error Handling

### 8.1 Runtime Errors

The VM must raise runtime errors for invalid conditions such as:

- division by zero
- index out of range
- invalid cast
- invalid operator application
- uncaught exception

### 8.2 Exception Handling

The VM must support try/catch semantics through the exception handler stack.

Rules:

- when an exception is thrown, the VM searches for the nearest handler
- if a handler is found, control transfers to the handler block
- if no handler is found, execution terminates with an error

---

## 9. Standard Library Integration

The VM must provide a runtime interface for built-in functions such as:

- `print`
- `input`
- `len`
- `type`

Rules:

- built-in functions should be implemented as intrinsic operations
- they must be callable from bytecode using the same calling convention as user functions

---

## 10. Implementation Notes for Conformance

An implementation is conforming if it:

- executes the bytecode according to the defined instruction semantics
- preserves runtime type behavior required by the language specification
- performs runtime checks required by the semantics
- reports errors with sufficient detail to identify the failing operation

---

## 11. Example Execution Scenario

Source:

```nexus
let x = 2
let y = x + 3
```

Conceptual execution flow:

1. push literal `2`
2. store to local variable `x`
3. load local `x`
4. push literal `3`
5. add
6. store to local variable `y`

The VM must produce the final state in which `x` equals `2` and `y` equals `5`.
