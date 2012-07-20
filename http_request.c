#include "http_request.h"

/**
 * Class for generating http request. 
 *
 * @author Juha Syrjälä <juha.syrjala@gmail.com>
 */
http_field* http_create_field(char* name, char* value)
{
  http_field* field = malloc(sizeof(http_field));
  if(field == NULL) {
    // fail
    return NULL;
  }
  field->name = name;
  field->value = value;
  return field;
}

void http_request_free(http_request* request) 
{
  if(!request) {
    return;
  }
  free(request);
}

http_request* http_request_create(http_method method, char* url, int timeout) 
{
  http_request* request = malloc(sizeof(http_request));
  if(request == NULL) {
    // fail
    return NULL;
  }
  request->method = method;
  request->cid = "1";
  request->url = url;
  request->user_agent = "RuuviTracker Firmware/0.1";
  request->content_type = "application/x-www-form-urlencoded";
  request->redirect = 1;
  request->timeout = timeout;
  request->fields = NULL;
  return request;
}

static int wait_for_response(const char* ok_response) {
  return 0;
}

static int send_at_command(const char* command) {
  // TODO implement
  printf("--Sending command:\n");
  printf("%s\n", command);
  return wait_for_response("OK");
}

static int send_data(const char* data, int length) {
  // TODO implement
  printf("--Sending data %d bytes:\n", length);
  printf("%s\n", data);
  printf("--Sending data ended.\n");
  return wait_for_response("OK");
}

static int send_parameter(char* buffer, const char* command, const void* name, const void* value) {
  sprintf(buffer, command, name, value);
  return send_at_command(buffer);
}

static int send_int_parameter(char* buffer, const char* command, const void* name, int value) {
  sprintf(buffer, command, name, value);
  return send_at_command(buffer);
}

static void url_encode(char* buffer, char* data) {
  char* source = data;
  char* dest = buffer;

  while(*source) {
    char input = *(source++);
    switch(input) {
    case '&':
      *(dest++) = '%';
      *(dest++) = '2';
      *(dest++) = '6';
      break;
    case ',':
      *(dest++) = '%';
      *(dest++) = '2';
      *(dest++) = 'C';
      break;
    case ' ':
      *(dest++) = '+';
      break;
    case '+':
      *(dest++) = '%';
      *(dest++) = '2';
      *(dest++) = 'B';
      break;
    default:
      *(dest++) = input;
    }
  }
  *(dest++) = 0;
}
static void generate_data(char* buffer, http_request* request) {
  list_node* fields = request->fields;

  char* dest = buffer;
  char url_encode_buffer[200];
  while(fields) {
    http_field* field = fields->data;
    url_encode(url_encode_buffer, field->name);
    strcpy(dest, url_encode_buffer);
    dest += strlen(url_encode_buffer);

    *(dest++) = '=';

    url_encode(url_encode_buffer, field->value);
    strcpy(dest, url_encode_buffer);
    dest += strlen(url_encode_buffer);

    *(dest++) = '&';
    fields = fields->next;
  }
  // replace excess '&' with zero
  *(dest-1) = '\0';
}

int http_request_send(http_request* request) {
  // TODO are HTTPINIT and HTTPTERM required at each request?
  if(send_at_command("AT+HTTPINIT") != 0) {
    return 1;
  }
  char cid_buf[50];
  if(send_parameter(cid_buf, "AT+HTTPPARA=\"%s\",\"%s\"", "CID", request->cid) != 0) {
    return 1;
  }
  char url_buf[512];
  if(send_parameter(url_buf, "AT+HTTPPARA=\"%s\",\"%s\"", "URL", request->url) != 0) {
    return 1;
  }
  char user_agent_buf[512];
  if(send_parameter(user_agent_buf, "AT+HTTPPARA=\"%s\",\"%s\"", "UA", request->user_agent) != 0) {
    return 1;
  }
  char content_type_buf[100];
  if(send_parameter(content_type_buf, "AT+HTTPPARA=\"%s\",\"%s\"", "CONTENT", request->content_type) != 0 ) {
    return 1;
  }
  char redirect_buf[50];
  if(send_int_parameter(redirect_buf, "AT+HTTPPARA=\"%s\",\"%d\"", "REDIR", request->redirect) != 0) {
    return 1;
  }
  char timeout_buf[50];
  if(send_int_parameter(timeout_buf, "AT+HTTPPARA=\"%s\",\"%d\"", "TIMEOUT", request->timeout) != 0) {
    return 1;
  }
  char data_buf[512];
  generate_data(data_buf, request);
  int bytes = strlen(data_buf);
  int send_timeout = 5000;
  char data_command_buf[30];
  sprintf(data_command_buf, "AT+HTTPDATA=%d,%d", bytes, send_timeout);
  if(send_at_command(data_command_buf) != 0) {
    return 1;
  }
  if(send_data(data_buf, bytes) != 0) {
    return 1;
  }

  char action_buf[30];
  sprintf(action_buf, "AT+HTTPACTION=%d", request->method);
  if(send_at_command(action_buf) != 0) {
    return 1;
  }
  if(send_at_command("AT+HTTPTERM") != 0) {
    return 1;
  }
  return 0;
}
