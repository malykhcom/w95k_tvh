/* $Id$ */

#define INCL_PM
#include <os2.h>
#include <malloc.h>

static CHAR TVHP[]="T&V HappyPlayer";

static BOOL Equals(PCHAR a, PCHAR b, ULONG size)
{
    ULONG t;
    for (t = 0; t < size; t++)
        if (a[t] != b[t])
            return FALSE;
    return TRUE;
}

static HWND VerifyWindowOld(PSWCNTRL control)
{

    if (Equals(control->szSwtitle, TVHP, sizeof(TVHP)-1))
    {
        HWND hwnd = control->hwnd;
        if (hwnd)
        {
            hwnd = WinWindowFromID(hwnd, FID_CLIENT);
            if (hwnd)
            {
                return hwnd;
            }
        }
    }
    return NULL;
}

static HWND VerifyWindowNew(PSWCNTRL control)
{
    HWND hwnd = control->hwnd;
    if (hwnd)
    {
        hwnd = WinWindowFromID(hwnd, FID_CLIENT);
        if (hwnd)
        {
            CHAR name[256];
            if (WinQueryClassName(hwnd, sizeof(name), name) > 0)
            {
                if (Equals(name, TVHP, sizeof(TVHP)))
                {
                    return hwnd;
                }
            }
        }
    }
    return NULL;
}

static HWND FindWindow(VOID)
{
    HWND ret = NULL;
    ULONG num;
    ULONG t;
    PSWBLOCK blocks;
    ULONG size;
    num = WinQuerySwitchList(0, NULL, 0);
    if (!num)
        return ret;
    size = sizeof(ULONG)+sizeof(SWENTRY)*num;
    blocks = (PSWBLOCK)malloc(size);
    if (blocks)
    {
        num = WinQuerySwitchList(0, blocks, size);
        for (t = 0; t < num; t++)
        {
            HWND hwnd = VerifyWindowNew(&blocks->aswentry[t].swctl);
            if (hwnd)
            {
                ret = hwnd;
                break;
            }
        }
        free(blocks);
    }
    return ret;
}

static VOID SendMute(HWND hwnd)
{
    WinPostMsg(hwnd, WM_CHAR,
               /*0x32011005*/
               MPFROMSH2CH(KC_CHAR|KC_SCANCODE, 1, 0x32),
               MPFROM2SHORT('m', 0));
}

static VOID Beep(VOID)
{
    DosBeep(1000, 10);
}


static VOID Mute(VOID)
{
    HWND hwnd;
    hwnd = FindWindow();
    if (hwnd)
    {
        SendMute(hwnd);
    }
    else
    {
        Beep();
    }
}

VOID EXPENTRY TVHMute(int argc, char * argv[])
{
    Mute();
}

ULONG APIENTRY LibMain(ULONG hmod, ULONG termination)
{
    return TRUE;
}
