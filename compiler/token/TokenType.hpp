#pragma once

namespace nexus::token
{

enum class TokenType
{
    Identifier,

    IntegerLiteral,
    FloatLiteral,
    StringLiteral,


    // 키워드
    Let,
    Const,
    Function,
    Return,

    If,
    Else,
    Elif,
    While,
    Break,
    Continue,

    Print,


    // 타입
    TypeInteger,
    TypeFloat,
    TypeString,
    TypeBoolean,


    // 연산자
    Plus,
    Minus,
    Multiply,
    Divide,

    Assign,

    Equal,
    NotEqual,

    Greater,
    Less,
    GreaterEqual,
    LessEqual,
    Not,


    // 기호
    Colon,
    Comma,

    LeftParen,
    RightParen,

    LeftBrace,
    RightBrace,

    Unknown,
    EndOfFile
};

}