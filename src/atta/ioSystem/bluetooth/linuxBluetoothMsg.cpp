//--------------------------------------------------
// Atta IO System
// linuxBluetooth.cpp
// Date: 2022-03-18
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::io
{
    #define RETURN_FOUND 1000// Used to indicate that found what was looking for

    int LinuxBluetooth::bluezParseObjects(sd_bus_message* m, const char* adapter, BluezMsgType msgType, void* user)
    {
        //LOG_VERBOSE("io::LinuxBluetooth", "Parse objects $0", adapter);
        // Enter array of objects
        int r = sd_bus_message_enter_container(m, 'a', "{oa{sa{sv}}}");
        if(r < 0)
        {           
            LOG_ERROR(_debugName.getString(), "Failed to parse objects (enter container)");
            return r;          
        }

        // Enter next object
        while((r = sd_bus_message_enter_container(m, 'e', "oa{sa{sv}}")) > 0)
        {
            r = bluezParseObject(m, adapter, msgType, user);
            if(r < 0 || r == RETURN_FOUND)
                return r;

            // Exit dict
            r = sd_bus_message_exit_container(m);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse objects (exit object)");
                return r;
            }
        }

        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse objects (next object)");
            return r;
        }

        // Exit array
        r = sd_bus_message_exit_container(m);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse objects (exit array)");
            return r;
        }
        return r;
    }

    int LinuxBluetooth::bluezParseObject(sd_bus_message* m, const char* adapter, BluezMsgType msgType, void* user)
    {
        const char* opath;

        // Object path
        int r = sd_bus_message_read_basic(m, 'o', &opath);
        if(r < 0) 
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse object (read basic)");
            return r;
        }

        // Check if it is below our own adapter path
        if(strncmp(opath, adapter, strlen(adapter)) == 0)
        {
            // Parse array of interfaces
            r = bluezParseInterfaces(m, opath, msgType, user);
        }
        else
        {
            // Ignore
            r = sd_bus_message_skip(m, "a{sa{sv}}");
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse object (skip interface)");
                return r;
            }
        }           

        return r;
    }

    int LinuxBluetooth::bluezParseInterfaces(sd_bus_message* m, const char* opath, BluezMsgType msgType, void* user)
    {
        //LOG_VERBOSE("io::LinuxBluetooth", "Parse interfaces $0", opath);
        // Enter array of interface
        int r = sd_bus_message_enter_container(m, 'a', "{sa{sv}}");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse interface (enter container)");
            return r;
        }

        // Enter next dict entry
        while((r = sd_bus_message_enter_container(m, 'e', "sa{sv}")) > 0)
        {
            r = bluezParseInterface(m, opath, msgType, user);
            if(r < 0 || r == RETURN_FOUND)
            {
                return r;
            }

            // Exit dict
            r = sd_bus_message_exit_container(m);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse interface (exit container)");
                return r;
            }
        }

        // Check error while traversing entries
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse interface (enter container loop)");
            return r;
        }

        // Exit array
        r = sd_bus_message_exit_container(m);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse interface (exit container array)");
            return r;
        }

        return r;
    }

    int LinuxBluetooth::bluezParseInterface(sd_bus_message* m, const char* opath, BluezMsgType msgType, void* user)
    {
        const char* intf;      

        // Get interface name
        int r = sd_bus_message_read_basic(m, 's', &intf);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse interface name");
            return r;
        }

        //LOG_VERBOSE(_debugName.getString(), "[*w]Interface $0 (op:$1)", intf, opath);

        // If know this interface, parse and return
        switch(msgType)
        {
            case MSG_DEVICE_SCAN:
                {
                    if(strcmp(intf, "org.bluez.Device1") == 0)
                    {
                        // Get device information
                        Device dev {};
                        LinuxDevice devL {};
                        r = bluezParseDevice1(m, opath, &dev, &devL);

                        bool found = false;
                        for(Device& d : _devices)
                        {
                            if(d.mac == dev.mac)
                            {
                                d = dev;
                                found = true;
                                break;
                            }
                        }
                        if(!found)
                        {
                            _devices.push_back(dev);
                            _linuxDevices.push_back(devL);
                            if(dev.connected)
                                populateDeviceServices(&_linuxDevices.back());
                        }

                        return r;
                    }
                    break;
                }
            case MSG_DEVICE:
                {
                    if(strcmp(intf, "org.bluez.Device1") == 0)
                    {
                        // Update device information
                        LinuxDevice* lDev = static_cast<LinuxDevice*>(user);
                        Device* dev = nullptr;
                        for(unsigned i = 0; i < _devices.size(); i++)
                            if(_devices[i].mac == lDev->mac)
                                dev = &_devices[i];
                        if(dev == nullptr)
                        {
                            LOG_WARN(_debugName.getString(), "Unable to find device when parsing Device1 interface");
                            return r;
                        }

                        r = bluezParseDevice1(m, opath, dev, lDev);
                        return r;
                    }
                    break;
                }
            case MSG_SERVICE:
                {
                    if(strcmp(intf, "org.bluez.GattService1") == 0)
                    {
                        LinuxService* lServ = static_cast<LinuxService*>(user);
                        if(lServ == nullptr)
                        {
                            LOG_WARN(_debugName.getString(), "Unable to find service when parsing Service1 interface");
                            return r;
                        }

                        r = bluezParseService1(m, opath, lServ);
                        return r;
                    }
                    break;
                }
            case MSG_CHARS:
                {
                    if(strcmp(intf, "org.bluez.GattCharacteristic1") == 0)
                    {
                        LinuxService* lServ = static_cast<LinuxService*>(user);
                        Service* serv = nullptr;
                        for(Device& device : _devices)
                            for(Service& s : device.services)
                                if(s.uuid == lServ->uuid)
                                    serv = &s;
                        if(lServ == nullptr || serv == nullptr)
                        {
                            LOG_WARN(_debugName.getString(), "Unable to find service when parsing Characteristic1 interface");
                            return r;
                        }

                        LinuxChar lch {};
                        Char ch {};
                        r = bluezParseCharacteristic1(m, opath, &ch, &lch);
                        lServ->chars.push_back(lch);
                        serv->chars.push_back(ch);
                        return r;
                    }
                    break;
                }
        }

        // Unknown interface or action, skip
        r = sd_bus_message_skip(m, "a{sv}");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to skip unknown interface");
            return r;
        }
        return r;
    }

    int LinuxBluetooth::bluezParseDevice1(sd_bus_message* m, const char* opath, Device* dev, LinuxDevice* lDev)
    {
        const char* str;

        // Enter array of dict entries
        int r = sd_bus_message_enter_container(m, 'a', "{sv}");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse device1 (enter container)");
            return r;
        }

        // Enter next dict entry
        while((r = sd_bus_message_enter_container(m, 'e', "sv")) > 0)
        {
            // Property name
            r = sd_bus_message_read_basic(m, 's', &str);
            if(r < 0) 
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse device1 (read basic)");
                return r;
            }

            if(strcmp(str, "Name") == 0)
            {
                r = bluezReadVariant(m, "s", &str);
                if(r < 0) return r;
                dev->name = str;
            }
            else if(strcmp(str, "Address") == 0)
            {
                r = bluezReadVariant(m, "s", &str);
                if(r < 0) return r;
                
                // Set MAC
                dev->mac = Bluetooth::stringToMAC(std::string(str));
                lDev->mac = dev->mac;

                // Create device path based on MAC address
                lDev->path = _adapter + "/dev_XX_XX_XX_XX_XX_XX";
                r = snprintf(lDev->path.data(), lDev->path.size()+1,
                        "%s/dev_%02X_%02X_%02X_%02X_%02X_%02X", _adapter.c_str(), dev->mac[5],
                        dev->mac[4], dev->mac[3], dev->mac[2], dev->mac[1], dev->mac[0]);
                if(r < 0)
                {
                    LOG_ERROR(_debugName.getString(), "Failed to contruct device path for [w]$0[]", MACToString(dev->mac));
                    return r;
                }
            }
            else if(strcmp(str, "UUIDs") == 0)
            {
                char** serviceUUIDs = nullptr;
                r = bluezReadVariant(m, &serviceUUIDs);
                if(r < 0) return r;
                // Will be populated with LinuxBluetooth::PopulateDeviceServices
            }
            else if(strcmp(str, "ServicesResolved") == 0)
            {
                int b;
                r = bluezReadVariant(m, "b", &b);
                if(r < 0) return r;
                lDev->servicesResolved = b;
            }
            else if(strcmp(str, "Connected") == 0)
            {
                int b;
                r = bluezReadVariant(m, "b", &b);
                if(r < 0) return r;
                dev->connected = b;
            }
            else if(strcmp(str, "RSSI") == 0) 
            {
                r = bluezReadVariant(m, "n", &dev->rssi);
                if(r < 0) return r;
            }
            else
            {
                r = sd_bus_message_skip(m, "v");
                if(r < 0) return r;
            }

            // Exit dict
            r = sd_bus_message_exit_container(m);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse device1 (exit container)");
                return r;
            }
        }

        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse device1 (container entry)");
            return r;
        }

        // Exit array
        r = sd_bus_message_exit_container(m);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse device1 (exit container array)");
            return r;
        }

        return r;
    }

    int LinuxBluetooth::bluezParseService1(sd_bus_message* m, const char* opath, LinuxService* lServ)
    {
        const char* str;
        const char* uuid;

        // Enter array of dict entries
        int r = sd_bus_message_enter_container(m, 'a', "{sv}");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse service1 (enter container)");
            return r;          
        }

        // Enter next dict
        while((r = sd_bus_message_enter_container(m, 'e', "sv")) > 0)
        {
            // Property name
            r = sd_bus_message_read_basic(m, 's', &str);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse service1 (read basic)");
                return r;
            }

            if(strcmp(str, "UUID") == 0)
            { 
                r = bluezReadVariant(m, "s", &uuid);
                if(r < 0)
                {
                    LOG_ERROR(_debugName.getString(), "Failed to parse service1 (read UUID)");
                    return r;
                }
            } 
            else
            {
                r = sd_bus_message_skip(m, "v");
                if(r < 0)
                {
                    LOG_ERROR(_debugName.getString(), "Failed to parse service1 (skip basic)");
                    return r;
                }
            }

            // Exit dict
            r = sd_bus_message_exit_container(m);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse service1 (exit container)");
                return r;
            }
        }

        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse service1 (next container)");
            return r;
        }
        // Exit array
        r = sd_bus_message_exit_container(m);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse service1 (exit container)");
            return r;
        }

        // if UUID matched, update service path
        if(strcasecmp(uuid, lServ->uuid.c_str()) == 0)
        {
            // Save object path and UUID
            lServ->path = std::string(opath);
            return RETURN_FOUND;
        }

        return r;
    }

    int LinuxBluetooth::bluezParseCharacteristic1(sd_bus_message* m, const char* opath, Char* ch, LinuxChar* lch)
    {
        const char* str;
        const char* uuid;
        char** flags;
   
        // Enter array of dict entries
        int r = sd_bus_message_enter_container(m, 'a', "{sv}");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse characteristic1 (enter container)");
            return r;
        }
   
        // Enter next dict
        while((r = sd_bus_message_enter_container(m, 'e', "sv")) > 0)
        {
            // Property name
            r = sd_bus_message_read_basic(m, 's', &str);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse characteristic1 (read basic)");
                return r;
            }
   
            if(strcmp(str, "UUID") == 0)
            {
                r = bluezReadVariant(m, "s", &uuid);
                if(r < 0) return r;
            }
            else if(strcmp(str, "Flags") == 0)
            {
                r = bluezReadVariant(m, &flags);
                if(r < 0) return r;
            }
            else
            {
                r = sd_bus_message_skip(m, "v");
                if(r < 0) return r;
            }
   
            // Exit dict
            r = sd_bus_message_exit_container(m);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse characteristic1 (exit dict)");
                return r;
            }
        }

        // Exit array
        r = sd_bus_message_exit_container(m);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse characteristic1 (exit container)");
            return r;
        }
    
        ch->uuid = std::string(uuid);
        ch->flags = CharFlags::NONE;
        lch->uuid = std::string(uuid);
        lch->path = std::string(opath);

        // Convert flags
        for(int i = 0; flags != nullptr && flags[i] != nullptr; i++)
        {
            if(strcmp(flags[i], "broadcast") == 0)
                ch->flags |= CharFlags::BROADCAST;
            else if(strcmp(flags[i], "read") == 0)
                ch->flags |= CharFlags::READ;
            else if(strcmp(flags[i], "write-without-response") == 0)
                ch->flags |= CharFlags::WRITE_NO_RESPONSE;
            else if(strcmp(flags[i], "write") == 0)
                ch->flags |= CharFlags::WRITE;
            else if(strcmp(flags[i], "notify") == 0)
                ch->flags |= CharFlags::NOTIFY;
            else if(strcmp(flags[i], "indicate") == 0)
                ch->flags |= CharFlags::INDICATE;
            free(flags[i]);
        }
        free(flags);
        return r;
    }

    int LinuxBluetooth::bluezParseNotify(sd_bus_message* m, Char* ch, LinuxChar* lch, const void** ptr, size_t* len)
    {
        int r;
        char* str;             

        // Interface name
        r = sd_bus_message_read_basic(m, 's', &str);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse notify (read interface name)");
            return -2;         
        }

        // Ignore all other interfaces
        if(strcmp(str, "org.bluez.GattCharacteristic1") != 0)
        {
            LOG_VERBOSE(_debugName.getString(), "Parse notify ignored interface $0", str);
            return 0;
        }

        // Enter array of dict entries
        r = sd_bus_message_enter_container(m, 'a', "{sv}");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse notify (enter dict)");
            return -2;
        }

        // Enter first element
        r = sd_bus_message_enter_container(m, 'e', "sv");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse notify (enter first element)");
            return -2;
        }

        // Property name
        r = sd_bus_message_read_basic(m, 's', &str);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse notify (read element name)");
            return -2;
        }

        // Ignore all except Value
        if(strcmp(str, "Notifying") == 0)
        {
            int b;
            r = bluezReadVariant(m, "b", &b); 
            if(r < 0)
            {
                return -2;
            }
            ch->notifying = b;
        }
        else if(strcmp(str, "Value") == 0)
        {
            // Enter variant
            r = sd_bus_message_enter_container(m, 'v', "ay");
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse notify (enter value variant)");
                return -2;
            }

            // Get byte array
            r = sd_bus_message_read_array(m, 'y', ptr, len);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to parse notify (read byte array)");
                return -2;
            }
        }
        else
        {
            LOG_VERBOSE(_debugName.getString(), "Parse notify ignored property $0", str);
            return 0;
        }

        // No need to exit containers as we stop parsing
        return r;
    }

    int LinuxBluetooth::bluezReadVariant(sd_bus_message* m, char* type, void* dest)
    {
        int r = sd_bus_message_enter_container(m, 'v', type);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse variant (enter container)");
            return r;
        }

        r = sd_bus_message_read_basic(m, type[0], dest);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse variant (read basic)");
            return r;
        }

        r = sd_bus_message_exit_container(m);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse variant (exit container)");
            return r;
        }

        return r;
    }

    int LinuxBluetooth::bluezReadVariant(sd_bus_message* m, char*** dest)
    {
        int r = sd_bus_message_enter_container(m, 'v', "as");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse variant (enter container)");
            return r;
        }

        r = sd_bus_message_read_strv(m, dest);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse variant (read basic)");
            return r;
        }

        r = sd_bus_message_exit_container(m);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse variant (exit container)");
            return r;
        }

        return r;
    }

    int LinuxBluetooth::bluezAppendProperty(sd_bus_message* m, const char* name, char type, const void* value)
    {
        // Open dict
        int r = sd_bus_message_open_container(m, 'e', "sv");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to append property (open container)");
            return r;
        }

        r = sd_bus_message_append_basic(m, 's', name);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to append property (append variant)");
            return r;
        }

        // Open variant
        r = sd_bus_message_open_container(m, 'v', "s");
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to append property (open variant)");
            return r;
        }

        r = sd_bus_message_append_basic(m, type, value);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to append property (append variant basic)");
            return r;
        }

        // Close variant
        r = sd_bus_message_close_container(m);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to append property (close variant)");
            return r;
        }

        // Close dict
        r = sd_bus_message_close_container(m);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to append property (close container)");
            return r;
        }

        return r;
    }
}
