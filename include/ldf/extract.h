#pragma once

#include "ldf/ldffile.h"
#include "ldf/ldf.pb.h"
#include "ldf/common.pb.h"

namespace ldf::extract {

// Top-level entry point
ldf::LdfFile extractFile(ldffile::LdfFile* file);

// Per-concept extraction
ldf::Node extractNode(const ldffile::RawNode& raw);
ldf::Signal extractSignal(const ldffile::RawSignal& raw);
ldf::Frame extractFrame(const ldffile::RawFrame& raw);
ldf::SignalEncodingType extractEncodingType(const ldffile::RawSignalEncodingType& raw);
ldf::ScheduleTable extractScheduleTable(const ldffile::RawScheduleTable& raw);
ldf::EventTriggeredFrame extractEventTriggeredFrame(const ldffile::RawEventTriggeredFrame& raw);
ldf::DiagnosticAddress extractDiagnosticAddress(const ldffile::RawDiagnosticAddress& raw);
ldf::SignalGroup extractSignalGroup(const ldffile::RawSignalGroup& raw);
ldf::DiagnosticSignal extractDiagnosticSignal(const ldffile::RawDiagnosticSignal& raw);
ldf::DiagnosticFrame extractDiagnosticFrame(const ldffile::RawDiagnosticFrame& raw);

// Enum converters
ldf::NodeRole nodeRoleFromRaw(bool is_master);

// Enrichment: merge frame info + encoding into signals
void enrichSignals(ldf::LdfFile& result, const ldffile::LdfFile* file);

// Merge node attributes into slave nodes
void mergeNodeAttributes(ldf::LdfFile& result,
                         const std::vector<ldffile::RawNodeAttributes>& attrs);

} // namespace ldf::extract
