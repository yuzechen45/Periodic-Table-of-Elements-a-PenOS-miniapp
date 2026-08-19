# miniapp-template

A GitHub template repository for building **miniapps** on [YouDao Dictionary Pen](https://cidian.youdao.com/pen/) (有道词典笔) devices.

The target hardware runs an **embedded Linux** system (busybox init). The build produces an `.amr` file via a cross-compilation toolchain.

## Supported Devices

| Device | Architecture |
|--------|-------------|
| A6P | ARMv7 (uclibc, bleeding-edge 2018.11) |
| X5 | ARMv7 (glibc, 2018.11) |
| P5 | AArch64 (glibc, 2018.11) |
| S6P | ARMv7 (glibc, 2018.11) |

## Quick Start

1. Click **"Use this template"** → **"Create a new repository"** on GitHub.
2. Clone your new repository.
3. Edit `jsapi/src/JSAPI.cpp` to register custom native JS API functions.
4. Edit `ui/src/pages/index/` to build your UI with Vue.
5. Push to `main` — GitHub Actions will build `.amr` artifacts for all four devices.

## Directory Structure

```
├── aiot-vue-cli/       # Vendored build tool (patched for Node.js compatibility)
├── jsapi/
│   ├── CMakeLists.txt  # CMake build config for the native C++ library
│   ├── src/
│   │   ├── JSAPI.cpp   # ← Register your JS API functions here
│   │   ├── JSAPI.hpp
│   │   ├── Fetch.*     # HTTP fetch utilities
│   │   ├── Database/   # SQLite database helpers
│   │   ├── Exceptions/ # Exception types
│   │   ├── AI/         # AI module
│   │   ├── IME/        # Input method engine
│   │   ├── ScanInput/  # Scan input handling
│   │   └── nlohmann/   # JSON library (header-only)
│   └── toolchains/     # Cross-compiler (downloaded at build time)
├── tools/
│   └── build.sh        # Main build script
├── ui/
│   ├── package.json    # Frontend package config
│   └── src/
│       ├── app.js      # Application lifecycle
│       ├── app.json    # Page routing config
│       ├── base-page.js
│       ├── pages/
│       │   └── index/  # Example index page
│       └── styles/     # Shared styles
├── pnpm-lock.yaml      # Dependency lock file
└── pnpm-workspace.yaml # pnpm workspace config
```

## Local Development

### Prerequisites

- **Node.js** 18+
- **pnpm** (installed via `npm install -g pnpm@latest-10`)
- **CMake** 3.10+
- A cross-compilation **toolchain** for your target device (see the table above)

### Manual Build

```bash
# 1. Install frontend dependencies
pnpm install -C ./ui

# 2. Download & extract toolchain into jsapi/toolchains/
mkdir -p jsapi/toolchains
wget -q <TOOLCHAIN_URL> -O jsapi/toolchains/<TOOLCHAIN_FILENAME>
tar -xjf jsapi/toolchains/<TOOLCHAIN_FILENAME> -C jsapi/toolchains

# 3. Download & extract versionInfo into jsapi/
wget -q <VERSIONINFO_URL>
tar -xf <VERSIONINFO_FILENAME> -C jsapi

# 4. Run the build
./tools/build.sh -a

# The .amr file will be in dist/
```

## Extending: Registering New JSAPI Modules

Edit `jsapi/src/JSAPI.cpp` and add your module export in `module_init(...)`, then register it via `custom_init_jsapis()`:

```cpp
static int module_init(JSContext *ctx, JSModuleDef *m)
{
    auto env = JQUTIL_NS::JQModuleEnv::CreateModule(ctx, m, "custom");

    // createMyModule(...) should return a JSValue (usually from a JQPublishObject factory).
    env->setModuleExport("MyModule", createMyModule(env.get()));
    env->setModuleExportDone(JS_UNDEFINED, exportList);
    return 0;
}

extern "C" JQUICK_EXPORT void custom_init_jsapis()
{
    registerCModuleLoader("custom", &custom_module_load);
}
```

## CI / GitHub Actions

The repository includes a unified GitHub Actions workflow (`.github/workflows/build.yml`) that builds for all four devices using a `strategy.matrix`. On every push to `main` (or manual dispatch), it:

1. Installs pnpm and frontend dependencies
2. Downloads the device-specific toolchain and versionInfo
3. Runs `tools/build.sh`
4. Uploads `dist/*.amr` as artifacts (`miniapp-<device>`, retained for 30 days)

## Acknowledgements

This template is based on the work of:

- **[langningchen/miniapp](https://github.com/langningchen/miniapp)** — The upstream original project (GPL-3.0). The vendored `aiot-vue-cli/` and `tools/build.sh` come directly from this repository.
- **[penosext/miniapp](https://github.com/penosext/miniapp)** — A community fork with CI workflows supporting multiple devices.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).
