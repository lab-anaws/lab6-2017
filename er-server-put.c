/**
  * \file
  *         A very simple Contiki application using CoAP 
  * \author
  *         Carlo Vallati <c.vallati@iet.unipi.it>
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"

#include "rest-engine.h"

#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]",(lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3],(lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

static int value = 10;


void
getputvalue_handler_get(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{

  char message[20];
  int length = 20; 

  sprintf(message, "Current Value: %03u", value);

  memcpy(buffer, message, length);

  REST.set_header_content_type(response, REST.type.TEXT_PLAIN); /* text/plain is the default, hence this option could be omitted. */
  REST.set_header_etag(response, (uint8_t *) &length, 1);
  REST.set_response_payload(response, buffer, length);
  
}

void
getputvalue_handler_put(void* request, void* response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{

  int new_value, len;
  const char *val = NULL;
     
  len=REST.get_post_variable(request, "value", &val);
     
  if( len > 0 ){
     new_value = atoi(val);
     PRINTF("new value %u\n", new_value);
     value = new_value;
     REST.set_response_status(response, REST.status.CREATED);
  } else {
     REST.set_response_status(response, REST.status.BAD_REQUEST);
  }
}


RESOURCE(getputvalue, "title=\"Get/Put value=10\";rt=\"Text\"", getputvalue_handler_get, getputvalue_handler_put, getputvalue_handler_put, NULL);

PROCESS(rest_server_example, "Erbium Example Server");
AUTOSTART_PROCESSES(&rest_server_example);

PROCESS_THREAD(rest_server_example, ev, data)
{
  PROCESS_BEGIN();

  PRINTF("Starting Erbium Example Server\n");

  /* Initialize the REST engine. */
  rest_init_engine();

  /* Activate the application-specific resources. */
  rest_activate_resource(&getputvalue, "value");
  /* Define application-specific events here. */
  while(1) {
    PROCESS_WAIT_EVENT();
  } /* while (1) */

  PROCESS_END();
}
