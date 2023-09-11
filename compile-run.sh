clang++ -o eva.llvm eva-llvm.cpp `llvm-config --cxxflags --system-libs --libs core` -L/opt/homebrew/Cellar/llvm/16.0.6/lib

./eva.llvm

lli ./out.ll

echo $?

printf "\n"