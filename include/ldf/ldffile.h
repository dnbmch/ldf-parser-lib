#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ldffile {

// ---------------------------------------------------------------------------
// Raw structs mirroring LDF sections (populated by parser, consumed by extract)
// ---------------------------------------------------------------------------

struct RawSignal {
    std::string name;
    uint32_t bit_length = 0;
    int64_t init_value = 0;
    std::string publisher;
    std::vector<std::string> subscribers;
};

struct RawFrameSignal {
    std::string signal_name;
    uint32_t start_bit = 0;
};

struct RawFrame {
    std::string name;
    uint32_t id = 0;
    std::string publisher;
    uint32_t length = 0;            // 0 = unspecified
    std::vector<RawFrameSignal> signals;
};

struct RawPhysicalValue {
    uint64_t min_raw = 0;
    uint64_t max_raw = 0;
    double factor = 0.0;
    double offset = 0.0;
    std::string unit;
};

struct RawLogicalValue {
    int64_t value = 0;
    std::string description;
};

struct RawSignalEncodingType {
    std::string name;
    std::vector<RawPhysicalValue> physical_values;  // piecewise ranges
    std::vector<RawLogicalValue> logical_values;
};

struct RawSignalRepresentation {
    std::string encoding_name;
    std::vector<std::string> signal_names;
};

struct RawScheduleEntry {
    std::string command;            // frame name or diagnostic command text
    double delay_ms = 0.0;
};

struct RawScheduleTable {
    std::string name;
    std::vector<RawScheduleEntry> entries;
};

struct RawNode {
    std::string name;
    bool is_master = false;
    double timebase_ms = 0.0;
    double jitter_ms = 0.0;
    // J2602 master extensions
    uint32_t max_frame_bits = 0;
    double duty_cycle_pct = 0.0;
};

struct RawConfigurableFrame {
    std::string frame_name;
    uint32_t message_id = 0;
    bool has_message_id = false;
};

struct RawNodeAttributes {
    std::string node_name;
    std::string lin_protocol;
    uint32_t configured_nad = 0;
    uint32_t initial_nad = 0;
    bool has_initial_nad = false;
    std::vector<uint32_t> product_id;
    std::string response_error;
    std::vector<std::string> fault_state_signals;
    double p2_min_ms = 0.0;
    double st_min_ms = 0.0;
    double n_as_timeout_ms = 0.0;
    double n_cr_timeout_ms = 0.0;
    std::vector<RawConfigurableFrame> configurable_frames;
    // J2602 extensions
    double response_tolerance_pct = 0.0;
    double wakeup_time_ms = 0.0;
    double poweron_time_ms = 0.0;
};

struct RawEventTriggeredFrame {
    std::string name;
    std::string collision_resolver;
    uint32_t id = 0;
    std::vector<std::string> associated_frames;
};

struct RawDiagnosticAddress {
    std::string node_name;
    uint32_t nad = 0;
};

struct RawSignalGroup {
    std::string name;
    uint32_t group_size = 0;
    std::vector<RawFrameSignal> signals;
};

// ISO 17987 diagnostic types

struct RawDiagnosticSignal {
    std::string name;
    uint32_t bit_length = 0;
    int64_t init_value = 0;
};

struct RawDiagnosticFrame {
    std::string name;
    uint32_t id = 0;
    std::vector<RawFrameSignal> signals;
};

// ---------------------------------------------------------------------------
// Top-level container
// ---------------------------------------------------------------------------

struct LdfFile {
    std::string lin_protocol_version;
    std::string lin_language_version;
    double lin_speed_kbps = 0.0;
    std::string channel_name;
    std::string ldf_file_revision;      // ISO 17987
    bool big_endian_signals = false;    // LIN_sig_byte_order_big_endian

    std::vector<RawNode> nodes;
    std::vector<RawSignal> signals;
    std::vector<RawFrame> frames;
    std::vector<RawSignalEncodingType> signal_encoding_types;
    std::vector<RawSignalRepresentation> signal_representations;
    std::vector<RawScheduleTable> schedule_tables;
    std::vector<RawNodeAttributes> node_attributes;
    std::vector<RawEventTriggeredFrame> event_triggered_frames;
    std::vector<RawDiagnosticAddress> diagnostic_addresses;
    std::vector<RawSignalGroup> signal_groups;
    std::vector<RawDiagnosticSignal> diagnostic_signals;   // ISO 17987
    std::vector<RawDiagnosticFrame> diagnostic_frames;     // ISO 17987
};

// ---------------------------------------------------------------------------
// Loader
// ---------------------------------------------------------------------------

struct Loader {
    static std::unique_ptr<LdfFile> readLdfFile(const std::string& path);
};

} // namespace ldffile
