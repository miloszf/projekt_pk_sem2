#include <stdbool.h>
#include <stdio.h>
#include <conio.h>

#include "terminal.h"

#define ESC "\x1b"
#define CSI "\x1b["

#define IN_RECORDS_NUMBER 16

bool EnableVTMode(HANDLE *out_handle, HANDLE* in_handle)
{
    *out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (*out_handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD consoleMode = 0;
    if (!GetConsoleMode(*out_handle, &consoleMode))
    {
        return false;
    }

    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_WRAP_AT_EOL_OUTPUT;
    if (!SetConsoleMode(*out_handle, consoleMode))
    {
        return false;
    }

    *in_handle = GetStdHandle(STD_INPUT_HANDLE);
    if (*in_handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    consoleMode = 0;
    if (!GetConsoleMode(*in_handle, &consoleMode))
    {
        return false;
    }

    consoleMode |= ENABLE_WINDOW_INPUT;
    if (!SetConsoleMode(*in_handle, consoleMode))
    {
        return false;
    }
    return true;
}

Error terminal_init(struct Terminal *term)
{
    if (!EnableVTMode(&term->output_handle, &term->input_handle))
        return ERROR_VT_INIT_FAILED;

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

    if (!GetConsoleScreenBufferInfo(term->output_handle, &screen_buffer_info))
       return ERROR_GET_SCREEN_BUFFER_FAILED;  

    //term->window_size.X = term->screen_buffer_info.srWindow.Right - term->screen_buffer_info.srWindow.Left + 1;
    //term->window_size.Y = term->screen_buffer_info.srWindow.Bottom - term->screen_buffer_info.srWindow.Top + 1;

    term->events = malloc(sizeof(struct Event));
    if (!term->events)
        return ERROR_MEMORY_ALLOCATION_FAILED;

    term->events->flag = WINDOW_RESIZED;
    term->events->value.window_size.x = screen_buffer_info.srWindow.Right - screen_buffer_info.srWindow.Left + 1;
    term->events->value.window_size.y = screen_buffer_info.srWindow.Bottom - screen_buffer_info.srWindow.Top + 1;

    // Enter the alternate buffer
    printf(CSI "?1049h");

    return ERROR_NONE;
}

void terminal_close(struct Terminal* term)
{
    // Exit the alternate buffer
    printf(CSI "?1049l");
}

Error terminal_update(struct Terminal* term)
{
    /*if (!GetConsoleScreenBufferInfo(term->output_handle, &term->screen_buffer_info))
        return ERROR_GET_SCREEN_BUFFER_FAILED;

    term->window_size.X = term->screen_buffer_info.srWindow.Right - term->screen_buffer_info.srWindow.Left + 1;
    term->window_size.Y = term->screen_buffer_info.srWindow.Bottom - term->screen_buffer_info.srWindow.Top + 1;*/

    if (!term)
        return ERROR_NULL_POINTER_EXCEPTION;

    INPUT_RECORD input_record[IN_RECORDS_NUMBER];
    DWORD events_read;

    if (!PeekConsoleInput(term->input_handle, &input_record, 1, &events_read))
        return ERROR_CONSOLE_INPUT_READ_FAILED;

    while (events_read)
    {
        if (!ReadConsoleInput(term->input_handle, &input_record, IN_RECORDS_NUMBER, &events_read))
            return ERROR_CONSOLE_INPUT_READ_FAILED;

        struct Event *new_events = realloc(term->events, sizeof(struct Event) * (term->events_number + IN_RECORDS_NUMBER));

        if (!new_events)
            return ERROR_MEMORY_ALLOCATION_FAILED;
        term->events = new_events;
        term->events_number += events_read;

        while (events_read)
        {
            struct Event new_event;

            switch (input_record[events_read - 1].EventType)
            {
            case KEY_EVENT:
                if (!input_record[events_read - 1].Event.KeyEvent.bKeyDown)
                {
                    new_event.flag = KEY_PRESSED;
                    WORD key;
                    switch (input_record[events_read - 1].Event.KeyEvent.wVirtualKeyCode)
                    {
                    case VK_ESCAPE:
                        key = KEY_ESC;
                        break;
                    case VK_F1:
                        key = KEY_F1;
                        break;
                    default:
                        key = 0;
                        break;
                    }
                    new_event.value.key = key;
                }
                break;

            case WINDOW_BUFFER_SIZE_EVENT:
                new_event.flag = WINDOW_RESIZED;
                COORD new_size = input_record[events_read - 1].Event.WindowBufferSizeEvent.dwSize;
                new_event.value.window_size.x = input_record[events_read - 1].Event.WindowBufferSizeEvent.dwSize.X;
                new_event.value.window_size.y = input_record[events_read - 1].Event.WindowBufferSizeEvent.dwSize.Y;
                break;
            }

            term->events[term->events_number - events_read - 1] = new_event;
            events_read--;
        }

    }

    //while (events_read)
    //{
    //    if (!ReadConsoleInput(term->input_handle, &input_record, 1, &events_read))
    //        return ERROR_CONSOLE_INPUT_READ_FAILED;

    //    switch (input_record.EventType)
    //    {
    //    case KEY_EVENT:
    //        if (!input_record.Event.KeyEvent.bKeyDown)
    //        {
    //            term->event.flag = KEY_PRESSED;
    //            WORD key;
    //            switch (input_record.Event.KeyEvent.wVirtualKeyCode)
    //            {
    //            case VK_ESCAPE:
    //                key = KEY_ESC;
    //                break;
    //            case VK_F1:
    //                key = KEY_F1;
    //                break;
    //            default:
    //                key = 0;
    //                break;
    //            }
    //            term->event.value.key = key;
    //        }
    //        break;
    //    case WINDOW_BUFFER_SIZE_EVENT:
    //        term->event.flag = WINDOW_RESIZED;
    //        COORD new_size = input_record.Event.WindowBufferSizeEvent.dwSize;
    //        if (new_size.X != term->event.value.window_size.x || new_size.Y != term->event.value.window_size.y)
    //        {
    //            term->event.value.window_size.x = input_record.Event.WindowBufferSizeEvent.dwSize.X;
    //            term->event.value.window_size.y = input_record.Event.WindowBufferSizeEvent.dwSize.Y;
    //        }
    //        break;
    //    }
    //}

    return ERROR_NONE;
}