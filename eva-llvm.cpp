#include <string>

#include "./src/EvaLLVM.hpp"

int main(int argc, char const* argv[]) {
    std::string program = R"(

        (printf "Value: %d" 42)

    )";

    EvaLLVM vm;
    vm.exec(program);

    return 0;
}