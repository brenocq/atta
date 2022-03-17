//--------------------------------------------------
// Atta UI System
// ioSystemWindowBluetooth.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::ui
{
    void IOSystemWindow::bluetoothTabItem()
    {
        if(!_bluetooth)
        {
            io::Bluetooth::CreateInfo info;
            info.debugName = StringId("io::Bluetooth");
            _bluetooth = io::IOManager::create<io::Bluetooth>(info);
            _bluetooth->start();
            _bluetooth->startScan();
        }
        _bluetooth->update();
    }
}
