/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
 */
/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* FreeRTOS+FAT includes. */
#include <FreeRTOS.h>

#include "FreeRTOS_CLI.h"
#include "ff_sddisk.h"
#include "ff_stdio.h"
#include "ff_utils.h"
#include "hw_config.h"
#include "stdio_cli.h"

extern int low_level_io_tests();
extern void vMultiTaskStdioWithCWDTest(const char* pcMountPath, uint16_t usStackSizeWords);
extern void vMultiTaskStdioWithCWDTest2(const char* pcMountPath0, const char* pcMountPath1,
                                        uint16_t usStackSizeWords);

/*
 * Functions used to create and then test files on a disk.
 */
extern void vStdioWithCWDTest(const char* pcMountPath);
extern void vMultiTaskStdioWithCWDTest(const char* const pcMountPath, uint16_t usStackSizeWords);
static void ls() {
    char pcWriteBuffer[128] = {0};

    FF_FindData_t xFindStruct;
    memset(&xFindStruct, 0x00, sizeof(FF_FindData_t));

    ff_getcwd(pcWriteBuffer, sizeof(pcWriteBuffer));
    printf("Directory Listing: %s\n", pcWriteBuffer);

    int iReturned = ff_findfirst("", &xFindStruct);
    if (FF_ERR_NONE != iReturned) {
        printf("ff_findfirst error: %s (%d)\n", strerror(stdioGET_ERRNO()), -stdioGET_ERRNO());
        return;
    }
    do {
        const char *pcWritableFile = "writable file", *pcReadOnlyFile = "read only file",
                   *pcDirectory = "directory";
        const char* pcAttrib;

        /* Point pcAttrib to a string that describes the file. */
        if ((xFindStruct.ucAttributes & FF_FAT_ATTR_DIR) != 0) {
            pcAttrib = pcDirectory;
        } else if (xFindStruct.ucAttributes & FF_FAT_ATTR_READONLY) {
            pcAttrib = pcReadOnlyFile;
        } else {
            pcAttrib = pcWritableFile;
        }
        /* Create a string that includes the file name, the file size and the
         attributes string. */
        printf("%s [%s] [size=%d]\n", xFindStruct.pcFileName, pcAttrib,
               (int)xFindStruct.ulFileSize);
    } while (FF_ERR_NONE == ff_findnext(&xFindStruct));
}

// Maximum number of elements in buffer
#define BUFFER_MAX_LEN 10

/*-----------------------------------------------------------*/
static BaseType_t runFormat(char* pcWriteBuffer, size_t xWriteBufferLen,
                            const char* pcCommandString) {
    (void)pcWriteBuffer;
    (void)xWriteBufferLen;
    const char* pcParameter;
    BaseType_t xParameterStringLength;

    /* Obtain the parameter string. */
    pcParameter =
        FreeRTOS_CLIGetParameter(pcCommandString,        /* The command string itself. */
                                 1,                      /* Return the first parameter. */
                                 &xParameterStringLength /* Store the parameter string length. */
        );
    /* Sanity check something was returned. */
    configASSERT(pcParameter);
    FF_Disk_t* pxDisk = NULL;
    bool rc = format(&pxDisk, pcParameter);
    if (!rc)
        printf("Format failed!\n");
    return pdFALSE;
}

static const CLI_Command_Definition_t xFormat = {
    "format", /* The command string to type. */
    "\nformat <device name>:\n Format <device name>\n"
    "\te.g.: \"format sd0\"\n",
    runFormat, /* The function to run. */
    1          /* One parameter is expected. */
};

static BaseType_t runMount(char* pcWriteBuffer, size_t xWriteBufferLen,
                           const char* pcCommandString) {
    (void)pcWriteBuffer;
    (void)xWriteBufferLen;
    const char* pcParameter;
    BaseType_t xParameterStringLength;

    /* Obtain the parameter string. */
    pcParameter =
        FreeRTOS_CLIGetParameter(pcCommandString,        /* The command string itself. */
                                 1,                      /* Return the first parameter. */
                                 &xParameterStringLength /* Store the parameter string length. */
        );
    /* Sanity check something was returned. */
    configASSERT(pcParameter);

    char buf[cmdMAX_INPUT_SIZE];
    snprintf(buf, cmdMAX_INPUT_SIZE, "/%s", pcParameter); // Add '/' for path
    FF_Disk_t* pxDisk = NULL;
    bool rc = mount(&pxDisk, pcParameter, buf);
    if (!rc)
        printf("Mount failed!\n");

    return pdFALSE;
}

static const CLI_Command_Definition_t xMount = {
    "mount", /* The command string to type. */
    "\nmount <device name>:\n Mount <device name> at /<device name>\n"
    "\te.g.: \"mount sd0\"\n",
    runMount, /* The function to run. */
    1         /* One parameter is expected. */
};

static BaseType_t runEject(char* pcWriteBuffer, size_t xWriteBufferLen,
                           const char* pcCommandString) {
    (void)pcWriteBuffer;
    (void)xWriteBufferLen;
    const char* pcParameter;
    BaseType_t xParameterStringLength;

    /* Obtain the parameter string. */
    pcParameter =
        FreeRTOS_CLIGetParameter(pcCommandString,        /* The command string itself. */
                                 1,                      /* Return the first parameter. */
                                 &xParameterStringLength /* Store the parameter string length. */
        );
    /* Sanity check something was returned. */
    configASSERT(pcParameter);

    char buf[cmdMAX_INPUT_SIZE];
    snprintf(buf, cmdMAX_INPUT_SIZE, "/%s", pcParameter); // Add '/' for path

    eject(pcParameter, buf);

    return pdFALSE;
}

static const CLI_Command_Definition_t xEject = {
    "eject", /* The command string to type. */
    "\neject <device name>:\n Eject <device name>\n"
    "\te.g.: \"eject sd0\"\n",
    runEject, /* The function to run. */
    1         /* One parameter is expected. */
};

static const CLI_Command_Definition_t xUnmount = {
    "unmount",                                  /* The command string to type. */
    "unmount: Alias for \"eject\"\n", runEject, /* The function to run. */
    1                                           /* One parameter is expected. */
};

void register_fs_tests() {
    /* Register all the command line commands defined immediately above. */
    extern const CLI_Command_Definition_t xMTLowLevIOTests;

    FreeRTOS_CLIRegisterCommand(&xFormat);
    FreeRTOS_CLIRegisterCommand(&xMount);
    FreeRTOS_CLIRegisterCommand(&xEject);
    FreeRTOS_CLIRegisterCommand(&xUnmount);
}
