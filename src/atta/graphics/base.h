// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

namespace atta::graphics {

struct GfxId {
    uint64_t data;

    GfxId(uint64_t val) : data(val) {}

    uint32_t* getPtr32() { return reinterpret_cast<uint32_t*>(&data); }
    uint64_t* getPtr64() { return &data; }

    operator uint32_t() { return uint32_t(data); }
    operator uint64_t() { return uint64_t(data); }
    operator bool() { return bool(data); }

    // uint64_t* operator&() { return &data; }
    uint32_t* operator&() { return reinterpret_cast<uint32_t*>(&data); }
};

} // namespace atta::graphics
