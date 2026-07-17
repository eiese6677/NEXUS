#pragma once

#include "Bytecode.hpp"
#include "Value.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace nexus::vm
{

struct CallFrame
{
    std::string functionName;

    // 함수 지역 변수 + 매개변수
    std::unordered_map<std::string, Value> locals;

    // 함수 종료 후 돌아갈 위치
    size_t returnAddress;
};


class VirtualMachine
{
private:
    // Operand stack
    std::vector<Value> stack;

    // 전역 변수
    std::unordered_map<std::string, Value> globals;

    // 실행 중인 함수 스택
    std::vector<CallFrame> callStack;


    Value LoadVariable(const std::string& name);
    void StoreVariable(
        const std::string& name,
        const Value& value
    );

public:
    void Execute(const Bytecode& bytecode);
};

}