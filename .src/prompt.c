#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


void error_and_exit(const char * error) {
  fprintf(stderr, "ERROR: %s\n", error);
  exit(1);
}

struct ResponseData {
  char * data;
  size_t size;
};


size_t write_callback(void * contents, size_t size, size_t nmemb, void * userp) {

  size_t total_size = size * nmemb;
  struct ResponseData * response = (struct ResponseData * ) userp;
  
  // Reallocate the buffer to accommodate the new data                                                                                               
  response -> data = (char * ) realloc(response -> data, response -> size + total_size + 1);
  if (response -> data == NULL) {
    error_and_exit("Memory allocation error");
  }

  memcpy( & (response -> data[response -> size]), contents, total_size);
  response -> size += total_size;
  response -> data[response -> size] = '\0'; // Null-terminate the string                                                                                

  return total_size;

}

int main(int argc, char * argv[]) {

  CURL * curl = curl_easy_init();

  if(!curl) error_and_exit("Failed to initialize libcurl");

  const char * url = "https://www.llama2.ai/api";
  const char * json_data = "\"systemPrompt\" : \"You are a helpful assistant.\", \"temperature\" : 0.75, \"topP\" : 0.9, \"maxTokens\" : 500, \"versio
  n\ " : \"2796ee9483c3fd7aa2e171d38f4ca12251a30609463dcfd4cd76703f22e96cdf\"";

  char post_data[1024 * 7] = {0};
  sprintf(post_data, "{%s, \"prompt\" : \"%s\"}", json_data, argv[1]);

  //printf("%s\n", post_data);                                                                                                                      

  struct curl_slist * headers = NULL;

  headers = curl_slist_append(headers, "Content-Type: application/json");

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  struct ResponseData response_data;

  response_data.data = NULL;

  response_data.size = 0;

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, & response_data);

  CURLcode result = curl_easy_perform(curl);

  if (result != CURLE_OK) {
    error_and_exit(curl_easy_strerror(result));
  }

  printf("%s\n", response_data.data);

  free(response_data.data);
  curl_easy_cleanup(curl);

  return 0;
}
