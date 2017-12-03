/*
 * bsp_console.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "bsp_console.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hal.h"

#define CONSOLE_LIST_MAX        16
#define CONSOLE_TOKEN_AMOUNT    8
#define CONSOLE_TOKEN_STRLEN    16
#define CONSOLE_TOKEN_CHAR      " \t\r\n\a"

//Put list in FRAM to save RAM.
#pragma PERSISTENT(list_str)
#pragma PERSISTENT(list_prompt)
#pragma PERSISTENT(list_func)

static uint8_t *list_str[CONSOLE_LIST_MAX] =
{ 0 };
static uint8_t *list_prompt[CONSOLE_LIST_MAX] =
{ 0 };
static uint16_t (*list_func[CONSOLE_LIST_MAX])(uint8_t **) =
{   0
};
static uint16_t list_size = 0;

static uint8_t console_str[64] =
{ 0 };


/***Internal Functions***/

uint8_t **Console_split(uint8_t *line)
{
    static uint8_t *tokens[CONSOLE_TOKEN_AMOUNT];
    uint8_t sbuf[CONSOLE_TOKEN_STRLEN] =
    { 0 };
    uint8_t *p;
    uint16_t i = 0;

    strcpy(sbuf, line);
    tokens[i++] = strtok(sbuf, CONSOLE_TOKEN_CHAR);
    while ((p = strtok(NULL, CONSOLE_TOKEN_CHAR)))
    {
        tokens[i++] = p;
    }

    return tokens;
}

uint16_t Console_execute(uint8_t **args)
{
    uint16_t i;

    if (args[0] == NULL)
    {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < list_size; i++)
    {
        // Check for valid command
        if (strcmp(args[0], list_str[i]) == 0)
        {
            return (*list_func[i])(args);
        }
    }

    // Run default command
    return 0;
}

uint16_t Console_process(uint8_t *str)
{
    uint8_t **args;
    uint16_t status;

    args = Console_split(str);
    status = Console_execute(args);

    if (status == 0)
    {
        Console_help(args);
    }
    return status;
}

uint16_t Console_help(uint8_t **args)
{
    uint16_t i;
    printf("\r\nCommand Prompt:\n");
    for (i = 0; i < list_size; i++)
    {
        printf("%s: %s\n", list_str[i], list_prompt[i]);
    }
    return 1;
}

void Console_printTimeStamp()
{
    uint16_t sec;
    uint16_t ms;

    sec = Hal_Rtc_getTimeStamp() / 1000;
    ms = Hal_Rtc_getTimeStamp() % 1000;

    printf("\r\n[%d.%d]", sec, ms);
}

/***External Functions***/

uint16_t Bsp_Console_init()
{
    Bsp_Console_insert("help", &Console_help, "\tPrint help hints");
    Bsp_Console_insert("time", &Console_printTimeStamp, "\tPrint time stamp");
    Bsp_Console_insert("reset", &Mcu_reset, "\tReset MCU");
    Bsp_Console_insert("bsl", &Mcu_enterBsl, "\tEnter TI BSL");

    printf("\r\nConsole Initial Done [Ver:%s][Time:%d ms].\r\n>", BSP_CONSOLE_VERSION,Hal_Rtc_getTimeStamp());
    Uart_gets(console_str, 16);

    return 1;
}

uint16_t Bsp_Console_run(void)
{
    uint16_t len;
    len = strlen(console_str);

    if ('\r' == console_str[len - 1])
    {
        Console_process(console_str);
        memset(console_str, 0, sizeof(console_str));
        Uart_gets(console_str, 64);
        printf("\r\n>");
    }
    return 1;
}

uint16_t Bsp_Console_insert(uint8_t *str, uint16_t func(uint8_t **), uint8_t *prompt)
{
    list_str[list_size] = str;
    list_func[list_size] = func;
    list_prompt[list_size] = prompt;
    list_size++;
    return 1;
}
