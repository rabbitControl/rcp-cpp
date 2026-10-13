#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "src/StringStreamWriter.h"
#include "src/1.0/RcpInt.h"


namespace rcp {
namespace v10 {


TEST_CASE("RcpInt benchmark")
{
    // BENCHMARK("Fibonacci 20") {
    //     StringStreamWriter writer;
    //     return RcpInt(20).write(writer, true);
    // };

    StringStreamWriter writer;
    RcpInt num(pow(2, 28) -1);


    long long micros = 0;

    for (int i = 0; i < 1000000; ++i)
    {
        writer.clear();
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        num.write(writer, true);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        micros += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    }

    std::cout << "Time difference = " << micros/1000 << "[us]" << std::endl;

}

}
}
