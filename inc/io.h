/*
 * io.h
 *
 *  Created on: 22/07/2013
 *      Author: bernardobreder
 */

#ifndef IO_H_
#define IO_H_

#include "struct.h"

unsigned char* file_read(char* path);

error_t file_write(char* path, unsigned char* buffer, long size);

long file_size(char* path);

#endif /* IO_H_ */
