# NEXUS Bytecode Specification

Version: 0.1

This document defines the instruction set for the NEXUS virtual machine. The bytecode format is designed to be simple, stack-based, and suitable for execution by a compact virtual machine implementation.

---

## 1. Design Goals

The bytecode for NEXUS is intended to satisfy the following requirements:

- stack-based execution
- simple operand encoding
- predictable control flow
- support for arithmetic, comparisons, variables, functions, and control structures
- compatibility with static type checking and runtime error handling

---

## 2. Execution Model

The virtual machine operates with:

- an instruction pointer
- an operand stack
- a call stack
- a variable environment
- an exception handler stack

Each instruction consumes and produces values on the operand stack according to its semantics.

---

## 3. Instruction Format

Instructions are encoded as one-byte opcodes followed by zero or more operands.

```text
opcode [operand ...]
```

The VM must interpret instructions sequentially unless control-flow instructions redirect execution.

---

## 4. Constants and Values

The VM must support the following runtime value kinds:

- Int
- Float
- Bool
- Char
- String
- Array
- List
- Map
- Void
- Nil

The bytecode may include instructions that push literals directly or load literals from a constant pool.

---

## 5. Instruction Set

### 5.1 Literal and Stack Instructions

#### `PUSH_CONST index`

Pushes a constant from the constant pool onto the operand stack.

#### `PUSH_INT value`

Pushes an integer literal.

#### `PUSH_FLOAT value`

Pushes a floating-point literal.

#### `PUSH_BOOL value`

Pushes a boolean literal.

#### `PUSH_STRING index`

Pushes a string literal from the constant pool.

#### `POP`

Removes the top value from the operand stack.

#### `DUP`

Duplicates the top value of the operand stack.

#### `LOAD_LOCAL index`

Loads the value of a local variable from the current frame.

#### `STORE_LOCAL index`

Stores the top stack value into a local variable slot.

#### `LOAD_GLOBAL name`

Loads a global variable by name.

#### `STORE_GLOBAL name`

Stores a value into a global variable.

---

### 5.2 Arithmetic Instructions

#### `ADD`

Pops two operands, adds them, and pushes the result.

#### `SUB`

Pops two operands, subtracts them, and pushes the result.

#### `MUL`

Pops two operands, multiplies them, and pushes the result.

#### `DIV`

Pops two operands, divides them, and pushes the result.

#### `MOD`

Pops two operands, computes the remainder, and pushes the result.

#### `NEG`

Negates the top operand.

---

### 5.3 Comparison and Logical Instructions

#### `EQ`

Compares two values for equality.

#### `NEQ`

Compares two values for inequality.

#### `LT`

Checks whether the left operand is less than the right operand.

#### `LTE`

Checks whether the left operand is less than or equal to the right operand.

#### `GT`

Checks whether the left operand is greater than the right operand.

#### `GTE`

Checks whether the left operand is greater than or equal to the right operand.

#### `AND`

Computes logical AND.

#### `OR`

Computes logical OR.

#### `NOT`

Computes logical NOT.

---

### 5.4 Bitwise Instructions

#### `BIT_AND`

Performs bitwise AND.

#### `BIT_OR`

Performs bitwise OR.

#### `BIT_XOR`

Performs bitwise XOR.

#### `BIT_NOT`

Performs bitwise NOT.

#### `SHL`

Performs left shift.

#### `SHR`

Performs right shift.

---

### 5.5 Control Flow Instructions

#### `JUMP offset`

Unconditionally jumps to the instruction at the given offset.

#### `JUMP_IF_TRUE offset`

Pops a boolean value and jumps if it is true.

#### `JUMP_IF_FALSE offset`

Pops a boolean value and jumps if it is false.

#### `CALL name`

Calls a function or built-in by name.

#### `RETURN`

Returns from the current function.

#### `HALT`

Stops program execution.

---

### 5.6 Collection Instructions

#### `BUILD_ARRAY count`

Builds an array from the top `count` values on the stack.

#### `BUILD_LIST count`

Builds a list from the top `count` values on the stack.

#### `BUILD_MAP count`

Builds a map from the top `count` values on the stack, where each pair is two values: key then value.

#### `INDEX`

Indexes a collection using the top value as the index and the next value as the collection.

#### `LEN`

Pushes the length of a collection or string.

---

### 5.7 String Instructions

#### `CONCAT`

Concatenates two strings.

#### `STRING_INTERPOLATE`

Interpolates a string with embedded expressions.

---

### 5.8 Exception Instructions

#### `TRY_BEGIN handlerOffset`

Marks the beginning of a protected region.

#### `TRY_END`

Marks the end of a protected region.

#### `THROW`

Throws the top value as an exception.

#### `CATCH handlerOffset`

Transfers control to the exception handler.

---

## 6. Runtime Semantics

### 6.1 Stack Behavior

- Unary operations pop one value and push one result.
- Binary operations pop two values and push one result.
- Control-flow instructions may change the instruction pointer directly.

### 6.2 Variable Access

- Local variables are accessed through the current frame.
- Global variables are stored in a module-level environment.

### 6.3 Function Calls

Function calls create a new frame and push a return address.

### 6.4 Errors

The VM must raise runtime errors for:

- division by zero
- invalid indexing
- invalid cast
- invalid type operations
- uncaught exceptions

---

## 7. Example Bytecode Sketch

Source:

```nexus
let x = 1 + 2
```

Conceptual bytecode:

```text
PUSH_INT 1
PUSH_INT 2
ADD
STORE_LOCAL 0
```

Source:

```nexus
if (x > 0) {
  print("ok")
}
```

Conceptual bytecode:

```text
LOAD_LOCAL 0
PUSH_INT 0
GT
JUMP_IF_FALSE L1
PUSH_STRING 0
CALL print
L1:
HALT
```
