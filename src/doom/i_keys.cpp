#include <stdint.h>
#include <stdio.h>
#include <sdk/calc/calc.hpp>
#include <sdk/os/input.hpp>

#include "doomdef.h"
#include "d_event.h"

uint32_t previousKey1 = 0;
uint32_t previousKey2 = 0;

evtype_t I_GetKeyEvType(uint32_t keyToCheck, uint32_t previousKey)
{
    if (keyToCheck & previousKey) 
    {
        return ev_keyup;
    }
    else
    {
        return ev_keydown;
    }
}

boolean I_CheckKey(Keys1 keyToCheck, 
    uint32_t deltaKey, 
    uint8_t doomKey, 
    event_t *ev)
{
    if (!(deltaKey & keyToCheck))
    {
        return false;
    }

    // Setup event data
    ev->data1 = doomKey;
    ev->type = I_GetKeyEvType(keyToCheck, previousKey1);

    // Toggle previouskey
    previousKey1 ^= keyToCheck;

    return true;
}

boolean I_CheckKey(Keys2 keyToCheck, 
    uint32_t deltaKey, 
    uint8_t doomKey, 
    event_t *ev)
{
    if (!(deltaKey & keyToCheck))
    {
        return false;
    }

    // Setup event data
    ev->data1 = doomKey;
    ev->type = I_GetKeyEvType(keyToCheck, previousKey2);

    // Toggle previouskey
    previousKey2 ^= keyToCheck;

    return true;
}

extern "C"
boolean I_GetKeyEvent(event_t *ev)
{
    // TODO: Configurable keys
    uint32_t key1;
    uint32_t key2;

    getKey(&key1, &key2);

    if (!Input_IsAnyKeyDown())
    {
        key1 = 0;
        key2 = 0;
    }

    uint32_t deltaKey1 = key1 ^ previousKey1;
    uint32_t deltaKey2 = key2 ^ previousKey2;

    // Check all gamekeys
    if (I_CheckKey(KEY_8, deltaKey2, KEY_UPARROW, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_5, deltaKey2, KEY_DOWNARROW, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_4, deltaKey2, KEY_LEFTARROW, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_6, deltaKey1, KEY_RIGHTARROW, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_CLEAR, deltaKey1, KEY_ENTER, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_EXE, deltaKey1, KEY_RCTRL, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_ADD, deltaKey1, KEY_RSHIFT, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_SUBTRACT, deltaKey1, ' ', ev))
    {
        return true;
    }

    // Backspace
    if (I_CheckKey((Keys1)0x00000080, deltaKey1, KEY_ESCAPE, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_Y, deltaKey2, 'y', ev))
    {
        return true;
    }

    return false;
}

