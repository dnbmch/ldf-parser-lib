#pragma once

#include "ldffile.h"
#include "ldf.pb.h"
#include "common.pb.h"

namespace extract {

// Top-level entry point
ldf::v1::LdfFile extractFile(ldffile::LdfFile* file);

// Per-concept extraction
ldf::v1::Node extractNode(const ldffile::RawNode& raw);
ldf::v1::Signal extractSignal(const ldffile::RawSignal& raw);
ldf::v1::Frame extractFrame(const ldffile::RawFrame& raw);
ldf::v1::SignalEncodingType extractEncodingType(const ldffile::RawSignalEncodingType& raw);
ldf::v1::ScheduleTable extractScheduleTable(const ldffile::RawScheduleTable& raw);
ldf::v1::EventTriggeredFrame extractEventTriggeredFrame(const ldffile::RawEventTriggeredFrame& raw);
ldf::v1::DiagnosticAddress extractDiagnosticAddress(const ldffile::RawDiagnosticAddress& raw);
ldf::v1::SignalGroup extractSignalGroup(const ldffile::RawSignalGroup& raw);
ldf::v1::DiagnosticSignal extractDiagnosticSignal(const ldffile::RawDiagnosticSignal& raw);
ldf::v1::DiagnosticFrame extractDiagnosticFrame(const ldffile::RawDiagnosticFrame& raw);

// Enum converters
ldf::v1::NodeRole nodeRoleFromRaw(bool is_master);

// Enrichment: merge frame info + encoding into signals
void enrichSignals(ldf::v1::LdfFile& result, const ldffile::LdfFile* file);

// Merge node attributes into slave nodes
void mergeNodeAttributes(ldf::v1::LdfFile& result,
                         const std::vector<ldffile::RawNodeAttributes>& attrs);

} // namespace extract
