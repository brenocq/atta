//--------------------------------------------------
// Atta Graphics Module
// base.h
// Date: 2022-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_BASE_H
#define ATTA_GRAPHICS_BASE_H

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

#endif // ATTA_GRAPHICS_BASE_H
