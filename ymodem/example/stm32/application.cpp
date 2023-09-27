//
// Created by moon on 2023/9/19.
//
#include "iostream"
#include "stm32ymodem.h"

Stm32YModem ymodem;

void application() {
//    printf("bootloader.\r\n");

    while (true) {
        /* receive file */
        ymodem.receive();

        if ((ymodem.getStatus() != YModem::StatusEstablish) &&
            (ymodem.getStatus() != YModem::StatusTransmit)) {
            /** file transfer over.
             * do user code */
            break;
        }

    }

    if (ymodem.getStatus() == YModem::StatusFinish) {
        /** file transfer success */
        Stm32YModem::FileInformation info = ymodem.getFileInformation();
        std::cout << "file name:" << info.name << std::endl;
        std::cout << "file size:" << info.size << std::endl;
    }else{
        /** file transfer faile */
        std::cout << "file transfer failed";
    }

    /** system reset */
//    NVIC_SystemReset();
}