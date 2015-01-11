/*
 *  Released under "The GNU General Public License (GPL-2.0)"
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
#ifndef __BrcmPatchRAM__
#define __BrcmPatchRAM__

#include <IOKit/IOService.h>
#include <IOKit/IOLib.h>
#include <IOKit/usb/IOUSBDevice.h>

#include "BrcmFirmwareStore.h"

#define kDisplayName "DisplayName"

enum DeviceState
{
    kUnknown,
    kInitialize,
    kFirmwareVersion,
    kMiniDriverComplete,
    kInstructionWrite,
    kInstructionWritten,
    kFirmwareWritten,
    kResetComplete,
    kUpdateComplete
};

class BrcmPatchRAM : public IOService
{
    private:
        typedef IOService super;
        OSDeclareDefaultStructors(BrcmPatchRAM);
    
        UInt16 mVendorId;
        UInt16 mProductId;
    
        IOUSBDevice* mDevice = NULL;
        IOUSBInterface* mInterface = NULL;
        IOUSBPipe* mInterruptPipe = NULL;
        IOUSBPipe* mBulkPipe = NULL;
    
        IOUSBCompletion mInterruptCompletion;
        IOBufferMemoryDescriptor* mReadBuffer;
    
        volatile DeviceState mDeviceState = kInitialize;
        volatile uint16_t mFirmareVersion = 0xFFFF;
    
        unsigned int getDelayValue(const char* key);

        static const char* getState(DeviceState deviceState);

        BrcmFirmwareStore* getFirmwareStore();
    
        void printDeviceInfo();
        int getDeviceStatus();
    
        bool resetDevice();
        bool setConfiguration(int configurationIndex);
    
        IOUSBInterface* findInterface();
        IOUSBPipe* findPipe(uint8_t type, uint8_t direction);
    
        void continousRead();
        static void readCompletion(void* target, void* parameter, IOReturn status, UInt32 bufferSizeRemaining);
    
        IOReturn hciCommand(void * command, uint16_t length);
        IOReturn hciParseResponse(void* response, uint16_t length, void* output, uint8_t* outputLength);
    
        IOReturn bulkWrite(void* data, uint16_t length);
    
        uint16_t getFirmwareVersion();
    
        bool performUpgrade();
    public:
        virtual IOService* probe(IOService *provider, SInt32 *probeScore);
        virtual const char* stringFromReturn(IOReturn rtn);
};

#endif //__BrcmPatchRAM__