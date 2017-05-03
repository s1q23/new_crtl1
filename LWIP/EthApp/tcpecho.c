/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#include "lwip/opt.h"

#if LWIP_NETCONN

#include "netconf.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "adc.h"
#define TCPECHO_THREAD_PRIO  ( 6u )



/*-----------------------------------------------------------------------------------*/

static void tcpecho_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  struct netbuf *buf;
	err_t recv_err;
 // void *data;
	char data[100]={0};
//  u16_t len;
	u16 value;
	float a;
//////////////////////////////////////////////////
	/*
             value=Get_ADC_Value(ADC_Channel_5,20);	
					   a=(float)value*(3.3/4096);
             sprintf(data, "%g", a);//将float转化为char型	
	*/
///////////////////////////////////////////////////	

	
 // memcpy(data,&a,sizeof(a));

  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!=NULL)
  {  
    /* Bind connection to well known port number 7. */
    err = netconn_bind(conn, NULL, LOCAL_PORT);
    
    if (err == ERR_OK)
    {
      /* Tell connection to go into listening mode. */
      netconn_listen(conn);
    
      while (1) 
      {
        /* Grab new connection. */
         accept_err = netconn_accept(conn, &newconn);
    
        /* Process the new connection. */
        if (accept_err == ERR_OK) 
        {
          recv_err = netconn_recv(newconn, &buf);
					while ( recv_err == ERR_OK) 
          {
            do 
            {
						  					
             // netbuf_data(buf, &data, &len);
              netconn_write(newconn, data, sizeof(data), NETCONN_COPY);
          
            } 
            while (netbuf_next(buf) >= 0);
          
            netbuf_delete(buf);
						recv_err = netconn_recv(newconn, &buf);
          }
        
          /* Close connection and discard connection identifier. */
          netconn_close(newconn);
          netconn_delete(newconn);
        }
      }
    }
    else
    {
      netconn_delete(newconn);
      printf(" can not bind TCP netconn");
    }
  }
  else
  {
    printf("can not create TCP netconn");
  }
}

/*-----------------------------------------------------------------------------------*/

void tcpecho_init(void)
{
  sys_thread_new("tcpecho_thread", tcpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCPECHO_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
