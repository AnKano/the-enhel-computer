/************************************************************************/
/*			KEGS: Apple //gs Emulator			*/
/*			Copyright 2002 by Kent Dickey			*/
/*									*/
/*		This code is covered by the GNU GPL			*/
/*									*/
/*	The KEGS web page is kegs.sourceforge.net			*/
/*	You may contact the author at: kadickey@alumni.princeton.edu	*/
/************************************************************************/

const char rcsid_scc_windriver_c[] = "@(#)$KmKId: scc_windriver.c,v 1.4 2004-11-19 02:00:46-05 kentd Exp $";

/* This file contains the Win32 COM1/COM2 calls */

#include "defc.h"
#include "scc.h"

extern Scc scc_stat[2];
extern word32 g_c025_val;

#ifdef _WIN32

int scc_serial_win_init(int port) {
    return 0;
}

void
scc_serial_win_change_params(int port) {}

void scc_serial_win_fill_readbuf(int port, int space_left, double dcycs) {}

void scc_serial_win_empty_writebuf(int port) {}

#endif
