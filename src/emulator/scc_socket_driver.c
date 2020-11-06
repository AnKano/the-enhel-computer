/************************************************************************/
/*			KEGS: Apple //gs Emulator			*/
/*			Copyright 2002-2004 by Kent Dickey		*/
/*									*/
/*		This code is covered by the GNU GPL			*/
/*									*/
/*	The KEGS web page is kegs.sourceforge.net			*/
/*	You may contact the author at: kadickey@alumni.princeton.edu	*/
/************************************************************************/

const char rcsid_scc_socket_driver_c[] = "@(#)$KmKId: scc_socket_driver.c,v 1.11 2004-12-06 19:42:09-05 kentd Exp $";

/* This file contains the socket calls */

#include "defc.h"
#include "scc.h"
#include <signal.h>

extern Scc scc_stat[2];
extern int g_serial_modem[];

extern int h_errno;

int g_wsastartup_called = 0;

void scc_socket_init(int port) {

}

void scc_socket_maybe_open_incoming(int port, double dcycs) {

}

void scc_socket_open_outgoing(int port, double dcycs) {

}

void scc_socket_make_nonblock(int port, double dcycs) {

}

void scc_socket_change_params(int port) {
}

void scc_socket_close(int port, int full_close, double dcycs) {

}

void scc_accept_socket(int port, double dcycs) {

}

void scc_socket_telnet_reqs(int port, double dcycs) {

}

void scc_socket_fill_readbuf(int port, int space_left, double dcycs) {

}

int g_scc_dbg_print_cnt = 50;

void scc_socket_recvd_char(int port, int c, double dcycs) {

}

void scc_socket_empty_writebuf(int port, double dcycs) {

}

void scc_socket_modem_write(int port, int c, double dcycs) {

}

void scc_socket_do_cmd_str(int port, double dcycs) {

}

void scc_socket_send_modem_code(int port, int code, double dcycs) {

}

void scc_socket_modem_hangup(int port, double dcycs) {

}

void scc_socket_modem_connect(int port, double dcycs) {

}

void scc_socket_modem_do_ring(int port, double dcycs) {

}

void scc_socket_do_answer(int port, double dcycs) {

}
