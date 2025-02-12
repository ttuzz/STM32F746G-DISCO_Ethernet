/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : LWIP.c
 * Description        : This file provides initialization code for LWIP
 *                      middleWare.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "lwip_rtos.h"

#include "lwip/init.h"
#if (defined ( __CC_ARM ) || defined (__ARMCC_VERSION))  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */
#include "ethernetif.h"
#include <string.h>

/* USER CODE BEGIN 0 */
#include "lwip/opt.h"
#include "main.h"
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif
#if LWIP_NETCONN
#include "lwip/sys.h"
#include "lwip/api.h"

#endif
#include "FreeRTOS.h"
/* USER CODE END 0 */
/* Private function prototypes -----------------------------------------------*/

/* ETH Variables initialization ----------------------------------------------*/

/* USER CODE BEGIN 1 */
/* Private variables ---------------------------------------------------------*/

/* Definitions for EthLink */

/* USER CODE END 1 */

/* Variables Initialization */
extern struct netif gnetif;
extern ip_addr_t ipaddr;
extern ip_addr_t netmask;
extern ip_addr_t gw;

/* USER CODE BEGIN OS_THREAD_ATTR_CMSIS_RTOS_V2 */
#define INTERFACE_THREAD_STACK_SIZE ( 1024 )

/* USER CODE END OS_THREAD_ATTR_CMSIS_RTOS_V2 */

/* USER CODE BEGIN 2 */
#if LWIP_NETCONN
/* Private define ------------------------------------------------------------*/
#define TCPECHO_THREAD_PRIO  (osPriorityAboveNormal)
#define UDPECHO_THREAD_PRIO  (osPriorityAboveNormal)

/* Private functions ---------------------------------------------------------*/
#define lenlen 4096*4
volatile char buffer[lenlen] = { '\0' };

static void tcpecho_thread(void *arg) {
	struct netconn *conn, *newconn;
	err_t err;
	LWIP_UNUSED_ARG(arg);

	/* Create a new connection identifier. */
	/* Bind connection to well known port number 7. */
	conn = netconn_new(NETCONN_TCP);
	netconn_bind(conn, IP_ADDR_ANY, 7);

	LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

	/* Tell connection to go into listening mode. */
	netconn_listen(conn);

	while (1) {

		/* Grab new connection. */
		err = netconn_accept(conn, &newconn);
		/*printf("accepted new connection %p\n", newconn);*/
		/* Process the new connection. */
		if (err == ERR_OK) {
			struct netbuf *buf;
			void *data;
			u16_t len;

			while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
				netbuf_data(buf, &data, &len);

				if ((len >= 5) && (strncmp(data, "ttuzz", 5) == 0)) {
					while (err == ERR_OK) {
						err = netconn_write_partly(newconn, buffer, lenlen,
								NETCONN_NOCOPY, NULL); // Veri göndermek için netconn_write_partly
					}
				} else {
					/*printf("Recved\n");*/
					do {

						// strncpy(buffer, buf->p->payload, buf->p->len);
						// printf("%s \n", buffer);
						netbuf_data(buf, &data, &len);
						err = netconn_write(newconn, data, len, NETCONN_COPY);
					} while (netbuf_next(buf) >= 0);
				}

				netbuf_delete(buf);
			}
			/*printf("Got EOF, looping\n");*/
			/* Close connection and discard connection identifier. */
			netconn_close(newconn);
			netconn_delete(newconn);
		}
	}
}

/*-----------------------------------------------------------------------------------*/
void tcpecho_init(void) {
	sys_thread_new("tcpecho_thread", tcpecho_thread, NULL,
			(configMINIMAL_STACK_SIZE * 3), TCPECHO_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/
static void udpecho_thread(void *arg) {
	struct netconn *conn;
	struct netbuf *buf, *tx_buf;
	err_t err;
	LWIP_UNUSED_ARG(arg);
	char data[100] = { '\0' };

	conn = netconn_new(NETCONN_UDP);
	netconn_bind(conn, IP_ADDR_ANY, 7);

	LWIP_ERROR("udpecho: invalid conn", (conn != NULL), return;);

	while (1) {
		err = netconn_recv(conn, &buf);
		if (err == ERR_OK) {

			/* print received data */
			strncpy(data, buf->p->payload, buf->p->len);
			printf("%s \n", data);

			tx_buf = netbuf_new();
			netbuf_alloc(tx_buf, buf->p->tot_len);

			pbuf_take(tx_buf->p, (const void*) buf->p->payload,
					buf->p->tot_len);

			err = netconn_sendto(conn, tx_buf, (const ip_addr_t*) &(buf->addr),
					buf->port);
			if (err != ERR_OK) {
				LWIP_DEBUGF(LWIP_DBG_ON, ("netconn_send failed: %d\n", (int)err));
			} else {
				LWIP_DEBUGF(LWIP_DBG_ON, ("got %s\n", buffer));
			}
			netbuf_delete(tx_buf);
		}
		netbuf_delete(buf);
	}
}
/*-----------------------------------------------------------------------------------*/
void udpecho_init(void) {
	sys_thread_new("udpecho_thread", udpecho_thread, NULL,
			(configMINIMAL_STACK_SIZE * 2), UDPECHO_THREAD_PRIO);
}

#endif /* MDK ARM Compiler */

