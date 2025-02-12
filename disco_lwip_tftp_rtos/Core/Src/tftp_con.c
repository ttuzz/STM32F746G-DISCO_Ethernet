/*
 * tftp_con.c
 *
 *  Created on: Feb 11, 2025
 *      Author: ttuzz
 */
#include "stm32f7xx_hal.h"
#include "lwip/apps/tftp_server.h"

extern struct tftp_state tftp_state;

uint32_t start,end;
uint64_t size=0;

static void* tftp_open(const char* fname, const char* mode, u8_t is_write) {
  LWIP_UNUSED_ARG(mode);
  start=HAL_GetTick();
  size=0;
  printf("file name: %s, write flag: %ld\n", fname, is_write);

  /*if (is_write) {
    return (void*)fopen(fname, "wb");
  } else {
    return (void*)fopen(fname, "rb");
  }*/
}

static void tftp_close(void* handle) {
	end=HAL_GetTick()-start;
	uint32_t speed_bps = size / (end / 1000); // Convert time from ms to seconds
	  printf("File transfer completed.\n");
	  printf("Elapsed time: %lu ms\n", end);
	  printf("Transfer speed: %lu Bps, %lu kbps \n", speed_bps,speed_bps/1024);
 // fclose((FILE*)handle);
}

static int tftp_read(void* handle, void* buf, int bytes) {
 /* int ret = fread(buf, 1, bytes, (FILE*)handle);
  if (ret <= 0) {
    return -1;
  }
  return ret;*/
	return 0;
}

static int tftp_write(void* handle, struct pbuf* p) {
	if(size==0){
		printf("buf %lu\n",p->len);
	}

	size+=p->len;


  /*while (p != NULL) {
    if (fwrite(p->payload, 1, p->len, (FILE*)handle) != (size_t)p->len) {
      return -1;
    }
    p = p->next;
  }*/
  return 0;
}


// ctx'nin doğru türde olması gerekiyor.
const struct tftp_context tftp_con = {
  tftp_open,
  tftp_close,
  tftp_read,
  tftp_write
};

void  tftp_init_con(){
	//tftp_state.port=69;
	//tftp_state.retries=3;
	  //tftp_state.ctx=&tftp_con;
	  tftp_init(&tftp_con);
}


