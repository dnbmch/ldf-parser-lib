# ldf-parser-lib

Prebuilt static library, public headers, and protobuf schema for the **ldf-parser** — a C++17 library that parses LIN Description Files (LDF) into Protocol Buffer messages.

**Author:** Danube Mechatronics Kft.

## Downloads

Prebuilt static libraries are available on the [Releases](https://github.com/dnbmch/ldf-parser-lib/releases) page:

| Artifact | Platform |
|----------|----------|
| `ldfextract-x86_64-windows-mingw` | Windows MinGW GCC (.a) |
| `ldfextract-x86_64-linux-gnu` | Linux x86_64 (.a) |
| `ldfextract-aarch64-linux-gnu` | Linux ARM64 (.a) |
| `ldfextract-headers` | Public headers and proto files |

## Quick Start

```bash
# 1. Clone this repo
git clone https://github.com/dnbmch/ldf-parser-lib.git
cd ldf-parser-lib

# 2. Download and extract the prebuilt library for your platform
#    (from the Releases page, extract into lib/)
mkdir -p lib
tar xzf ldfextract-x86_64-linux-gnu-v0.2.0.tar.gz -C lib/

# 3. Build the examples
cmake -B build -DLDF_LIB_DIR=lib
cmake --build build

# 4. Run
./build/ldf_basic path/to/file.ldf
```

## Contents

| Directory | Description |
|-----------|-------------|
| `include/` | Public C++ headers (`ldffile.h`, `extract.h`) |
| `proto/` | Protobuf schema files (`.proto`) for multi-language binding generation |
| `examples/` | Example applications (basic summary, JSON export, signal dump) |

## Integration

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
