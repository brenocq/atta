//--------------------------------------------------
// Atta Parallel Module
// manager.inl
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::parallel {

template <typename Script>
void Manager::runImpl(cmp::Entity entity, float dt, uint32_t num) {
    switch (_device->getType()) {
        case Device::Type::SERIAL:
            _serialDevice->run<Script>(entity, dt, num);
            break;
        case Device::Type::CPU:
            _cpuDevice->run<Script>(entity, dt, num);
            break;
        case Device::Type::GPU:
            _gpuDevice->run<Script>(entity, dt, num);
            break;
        default:
            LOG_WARN("pll::Manager", "This device is not supported yet");
    }
}

} // namespace atta::parallel
