/*
 * io.c
 *
 *  Created on: 22/07/2013
 *      Author: bernardobreder
 */

#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char* file_read(char* path) {
	FILE* fp = fopen(path, "r");
	if (!fp) {
		return 0;
	}
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	unsigned char* buffer = (unsigned char*) malloc((size + 1) * sizeof(unsigned char));
	if (!buffer) {
		fclose(fp);
		return 0;
	}
	buffer[size] = 0;
	if (fread(buffer, sizeof(unsigned char), size, fp) != size) {
		fclose(fp);
		free(buffer);
		return 0;
	}
	fclose(fp);
	return buffer;
}

error_t file_write(char* path, unsigned char* buffer, long size) {
	FILE* fp = fopen(path, "w+");
	if (!fp) {
		return 1;
	}
	if (size < 0) {
		size = strlen((char*) buffer);
	}
	if (fwrite(buffer, sizeof(unsigned char), size, fp) != size) {
		fclose(fp);
		return 1;
	}
	fclose(fp);
	return 0;
}

long file_size(char* path) {
	FILE* fp = fopen(path, "r");
	if (!fp) {
		return -1;
	}
	fseek(fp, 0L, SEEK_END);
	long result = ftell(fp);
	fclose(fp);
	return result;
}
