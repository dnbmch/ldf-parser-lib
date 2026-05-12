// lib/examples/json_export/main.cpp
// JSON export example: parse LDF -> extract -> serialize to JSON via protobuf.
//
// Uses protobuf's built-in JSON serialization (MessageToJsonString).
// Output can be consumed by Python, web UIs, databases, etc.
//
// Built by the main CMakeLists.txt as the `ldf_example_json_export` target.

#include "ldf/ldffile.h"
#include "ldf/extract.h"

#include <google/protobuf/util/json_util.h>

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: json_export <file.ldf> [output.json]\n";
        return 1;
    }

    // Parse + extract
    auto raw = ldffile::Loader::readLdfFile(argv[1]);
    if (!raw) {
        std::cerr << "Failed to parse: " << argv[1] << "\n";
        return 1;
    }
    ldf::LdfFile file = ldf::extract::extractFile(raw.get());

    // Configure JSON output
    google::protobuf::util::JsonPrintOptions opts;
    opts.add_whitespace = true;
    opts.always_print_enums_as_ints = false;
    opts.preserve_proto_field_names = true;

    std::string json;
    auto status = google::protobuf::util::MessageToJsonString(file, &json, opts);
    if (!status.ok()) {
        std::cerr << "JSON serialization failed: " << status.ToString() << "\n";
        return 1;
    }

    // Output to file or stdout
    if (argc >= 3) {
        std::ofstream out(argv[2]);
        if (!out.is_open()) {
            std::cerr << "Failed to open output: " << argv[2] << "\n";
            return 1;
        }
        out << json;
        std::cout << "Written " << json.size() << " bytes to " << argv[2] << "\n";
    } else {
        std::cout << json << "\n";
    }

    return 0;
}
