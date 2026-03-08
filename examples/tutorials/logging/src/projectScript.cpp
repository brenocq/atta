//--------------------------------------------------
// Logging
// projectScript.cpp
// Date: 2021-11-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "projectScript.h"

//---------------------------------//
//---------- Description ----------//
//---------------------------------//
// Create colored output logs with parameter pack
// Colors: [w]  -> white
//         [r]  -> red
//         [g]  -> green
//         [b]  -> blue
//         [c]  -> cyan
//         [m]  -> magenta
//         [y]  -> yellow
//         [k]  -> black
//         [*w]  -> bold white
//         [*r]  -> bold red
//         [*g]  -> bold green
//         [*b]  -> bold blue
//         [*c]  -> bold cyan
//         [*m]  -> bold magenta
//         [*y]  -> bold yellow
//         [*k]  -> bold black
//         []   -> reset to default color
//
// Argument substitution: $0 -> first argument
//                        $1 -> second argument
//                        ...

struct Test {
    int val1 = 0;
    float val2 = 20.0f;
    Test(int v1, float v2) : val1(v1), val2(v2) {}
};
// We can define this operator to make a class printable
inline std::ostream& operator<<(std::ostream& os, const Test& t) { return os << "Test{val1:" << t.val1 << ", val2:" << t.val2 << "}"; }

void Project::onLoad() {
    LOG_VERBOSE("Project", "---------- Logging Example ----------");
    // Logging modes
    LOG_VERBOSE("Project", "Verbose Hello World!");
    LOG_DEBUG("Project", "Debug Hello World!");
    LOG_SUCCESS("Project", "Success Hello World!");
    LOG_WARN("Project", "Warn Hello World!");
    LOG_ERROR("Project", "Error Hello World!");

    // Changing text color
    LOG_DEBUG("Project", "[w]Hello [r]World!");
    LOG_DEBUG("Project", "[*g]Hello [c]World!");
    LOG_DEBUG("Project", "[r]R[*g]a[y]i[*c]n[m]b[*w]o[*m]w");
    LOG_DEBUG("Project", "[w]Hello []World!");
    LOG_SUCCESS("Project", "[w]Hello []World!");

    // Printing variables
    int i = 10;
    std::vector<float> vec = {0, 1, 2, 3, 4};
    LOG_DEBUG("Project", "int: $0", i);
    LOG_DEBUG("Project", "int: $x0", 0xcafe);
    LOG_DEBUG("Project", "int: $b0", 0b101010);
    LOG_DEBUG("Project", "float: $0", 3.1415f);
    LOG_DEBUG("Project", "pointer: $0", (void*)&i);

    LOG_DEBUG("Project",
              "\n - [y]int[]: [*w]$0[]\n - [y]float[]: [*w]$1[]"
              "\n - [y]char[]: [*w]$2[]\n - [y]string[]: [*w]$3[]\n - [y]vector[]: [*w]$4[]",
              i, 20.0f, 'a', "testing", vec);

    // It is possible to print any class that supports << for std::ostream
    LOG_DEBUG("Project", "vec4: [y]$0", atta::vec4(1.0f, 2.0f, 3.0f, 4.0f));
    LOG_DEBUG("Project", "Test: [g]$0", Test(0, 10.0f));
}
