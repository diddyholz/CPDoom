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

    if (!deltaKey1 && !deltaKey2)
    {
        return false;
    }

    // Check all CAS_Keys
    if (I_CheckKey(KEY_0, deltaKey2, '0', ev))
    {
        return true;
    }
    
    if (I_CheckKey(KEY_1, deltaKey2, '1', ev))
    {
        return true;
    }
    
    if (I_CheckKey(KEY_2, deltaKey2, '2', ev))
    {
        return true;
    }
    
    if (I_CheckKey(KEY_3, deltaKey1, '3', ev))
    {
        return true;
    }
    
    if (I_CheckKey(KEY_4, deltaKey2, '4', ev))
    {
        return true;
    }
    
    if (I_CheckKey(KEY_5, deltaKey2, '5', ev))
    {
        return true;
    }
    
    if (I_CheckKey(KEY_6, deltaKey1, '6', ev))
    {
        return true;
    }
    
    if (I_CheckKey(KEY_7, deltaKey2, '7', ev))
    {
        return true;
    }
    
    if (I_CheckKey(KEY_8, deltaKey2, '8', ev))
    {
        return true;
    }
    
    if (I_CheckKey(KEY_9, deltaKey1, '9', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_KEYBOARD, deltaKey2, KEY_ESCAPE, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_SHIFT, deltaKey1, KEY_RSHIFT, ev))
    {
        return true;
    }

    // Backspace
    if (I_CheckKey((Keys1)0x00000080, deltaKey1, KEY_BACKSPACE, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_CLEAR, deltaKey1, KEY_ENTER, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_X, deltaKey2, 'n', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_Y, deltaKey2, 'y', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_Z, deltaKey1, 'z', ev))
    {
        return true;
    }

    // Equals
    if (I_CheckKey((Keys2)0x00000080, deltaKey2, '=', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_LEFT_BRACKET, deltaKey2, '(', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_RIGHT_BRACKET, deltaKey2, ')', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_COMMA, deltaKey2, ',', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_NEGATIVE, deltaKey2, '_', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_DOT, deltaKey2, '.', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_EXP, deltaKey1, KEY_RCTRL, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_POWER, deltaKey1, '^', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_DIVIDE, deltaKey1, '/', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_MULTIPLY, deltaKey1, '*', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_ADD, deltaKey1, '+', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_SUBTRACT, deltaKey1, '-', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_EXE, deltaKey1, ' ', ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_UP, deltaKey2, KEY_UPARROW, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_DOWN, deltaKey2, KEY_DOWNARROW, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_LEFT, deltaKey1, KEY_LEFTARROW, ev))
    {
        return true;
    }

    if (I_CheckKey(KEY_RIGHT, deltaKey1, KEY_RIGHTARROW, ev))
    {
        return true;
    }

    return false;
}

