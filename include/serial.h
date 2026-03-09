/*
 * This file is part of the flashrom project.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

/**
 * This file exposes a platform independent way to enumerate and interact with serial devices connected to a machine.
*/

#ifndef __SERIAL_H__
#define __SERIAL_H__ 1

#if IS_WINDOWS
#include <windows.h>
#endif

#if IS_WINDOWS
typedef HANDLE fdtype;
#define SER_INV_FD	INVALID_HANDLE_VALUE
#else
typedef int fdtype;
#define SER_INV_FD	-1
#endif

/* Serial port/pin mapping:

  1	CD	<-
  2	RXD	<-
  3	TXD	->
  4	DTR	->
  5	GND     --
  6	DSR	<-
  7	RTS	->
  8	CTS	<-
  9	RI	<-
*/
enum SP_PIN {
	PIN_CD = 1,
	PIN_RXD,
	PIN_TXD,
	PIN_DTR,
	PIN_GND,
	PIN_DSR,
	PIN_RTS,
	PIN_CTS,
	PIN_RI,
};

/**
 * @brief Set serial port pin state
 * @param pin Pin to control (PIN_TXD, PIN_DTR, PIN_RTS)
 * @param val Pin state (1 for high, 0 for low)
 */
void sp_set_pin(enum SP_PIN pin, int val);

/**
 * @brief Get serial port pin state
 * @param pin Pin to read (PIN_CTS, PIN_DSR)
 * @return 1 if pin is high, 0 if pin is low
 */
int sp_get_pin(enum SP_PIN pin);


/* Global file descriptor for the currently open serial port */
extern fdtype sp_fd;


/**
 * @brief Flush all incoming data from serial port buffer
 */
void sp_flush_incoming(void);

/**
 * @brief Open and configure serial port
 * @param dev Device path (e.g., "/dev/ttyS0" or "COM1")
 * @param baud Baud rate (use -1 to keep current rate)
 * @return File descriptor on success, SER_INV_FD on failure
 */
fdtype sp_openserport(char *dev, int baud);

/**
 * @brief Configure serial port parameters
 * @param fd File descriptor of the serial port
 * @param baud Baud rate (use -1 to keep current rate)
 * @return 0 on success, 1 on failure
 */
int serialport_config(fdtype fd, int baud);

/**
 * @brief Close serial port connection
 * @param data Unused parameter (for compatibility)
 * @return 0 on success.
 */
int serialport_shutdown(void *data);

/**
 * @brief Write data to serial port (blocking mode)
 * @param buf Buffer containing data to write
 * @param writecnt Number of bytes to write
 * @return 0 on success, 1 on failure
 */
int serialport_write(const unsigned char *buf, unsigned int writecnt);

/**
 * @brief Write data to serial port (non-blocking mode)
 * @param buf Buffer containing data to write
 * @param writecnt Number of bytes to write
 * @param timeout Timeout in milliseconds
 * @param really_wrote Pointer to store actual bytes written (can be NULL)
 * @return 0 on success, positive on temporary errors, negative on permanent errors
 */
int serialport_write_nonblock(const unsigned char *buf, unsigned int writecnt, unsigned int timeout, unsigned int *really_wrote);

/**
 * @brief Read data from serial port (blocking mode)
 * @param buf Buffer to store read data
 * @param readcnt Number of bytes to read
 * @return 0 on success, 1 on failure
 */
int serialport_read(unsigned char *buf, unsigned int readcnt);

/**
 * @brief Read data from serial port (non-blocking mode)
 * @param c Buffer to store read data
 * @param readcnt Number of bytes to read
 * @param timeout Timeout in milliseconds
 * @param really_read Pointer to store actual bytes read (can be NULL)
 * @return 0 on success, positive on temporary errors, negative on permanent errors.
 */
int serialport_read_nonblock(unsigned char *c, unsigned int readcnt, unsigned int timeout, unsigned int *really_read);

#endif
