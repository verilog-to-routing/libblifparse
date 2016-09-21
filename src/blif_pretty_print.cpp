#include <cstdio>
#include <cassert>

#include "blif_pretty_print.hpp"

namespace blifparse {

void BlifPrettyPrinter::start_model(std::string model_name) {
    printf(".model %s\n", model_name.c_str());
}

void BlifPrettyPrinter::inputs(std::vector<std::string> input_conns) {
    printf(".inputs \\\n");
    ++indent_level_;
    for(size_t i = 0; i < input_conns.size(); ++i) {
        printf("%s%s", indent().c_str(), input_conns[i].c_str());
        if(i != input_conns.size() - 1) {
            printf(" \\");
        }
        printf("\n");
    }
    --indent_level_;
}

void BlifPrettyPrinter::outputs(std::vector<std::string> output_conns) {
    printf(".outputs \\\n");

    ++indent_level_;
    for(size_t i = 0; i < output_conns.size(); ++i) {
        printf("%s%s", indent().c_str(), output_conns[i].c_str());
        if(i != output_conns.size() - 1) {
            printf(" \\");
        }
        printf("\n");
    }
    --indent_level_;

    printf("\n");
}

void BlifPrettyPrinter::start_names(std::vector<std::string> connections) {
    printf(".names ");
    for(size_t i = 0; i < connections.size(); ++i) {
        printf("%s", connections[i].c_str());
        if(i != connections.size() - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

void BlifPrettyPrinter::single_output_cover_row(std::vector<LogicValue> so_row) {
    for(size_t i = 0; i < so_row.size(); ++i) {
        switch(so_row[i]) {
            case LogicValue::FALSE:     printf("0"); break;
            case LogicValue::TRUE:      printf("1"); break;
            case LogicValue::DONT_CARE: printf("-"); break;
            default: assert(false);
        }
        if(i == so_row.size() - 2) {
            printf(" ");
        }
    }
    printf("\n");
}

void BlifPrettyPrinter::end_names() {
    printf("\n");
}

void BlifPrettyPrinter::latch(std::string input, std::string output, LatchType type, std::string control, LogicValue init) {
    printf(".latch \\\n");

    ++indent_level_;
    printf("%s%s \\\n", indent().c_str(), input.c_str());
    printf("%s%s \\\n", indent().c_str(), output.c_str());
    switch(type) {
        case LatchType::RISING_EDGE:    printf("%sre \\\n", indent().c_str()); break;
        case LatchType::FALLING_EDGE:   printf("%sfe \\\n", indent().c_str()); break;
        case LatchType::ACTIVE_HIGH:    printf("%sah \\\n", indent().c_str()); break;
        case LatchType::ACTIVE_LOW:     printf("%sal \\\n", indent().c_str()); break;
        case LatchType::ASYNCHRONOUS:   printf("%sas \\\n", indent().c_str()); break;
        case LatchType::UNSPECIFIED:    break;
        default: assert(false);

    }
    if(control.empty()) {
        printf("%sNIL \\\n", indent().c_str());
    } else {
        printf("%s%s \\\n", indent().c_str(), control.c_str());
    }
    switch(init) {
        case LogicValue::FALSE:     printf("%s0", indent().c_str()); break;
        case LogicValue::TRUE:      printf("%s1", indent().c_str()); break;
        case LogicValue::DONT_CARE: printf("%s2", indent().c_str()); break;
        case LogicValue::UNKOWN:    printf("%s3", indent().c_str()); break;
        default: assert(false);
    }
    printf("\n");
    --indent_level_;

    printf("\n");
}

void BlifPrettyPrinter::start_subckt(std::string model) {
    printf(".subckt %s \\\n", model.c_str());
    ++indent_level_;
}

void BlifPrettyPrinter::port_connection(std::string port, std::string net) {
    printf("%s%s=%s \\\n", indent().c_str(), port.c_str(), net.c_str());
}

void BlifPrettyPrinter::end_subckt() {
    --indent_level_;
    printf("\n");
}

void BlifPrettyPrinter::blackbox() {
    printf(".blackbox\n");
    printf("\n");
}

void BlifPrettyPrinter::end_model() {
    printf(".end\n");
    printf("\n");
}

std::string BlifPrettyPrinter::indent() {
    std::string indent_str;
    for(size_t i = 0; i < indent_level_; ++i) {
        indent_str += "    ";
    }
    return indent_str;
}

}
