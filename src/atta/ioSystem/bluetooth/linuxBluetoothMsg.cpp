//--------------------------------------------------
// Atta IO System
// linuxBluetooth.cpp
// Date: 2022-03-18
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::io
{
    #define RETURN_FOUND 1000// Used to indicate that found what was looking for
                             //
    int LinuxBluetooth::bluezParseInterfaces(sd_bus_message* m, const char* opath, BluezMsgType msgType)
    {
        LOG_VERBOSE("io::LinuxBluetooth", "Parse message $0", opath);
        // Enter array of interface
        int r = sd_bus_message_enter_container(m, 'a', "{sa{sv}}");
        if(r < 0) {
            LOG_ERROR(_debugName.getString(), "Failed to parse interface (enter container)");
            return r;
        }

        // Enter next dict entry
        while((r = sd_bus_message_enter_container(m, 'e', "sa{sv}")) > 0)
        {
            r = bluezParseInterface(m, opath, msgType);
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

    int LinuxBluetooth::bluezParseInterface(sd_bus_message* m, const char* opath, BluezMsgType msgType)
    {
        const char* intf;      

        // Get interface name
        int r = sd_bus_message_read_basic(m, 's', &intf);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to parse interface name");
            return r;
        }

        // If know this interface, parse and return
        switch(msgType)
        {
            case MSG_DEVICE_SCAN:
                {
                    if(strcmp(intf, "org.bluez.Device1") == 0)
                    {
                        // Get device information
                        Device dev {};
                        r = bluezParseDevice1(m, opath, &dev);
                        
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
                            _devices.push_back(dev);

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


        //if (act == MSG_SERV_FIND && strcmp(intf, "org.bluez.GattService1") == 0) {
        //    /* find service by UUID, returns RETURN_FOUND if found, user
        //     * points to a blz_serv where the UUID to look for is filled */
        //    r = msg_parse_service1(m, opath, user);
        //} else if (act == MSG_CHAR_FIND 
        //        && strcmp(intf, "org.bluez.GattCharacteristic1") == 0) {
        //    /* find char by UUID, returns RETURN_FOUND if found, user
        //     * points to a blz_char where the UUID to look for is filled */
        //    r = msg_parse_characteristic1(m, opath, user);
        //} else if (act == MSG_CHAR_COUNT
        //        && strcmp(intf, "org.bluez.GattCharacteristic1") == 0) {
        //    /* just count the times the GattCharacteristic1 interface was
        //     * found, user just is an int pointer */
        //    int* cnt = user;
        //    (*cnt)++;          
        //    r = sd_bus_message_skip(m, "a{sv}");
        //    if (r < 0) {
        //        LOG_ERR("BLZ error parse 1intf 2");
        //    }
        //} else if (act == MSG_CHARS_ALL 
        //        && strcmp(intf, "org.bluez.GattCharacteristic1") == 0) {
        //    /* get UUIDs from all characteristics. user points to the service
        //     * where enough space for them has already been allocated */
        //    blz_serv* srv = user;
        //    blz_char ch = {0}; // temporary char
        //    r = msg_parse_characteristic1(m, opath, &ch);
        //    if (r < 0) {       
        //        return r;      
        //    }
        //    /* copy UUID */
        //    srv->char_uuids[srv->chars_idx] = strdup(ch.uuid);
        //    srv->chars_idx++;  
        //    return 0; // override RETURN_FOUND this would stop the loop
        //} else if (act == MSG_DEVICE && strcmp(intf, "org.bluez.Device1") == 0) {
        //    /* parse device properties, user points to device */
        //    r = msg_parse_device1(m, opath, user);
        //} else if (act == MSG_DEVICE_SCAN 
        //        && strcmp(intf, "org.bluez.Device1") == 0) {
        //    /* used in scan callback. user points to a blz* where the scan_cb
        //     * can be found. create a temporary device, parse all info into
        //     * it and then call callback */
        //    blz_dev dev = {0};
        //    r = msg_parse_device1(m, opath, &dev);
        //    if (r < 0) {
        //        return r;
        //    }

        //    /* callback */
        //    blz_ctx* ctx = user;
        //    if (ctx != NULL && ctx->scan_cb != NULL) {
        //        ctx->scan_cb(dev.mac, BLZ_ADDR_UNKNOWN, dev.rssi, NULL, 0,
        //                ctx->scan_user);
        //    }

        //    /* free uuids of temporary device */
        //    for (int i = 0;
        //            dev.service_uuids != NULL && dev.service_uuids[i] != NULL; i++) {
        //        free(dev.service_uuids[i]);
        //    }
        //    free(dev.service_uuids);
        //} else {
        //    /* unknown interface or action */
        //    r = sd_bus_message_skip(m, "a{sv}");
        //    if (r < 0) {
        //        LOG_ERR("BLZ error parse 1intf 3");
        //    }
        //}
        return r;
    }

    int LinuxBluetooth::bluezParseDevice1(sd_bus_message* m, const char* opath, Device* dev)
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
                dev->mac = Bluetooth::stringToMAC(std::string(str));
            }
            else if(strcmp(str, "UUIDs") == 0)
            {
                char** service_uuids;
                r = bluezReadVariant(m, &service_uuids);
                if(r < 0) return r;
            }
            else if(strcmp(str, "ServicesResolved") == 0)
            {
                int b;
                r = bluezReadVariant(m, "b", &b);
                if(r < 0) return r;
                //dev->services_resolved = b;
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
}
