#include <stdbool.h>
#include <stdio.h>
#include <conio.h>

//#include "event_flags.h"
#include "terminal.h"


#define ESC "\x1b"
#define CSI "\x1b["

#define IN_RECORDS_ARRAY_SIZE 16ULL

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

Error terminal_init(struct Terminal *term, struct Eventflags*ev_flags)
{
    if (!EnableVTMode(&term->output_handle, &term->input_handle))
        return ERROR_VT_INIT_FAILED;

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

    if (!GetConsoleScreenBufferInfo(term->output_handle, &screen_buffer_info))
       return ERROR_GET_SCREEN_BUFFER_FAILED;

    ev_flags->window_size.x = screen_buffer_info.srWindow.Right - screen_buffer_info.srWindow.Left + 1;
    ev_flags->window_size.y = screen_buffer_info.srWindow.Bottom - screen_buffer_info.srWindow.Top + 1;
    ev_flags->window_resize = true;

    // Enter the alternate buffer
    printf(CSI "?1049h");

    return ERROR_NONE;
}

void terminal_close(struct Terminal* term)
{
    // Exit the alternate buffer
    printf(CSI "?1049l");
}

Error terminal_update(struct Terminal* term, struct Eventflags* ev_flags)
{
    if (!(term && ev_flags))
        return ERROR_NULL_POINTER_EXCEPTION;

    INPUT_RECORD input_record[IN_RECORDS_ARRAY_SIZE];
    DWORD events_read;

    if (!PeekConsoleInput(term->input_handle, input_record, 1, &events_read))
        return ERROR_CONSOLE_INPUT_READ_FAILED;

    while (events_read)
    {
        if (!ReadConsoleInput(term->input_handle, input_record, IN_RECORDS_ARRAY_SIZE, &events_read))
            return ERROR_CONSOLE_INPUT_READ_FAILED;       

        for (unsigned long ev_num = 0; ev_num < events_read; ev_num++)
        {
            switch (input_record[ev_num].EventType)
            {
            case KEY_EVENT:
            {
                struct Keys* key;
                if (input_record[ev_num].Event.KeyEvent.bKeyDown)
                    key = &ev_flags->pressed;
                else
                    key = &ev_flags->released;

                switch (input_record[ev_num].Event.KeyEvent.wVirtualKeyCode)
                {
                case VK_ESCAPE:
                    key->esc = true;
                    break;
                case VK_F1:
                    key->f1 = true;
                    break;
                }
                break;
            } 
            case WINDOW_BUFFER_SIZE_EVENT:
                ev_flags->window_resize = true;
                ev_flags->window_size.x = input_record[ev_num].Event.WindowBufferSizeEvent.dwSize.X;
                ev_flags->window_size.y = input_record[ev_num].Event.WindowBufferSizeEvent.dwSize.Y;
                break;
            }
        }
       
        if (events_read < IN_RECORDS_ARRAY_SIZE)
            events_read = 0;
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