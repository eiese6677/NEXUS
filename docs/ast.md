# NEXUS AST Specification

Version: 0.1

This document defines the abstract syntax tree (AST) structure for NEXUS programs. It describes the node kinds, their required fields, and the semantic expectations for each node. Implementers should use this document as the contract between parsing and downstream analysis or code generation.

---

## 1. Design Principles

The AST for NEXUS is a typed tree representing the syntactic structure of source programs. Each node captures:

- the syntactic category of the construct
- the source span or position when available
- child nodes in a deterministic order
- semantic information required by type checking and code generation

The AST must remain independent of concrete syntax formatting such as whitespace or semicolons.

---

## 2. Root Node

### ProgramNode

Represents the full source file.

```text
ProgramNode {
  statements: List<StatementNode>
}
```

Rules:

- The root contains all top-level declarations and statements.
- Module, namespace, import, function, variable, and constant declarations are represented as statement-like nodes.

---

## 3. Statements

### BlockNode

Represents a lexical block.

```text
BlockNode {
  statements: List<StatementNode>
}
```

### VariableDeclarationNode

```text
VariableDeclarationNode {
  name: Identifier
  typeAnnotation: TypeAnnotationNode | null
  initializer: ExpressionNode
  mutable: true
}
```

Rules:

- The initializer must be present.
- If a type annotation is absent, the type is inferred.

### ConstantDeclarationNode

```text
ConstantDeclarationNode {
  name: Identifier
  typeAnnotation: TypeAnnotationNode | null
  initializer: ExpressionNode
}
```

Rules:

- The initializer must be present.
- The value cannot be reassigned.

### AssignmentNode

```text
AssignmentNode {
  target: Identifier
  value: ExpressionNode
}
```

Rules:

- The target must refer to a mutable variable or storage location.
- Type compatibility must be checked by the type checker.

### ExpressionStatementNode

```text
ExpressionStatementNode {
  expression: ExpressionNode
}
```

### IfNode

```text
IfNode {
  condition: ExpressionNode
  thenBranch: BlockNode
  elseBranch: BlockNode | null
}
```

### WhileNode

```text
WhileNode {
  condition: ExpressionNode
  body: BlockNode
}
```

### ForNode

```text
ForNode {
  loopVariable: Identifier
  iterable: ExpressionNode
  body: BlockNode
}
```

### BreakNode

```text
BreakNode {}
```

### ContinueNode

```text
ContinueNode {}
```

### ReturnNode

```text
ReturnNode {
  value: ExpressionNode | null
}
```

### TryNode

```text
TryNode {
  tryBlock: BlockNode
  catchVariable: Identifier
  catchBlock: BlockNode
}
```

### FunctionDeclarationNode

```text
FunctionDeclarationNode {
  name: Identifier
  parameters: List<ParameterNode>
  returnType: TypeAnnotationNode
  body: BlockNode
}
```

Rules:

- Parameter types are mandatory.
- Return type is mandatory.
- The function body may contain return statements.

### ImportDeclarationNode

```text
ImportDeclarationNode {
  moduleName: QualifiedName
  alias: Identifier | null
}
```

### ModuleDeclarationNode

```text
ModuleDeclarationNode {
  name: QualifiedName
}
```

### NamespaceDeclarationNode

```text
NamespaceDeclarationNode {
  name: Identifier
}
```

---

## 4. Expressions

### LiteralNode

```text
LiteralNode {
  kind: "Int" | "Float" | "String" | "Char" | "Bool"
  value: any
}
```

### IdentifierNode

```text
IdentifierNode {
  name: string
}
```

### UnaryExpressionNode

```text
UnaryExpressionNode {
  operator: "!" | "~" | "-"
  operand: ExpressionNode
}
```

### BinaryExpressionNode

```text
BinaryExpressionNode {
  operator: string
  left: ExpressionNode
  right: ExpressionNode
}
```

Rules:

- The operator string must represent one of the supported operators.
- The type checker validates operand compatibility.

### CallExpressionNode

```text
CallExpressionNode {
  callee: ExpressionNode
  arguments: List<ExpressionNode>
}
```

Rules:

- The callee is typically an IdentifierNode or another expression resolving to a callable.

### IndexExpressionNode

```text
IndexExpressionNode {
  target: ExpressionNode
  index: ExpressionNode
}
```

### ArrayLiteralNode

```text
ArrayLiteralNode {
  elements: List<ExpressionNode>
}
```

### ListLiteralNode

```text
ListLiteralNode {
  elements: List<ExpressionNode>
}
```

### MapLiteralNode

```text
MapLiteralNode {
  entries: List<MapEntryNode>
}
```

### MapEntryNode

```text
MapEntryNode {
  key: ExpressionNode
  value: ExpressionNode
}
```

### CastExpressionNode

```text
CastExpressionNode {
  expression: ExpressionNode
  targetType: TypeAnnotationNode
}
```

### InterpolatedStringNode

```text
InterpolatedStringNode {
  parts: List<StringPartNode>
}
```

### StringPartNode

```text
StringPartNode {
  kind: "text" | "expression"
  value: string | ExpressionNode
}
```

---

## 5. Type System Nodes

### TypeAnnotationNode

```text
TypeAnnotationNode {
  name: string
}
```

Rules:

- The name is the canonical type identifier, such as `Int`, `Float`, `String`, `Any`, or a collection type name.

### ParameterNode

```text
ParameterNode {
  name: Identifier
  typeAnnotation: TypeAnnotationNode
}
```

---

## 6. Semantic Expectations

### 6.1 Name Resolution

The parser should produce AST nodes that preserve identifiers as names. Name resolution is performed later by semantic analysis.

### 6.2 Type Inference

If a variable or constant declaration has no explicit type annotation, the AST still stores the declaration node and the initializer expression. The type checker infers the type from the initializer.

### 6.3 Any Type

The AST should preserve the explicit `Any` type annotation when present. It should not collapse `Any` into another type.

### 6.4 Casts

Cast expressions must be represented explicitly as `CastExpressionNode` rather than as normal binary operations.

### 6.5 Blocks and Scope

Blocks are represented as separate `BlockNode` instances. Scope analysis uses block structure, not token structure.

---

## 7. Example AST Sketch

Source:

```nexus
let x = 10
fn add(a: Int, b: Int): Int {
  return a + b
}
```

Conceptual AST:

```text
ProgramNode {
  statements: [
    VariableDeclarationNode {
      name: "x",
      typeAnnotation: null,
      initializer: LiteralNode { kind: "Int", value: 10 }
    },
    FunctionDeclarationNode {
      name: "add",
      parameters: [
        ParameterNode { name: "a", typeAnnotation: Int },
        ParameterNode { name: "b", typeAnnotation: Int }
      ],
      returnType: Int,
      body: BlockNode {
        statements: [
          ReturnNode {
            value: BinaryExpressionNode {
              operator: "+",
              left: IdentifierNode { name: "a" },
              right: IdentifierNode { name: "b" }
            }
          }
        ]
      }
    }
  ]
}
```
