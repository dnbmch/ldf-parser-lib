# ldf-parser-lib

Prebuilt static library, public headers, and protobuf schema for the **ldf-parser** — a C++17 library that parses LIN Description Files (LDF) into Protocol Buffer messages.

**Author:** Danube Mechatronics Kft.

## Downloads

Prebuilt static libraries are available on the [Releases](https://github.com/dnbmch/ldf-parser-lib/releases) page:

| Artifact | Platform |
|----------|----------|
| `ldfparser-x86_64-windows-mingw` | Windows MinGW GCC (.a) |
| `ldfparser-x86_64-linux-gnu` | Linux x86_64 (.a) |
| `ldfparser-aarch64-linux-gnu` | Linux ARM64 (.a) |
| `ldfparser-x86_64-windows-msvc` | Windows MSVC (.lib) |

## Quick Start

Each platform archive contains a complete install prefix: matching public and
protobuf-generated headers, the static library, schemas, CMake package files and
`share/ldfparser/build-info.json`. Set `CMAKE_PREFIX_PATH` to the extracted prefix;
CMake resolves `ldfparser::ldfparser` through `find_package(ldfparser CONFIG REQUIRED)`.
Use a compatible compiler/runtime and the producer's exact protobuf version. Dependency
libraries are supplied separately by your toolchain. Do not regenerate C++ headers
against a prebuilt binary. Historical split archives do not satisfy this contract;
choose a complete package from a deliberate future release or a local producer install.

Package CI runs when repository variable `PARSER_PACKAGE_TAG` names an existing
complete-package release, and supports manual dispatch. No tag is selected by default.


```bash
# 1. Clone this repo
git clone https://github.com/dnbmch/ldf-parser-lib.git
cd ldf-parser-lib

# 2. Download and extract the prebuilt library for your platform
#    (from the Releases page, extract into package/)
TAG=... # Select an existing complete-package release tag.
mkdir -p package
tar xzf ldfparser-x86_64-linux-gnu-${TAG}.tar.gz -C package/

# 3. Build the examples
cmake -B build -DCMAKE_PREFIX_PATH=/absolute/path/to/package
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
#include "ldf/ldffile.h"
#include "ldf/extract.h"

auto file = ldffile::Loader::readLdfFile("path/to/file.ldf");
ldf::LdfFile result = ldf::extract::extractFile(*file);

for (const auto& frame : result.frames()) {
    // Access signals, encodings, schedule tables, etc.
}
```

## Build Requirements

- C++17 compiler (GCC, Clang, or MSVC)
- Protocol Buffers (protobuf) runtime library

## License

Dual licensed: GPL-2.0 or Commercial. See [LICENSE.md](LICENSE.md).
