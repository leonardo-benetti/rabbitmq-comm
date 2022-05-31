#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <amqp.h>
#include <amqp_tcp_socket.h>

#include "utils.h"

int main(int argc, char const *const *argv) {
  char const *hostname = "rabbitmq";
  int port = 5672;
  int status;
  amqp_socket_t *socket = NULL;
  amqp_connection_state_t conn;

  conn = amqp_new_connection();


  // Opens connection
  socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    die("creating TCP socket");
  }

  status = amqp_socket_open(socket, hostname, port);
  if (status) {
    die("opening TCP socket");
  }

  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                               "guest", "guest"),
                    "Logging in");
  amqp_channel_open(conn, 1);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");


  // Reads picture
  FILE *fp;
  fp= fopen("/app/images/duck.jpeg", "rb");
  fseek(fp, 0, SEEK_END);
  int length = ftell(fp);
  rewind(fp);
  unsigned char *file_data= (char *)malloc((length+1)*sizeof(char));
  if (file_data == NULL)
  {
    printf("Could not allocate memory");
    exit(1);
  }
  size_t k = fread(file_data, length, 1, fp);

  // Setup body message
  amqp_bytes_t message_bytes;
  message_bytes.len = length;
  message_bytes.bytes = file_data;

  // Setup headers data
  amqp_basic_properties_t props;
  props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG |  AMQP_BASIC_HEADERS_FLAG;
  props.content_type = amqp_cstring_bytes("image/jpeg");
  props.delivery_mode = 2; /* persistent delivery mode */
  amqp_table_t *table=&props.headers;
  props.headers.num_entries = 2;
  props.headers.entries=calloc(props.headers.num_entries, sizeof(amqp_table_entry_t));
  table->entries[0].key = amqp_cstring_bytes("String1");
  table->entries[0].value.kind = AMQP_FIELD_KIND_BYTES;
  table->entries[0].value.value.bytes = amqp_cstring_bytes("abcd efgh");
  table->entries[1].key = amqp_cstring_bytes("String2");
  table->entries[1].value.kind = AMQP_FIELD_KIND_BYTES;
  table->entries[1].value.value.bytes = amqp_cstring_bytes("1234 5678");
  

  // Sends data  
  die_on_error(
      amqp_basic_publish(conn, 1, amqp_cstring_bytes(""), amqp_cstring_bytes("msg_queue"), 0, 0, &props, message_bytes),
      "Publishing"
    );


  // Closes connection
  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS),
                    "Closing channel");
  die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS),
                    "Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");
  
  // frees data
  free(file_data);
  
  return 0;
}
