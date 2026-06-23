#pragma once

#include "ldf/ldffile.h"
#include "ldf/ldf.pb.h"
#include "ldf/common.pb.h"

namespace ldf::extract {

// Top-level entry point: transform a parsed LDF into the typed proto contract.
ldf::LdfFile extractFile(const ldffile::LdfFile& file);

} // namespace ldf::extract
