// examples/signal_dump/main.cpp
// Signal dump CLI: parse LDF and print a flat signal table for quick reference.
//
// Output format (TSV-like):
//   SIGNAL_NAME  FRAME  ID  START_BIT  BITS  FACTOR  OFFSET  UNIT  PUBLISHER  ENCODING
//
// Build (from project root):
//   g++ -std=c++17 -O2 -I ldfparser_dnbm -I gen -I /c/msys64/mingw64/include \
//       examples/signal_dump/main.cpp ldfparser_dnbm/ldffile.cpp \
//       ldfparser_dnbm/extract.cpp ldfparser_dnbm/extract_frame.cpp \
//       ldfparser_dnbm/extract_encoding.cpp gen/common.pb.cc gen/ldf.pb.cc \
//       -L /c/msys64/mingw64/lib -lprotobuf.dll -o signal_dump.exe

#include "ldf/ldffile.h"
#include "ldf/extract.h"

#include <iomanip>
#include <iostream>
#include <string>

static void printRow(const std::string& name, const std::string& frame,
                     uint32_t id, uint32_t startBit, uint32_t bits,
                     const std::string& factor, const std::string& offset,
                     const std::string& unit, const std::string& publisher,
                     const std::string& encoding) {
    std::cout << std::left
              << std::setw(32) << name
              << std::setw(20) << frame
              << std::setw(5) << id
              << std::setw(6) << startBit
              << std::setw(6) << bits
              << std::setw(10) << factor
              << std::setw(10) << offset
              << std::setw(12) << unit
              << std::setw(16) << publisher
              << encoding << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: signal_dump <file.ldf>\n";
        return 1;
    }

    auto raw = ldffile::Loader::readLdfFile(argv[1]);
    if (!raw) {
        std::cerr << "Failed to parse: " << argv[1] << "\n";
        return 1;
    }
    ldf::LdfFile file = ldf::extract::extractFile(raw.get());

    // Header
    printRow("SIGNAL", "FRAME", 0, 0, 0, "FACTOR", "OFFSET", "UNIT", "PUBLISHER", "ENCODING");
    std::cout << std::string(130, '-') << "\n";

    for (int i = 0; i < file.signals_size(); i++) {
        const auto& sig = file.signals(i);

        std::string factor = "-";
        std::string offset = "-";
        std::string unit = "";
        std::string encName = "(raw)";

        if (sig.has_encoding()) {
            encName = sig.encoding().encoding_name();
            if (sig.encoding().physical_values_size() > 0) {
                const auto& pv = sig.encoding().physical_values(0);
                factor = std::to_string(pv.factor());
                offset = std::to_string(pv.offset());
                unit = pv.unit();
            }
            if (sig.encoding().logical_values_size() > 0 &&
                sig.encoding().physical_values_size() == 0) {
                // Pure enum signal
                encName += " (enum:" + std::to_string(sig.encoding().logical_values_size()) + ")";
            }
        }

        printRow(sig.name(), sig.frame_name(), sig.frame_id(),
                 sig.start_bit(), sig.bit_length(),
                 factor, offset, unit, sig.publisher(), encName);
    }

    std::cout << "\n" << file.signals_size() << " signals total.\n";
    return 0;
}
