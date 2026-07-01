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

/// One LIN signal: name, bit length, per-byte init values, and publisher/subscribers.
struct RawSignal {
    std::string name;
    uint32_t bit_length = 0;
    std::vector<int64_t> init_values;   // one element for scalar, all for array
    std::string publisher;
    std::vector<std::string> subscribers;
};

/// A signal's placement inside a frame: signal name and its start bit.
struct RawFrameSignal {
    std::string signal_name;
    uint32_t start_bit = 0;
};

/// One LIN frame: id, publisher, byte length (0 = unspecified), and member signals.
struct RawFrame {
    std::string name;
    uint32_t id = 0;
    std::string publisher;
    uint32_t length = 0;            // 0 = unspecified
    std::vector<RawFrameSignal> signals;
};

/// One piecewise physical range: raw min/max mapped by factor/offset, with a unit.
struct RawPhysicalValue {
    uint64_t min_raw = 0;
    uint64_t max_raw = 0;
    double factor = 0.0;
    double offset = 0.0;
    std::string unit;
};

/// One logical (enum) encoding: a raw value and its textual description.
struct RawLogicalValue {
    int64_t value = 0;
    std::string description;
};

/// A named encoding: its piecewise physical ranges and logical value entries.
struct RawSignalEncodingType {
    std::string name;
    std::vector<RawPhysicalValue> physical_values;  // piecewise ranges
    std::vector<RawLogicalValue> logical_values;
};

/// Binds an encoding name to the signals that use it.
struct RawSignalRepresentation {
    std::string encoding_name;
    std::vector<std::string> signal_names;
};

// Kind of a schedule entry. A schedule entry is either a regular frame slot or
// one of the LIN 2.x / ISO 17987 diagnostic commands.
enum class ScheduleCommandKind {
    Frame = 0,            // command names a regular frame
    AssignNad,
    ConditionalChangeNad,
    AssignFrameId,
    AssignFrameIdRange,
    UnassignFrameId,
    DataDump,
    SaveConfiguration,
    FreeFormat,
    AssignNadViaSnpd,
    AssignNadViaJ2602,
};

/// One schedule-table slot: a frame or a typed diagnostic command, plus its delay.
struct RawScheduleEntry {
    ScheduleCommandKind kind = ScheduleCommandKind::Frame;
    std::string frame_name;              // set when kind == Frame
    std::vector<std::string> arguments;  // diagnostic-command brace tokens
    double delay_ms = 0.0;
};

/// A named schedule table and its ordered entries.
struct RawScheduleTable {
    std::string name;
    std::vector<RawScheduleEntry> entries;
};

/// One LIN node: master/slave role, timing, and J2602 master extensions.
struct RawNode {
    std::string name;
    bool is_master = false;
    double timebase_ms = 0.0;
    double jitter_ms = 0.0;
    // J2602 master extensions
    uint32_t max_frame_bits = 0;
    double duty_cycle_pct = 0.0;
};

/// A configurable frame entry: frame name and an optional assigned message id.
struct RawConfigurableFrame {
    std::string frame_name;
    uint32_t message_id = 0;
    bool has_message_id = false;
};

/// Per-node attributes: protocol, NAD, timing, configurable frames, and J2602 extensions.
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

/// An event-triggered frame: collision resolver, id, and associated unconditional frames.
struct RawEventTriggeredFrame {
    std::string name;
    std::string collision_resolver;
    uint32_t id = 0;
    std::vector<std::string> associated_frames;
};

/// Maps a node name to its diagnostic node address (NAD).
struct RawDiagnosticAddress {
    std::string node_name;
    uint32_t nad = 0;
};

/// A named signal group and its member signals.
struct RawSignalGroup {
    std::string name;
    uint32_t group_size = 0;
    std::vector<RawFrameSignal> signals;
};

// ISO 17987 diagnostic types

/// ISO 17987 diagnostic signal: name, bit length, and init value.
struct RawDiagnosticSignal {
    std::string name;
    uint32_t bit_length = 0;
    int64_t init_value = 0;
};

/// ISO 17987 diagnostic frame: id and member signals.
struct RawDiagnosticFrame {
    std::string name;
    uint32_t id = 0;
    std::vector<RawFrameSignal> signals;
};

// ---------------------------------------------------------------------------
// Parse-time diagnostics
// ---------------------------------------------------------------------------

enum class DiagnosticSeverity {
    Warning = 1,   // lossy/guessed value; parsing continued
    Dropped = 2,   // a record or section was skipped entirely
};

/// One parse-time diagnostic: source location, message, and severity.
struct RawDiagnostic {
    std::string location;   // section / record name
    std::string message;
    DiagnosticSeverity severity = DiagnosticSeverity::Warning;
};

// ---------------------------------------------------------------------------
// Top-level container
// ---------------------------------------------------------------------------

/// Parsed LDF: global header fields, all section vectors, and parse diagnostics.
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

    std::vector<RawDiagnostic> diagnostics;   // lossy/guessed parse events
};

// ---------------------------------------------------------------------------
// Loader
// ---------------------------------------------------------------------------

struct Loader {
    static std::unique_ptr<LdfFile> readLdfFile(const std::string& path);
};

} // namespace ldffile
