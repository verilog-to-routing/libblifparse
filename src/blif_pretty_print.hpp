#ifndef BLIF_PRETTY_PRINT
#define BLIF_PRETTY_PRINT
#include "blifparse.hpp"

namespace blifparse {

//An example callback which pretty-prints to stdout
//the BLIF which is being parsed 
class BlifPrettyPrinter : public Callback {
    public:
        void start_model(std::string model_name) override;
        void inputs(std::vector<std::string> inputs) override;
        void outputs(std::vector<std::string> outputs) override;

        void start_names(std::vector<std::string> connections) override;
        void single_output_cover_row(std::vector<LogicValue> so_row) override;
        void end_names() override;

        void latch(std::string input, std::string output, LatchType type, std::string control, LogicValue init) override;

        void start_subckt(std::string model) override;
        void port_connection(std::string port, std::string net) override;
        void end_subckt() override;

        void blackbox() override;

        void end_model() override;
    private:
        std::string indent();

        size_t indent_level_ = 0;

};

}
#endif
