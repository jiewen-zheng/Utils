//
// Created by moon on 2023/9/19.
//

#include "stm32ymodem.h"
#include <cstring>

Stm32YModem::Stm32YModem() {
    writeAddress = 0;
    readAddress  = 0;
}

Stm32YModem::~Stm32YModem() {

}


Stm32YModem::FileInformation Stm32YModem::getFileInformation() {
    return fileInfo;
}


uint32_t Stm32YModem::write(uint8_t *data, uint32_t len) {
    /** uart write */
//    HAL_UART_Transmit(&huart1, data, len, 1000);
    return len;
}

uint32_t Stm32YModem::read(uint8_t *data, uint32_t len) {
    /** uart read or fifo space read */
//    return fifo_read(&uart1Data.fifo, data, len);
    return len;
}

/**
 * \brief YModem file operation callback function.
 * \param type file operation type.
 * \param data write data or read data pointer.
 * \param len write data length.
 * \return success = 0,fail = -1, or return data length.
 */
int Stm32YModem::fileOperate(YModem::OperateType type, uint8_t *data, uint32_t len) {
    switch (type) {
    case YModem::FileOpenWriteOnly:
    /** erase flash space
     * Replace with your function
     * */
        // erase_flash_page(USER_APP_START_ADDR,
        //                  USER_APP_START_ADDR + USER_APP_MAX_SIZE);
        return OperateSuccess;

    case YModem::FileClose:
        readAddress  = 0;
        writeAddress = 0;
        return OperateSuccess;

    case YModem::FileSetName:
        memset(fileInfo.name, 0, sizeof(fileInfo.name));
        strcpy(fileInfo.name, (const char *) data);
        return OperateSuccess;

    case YModem::FileSetSize:
        fileInfo.size = len;
        return OperateSuccess;

    case YModem::FileWrite:
    /** write flash space
     * Replace with your function
     * */
        // program_flash(USER_APP_START_ADDR + writeAddress,
        //                   (uint32_t *) data, len);
        writeAddress += len;
        return len;

    case YModem::FileRead:
    /** read flash space
     * Replace with your function
     * */
        // read_flash(USER_APP_START_ADDR + readAddress,
        //                (uint32_t *) data, len);
        readAddress += len;
        return len;

    default:
        break;
    }

    return OperateFail;
}