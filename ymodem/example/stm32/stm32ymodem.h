//
// Created by moon on 2023/9/19.
//

#ifndef _STM32YMODEM_H
#define _STM32YMODEM_H

#include "../../ymodem.h"

#define FLASH_WRITE_START_ADDRESS 0x8010000

class Stm32YModem : public YModem {
public:
    struct FileInformation {
        char     name[32];
        uint32_t size;
    };

public:
    Stm32YModem();
    ~Stm32YModem();

    FileInformation getFileInformation();

protected:
    uint32_t write(uint8_t *data, uint32_t len) override;
    uint32_t read(uint8_t *data, uint32_t len) override;
    int fileOperate(OperateType type, uint8_t *data, uint32_t len) override;

private:
    uint32_t writeAddress;
    uint32_t readAddress;

    FileInformation fileInfo;
};


#endif //_STM32YMODEM_H
