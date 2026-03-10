// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

namespace atta::graphics {

class IndexBuffer {
  public:
    struct CreateInfo {
        uint8_t* data = nullptr;
        uint32_t size = 0;   ///< Size in bytes
        uint32_t offset = 0; ///< Offset in bytes
    };

    IndexBuffer(const CreateInfo& info);
    virtual ~IndexBuffer() = default;

    virtual void bind() const = 0;

    uint32_t getSize() const { return _size; };
    uint32_t getCount() const { return _size / sizeof(uint32_t); };

  protected:
    uint32_t _offset;
    uint32_t _size;
    const uint8_t* _data;
};

} // namespace atta::graphics
