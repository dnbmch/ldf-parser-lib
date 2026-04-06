# ldf-parser-lib

Prebuilt static library, public headers, and protobuf schema for the **ldf-parser** — a C++17 library that parses LIN Description Files (LDF) into Protocol Buffer messages.

**Author:** Danube Mechatronics Kft.

## Contents

| Directory | Description |
|-----------|-------------|
| `include/` | Public C++ headers (`ldffile.h`, `extract.h`) |
| `proto/` | Protobuf schema files (`.proto`) for multi-language binding generation |
| `examples/` | Example applications (basic summary, JSON export, signal dump) |

## Quick Start

```cpp
#include "ldffile.h"
#include "extract.h"

auto file = ldffile::Loader::readLdfFile("path/to/file.ldf");
ldf::v1::LdfFile result = extract::extractFile(file.get());

for (const auto& frame : result.frames()) {
    // Access signals, encodings, schedule tables, etc.
}
```

## Build Requirements

- C++17 compiler (GCC, Clang, or MSVC)
- Protocol Buffers (protobuf) runtime library

## License

Dual licensed: GPL-2.0 or Commercial. See [LICENSE.md](LICENSE.md).
