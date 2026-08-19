// Copyright (C) 2025 Template Author
//
// This file is part of miniapp-template.
//
// miniapp-template is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// miniapp-template is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with miniapp-template. If not, see <https://www.gnu.org/licenses/>.

#include "JSAPI.hpp"

#include <jsmodules/JSCModuleExtension.h>
#include <jquick_config.h>
#include <string>
#include <vector>

#include "AI/JSAI.hpp"
#include "IME/JSIME.hpp"
#include "ScanInput/JSScanInput.hpp"

using namespace JQUTIL_NS;

static std::vector<std::string> exportList = {
    // Uncomment module names below when enabling their exports.
    // "AI",
    // "IME",
    // "ScanInput",
};

static int module_init(JSContext *ctx, JSModuleDef *m)
{
    auto env = JQUTIL_NS::JQModuleEnv::CreateModule(ctx, m, "custom");

    // env->setModuleExport("AI", createAI(env.get()));
    // env->setModuleExport("IME", createIME(env.get()));
    // env->setModuleExport("ScanInput", createScanInput(env.get()));

    // Add your own module exports here, e.g.:
    // env->setModuleExport("MyModule", createMyModule(env.get()));

    env->setModuleExportDone(JS_UNDEFINED, exportList);
    return 0;
}

DEF_MODULE_LOAD_FUNC_EXPORT(custom, module_init, exportList)

extern "C" JQUICK_EXPORT void custom_init_jsapis()
{
    registerCModuleLoader("custom", &custom_module_load);
}
