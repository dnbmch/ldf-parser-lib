// examples/basic/main.cpp
// Basic integration example: parse LDF file, walk extracted protobuf data.
//
// Build (from project root):
//   g++ -std=c++17 -O2 -I ldfparser_dnbm -I gen -I /c/msys64/mingw64/include \
//       examples/basic/main.cpp ldfparser_dnbm/ldffile.cpp ldfparser_dnbm/extract.cpp \
//       ldfparser_dnbm/extract_frame.cpp ldfparser_dnbm/extract_encoding.cpp \
//       gen/common.pb.cc gen/ldf.pb.cc \
//       -L /c/msys64/mingw64/lib -lprotobuf.dll -o basic_example.exe

#include "ldffile.h"
#include "extract.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: basic_example <file.ldf>\n";
        return 1;
    }

    // Step 1: Parse the LDF file into raw data structures
    auto raw = ldffile::Loader::readLdfFile(argv[1]);
    if (!raw) {
        std::cerr << "Failed to parse: " << argv[1] << "\n";
        return 1;
    }

    // Step 2: Extract into typed protobuf messages
    ldf::v1::LdfFile file = extract::extractFile(raw.get());

    // Step 3: Walk the extracted data
    std::cout << "=== LDF File Summary ===\n";
    std::cout << "Protocol: " << file.lin_protocol_version() << "\n";
    std::cout << "Speed: " << file.lin_speed_kbps() << " kbps\n";
    std::cout << "Master: " << file.master().name() << "\n";
    std::cout << "\n";

    // Slave nodes
    std::cout << "--- Slaves (" << file.slaves_size() << ") ---\n";
    for (int i = 0; i < file.slaves_size(); i++) {
        const auto& slave = file.slaves(i);
        std::cout << "  " << slave.name();
        if (slave.has_attributes())
            std::cout << " (NAD=0x" << std::hex << slave.attributes().configured_nad()
                      << std::dec << ")";
        std::cout << "\n";
    }
    std::cout << "\n";

    // Frames with their signals
    std::cout << "--- Frames (" << file.frames_size() << ") ---\n";
    for (int i = 0; i < file.frames_size(); i++) {
        const auto& f = file.frames(i);
        std::cout << "  " << f.name() << " (ID=" << f.id()
                  << ", pub=" << f.publisher() << ")\n";
        for (int j = 0; j < f.signals_size(); j++) {
            std::cout << "    [" << f.signals(j).start_bit() << "] "
                      << f.signals(j).signal_name() << "\n";
        }
    }
    std::cout << "\n";

    // Signals with encoding info
    std::cout << "--- Signals (" << file.signals_size() << ") ---\n";
    for (int i = 0; i < file.signals_size(); i++) {
        const auto& sig = file.signals(i);
        std::cout << "  " << sig.name()
                  << " (" << sig.bit_length() << " bits"
                  << ", pub=" << sig.publisher() << ")";

        if (sig.has_encoding()) {
            const auto& enc = sig.encoding();
            std::cout << " [" << enc.encoding_name();
            if (enc.physical_values_size() > 0) {
                const auto& pv = enc.physical_values(0);
                std::cout << ": factor=" << pv.factor()
                          << " offset=" << pv.offset();
                if (!pv.unit().empty())
                    std::cout << " " << pv.unit();
            }
            if (enc.logical_values_size() > 0)
                std::cout << ", " << enc.logical_values_size() << " enums";
            std::cout << "]";
        }
        std::cout << "\n";
    }

    // Serialize to binary for downstream consumption
    std::string binary;
    file.SerializeToString(&binary);
    std::cout << "\nSerialized to " << binary.size() << " bytes protobuf binary.\n";

    return 0;
}
