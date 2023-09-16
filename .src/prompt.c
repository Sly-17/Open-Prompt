#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


void error_and_exit(const char * error) {
  fprintf(stderr, "ERROR: %s\n", error);
  exit(1);
}


size_t write_callback(void * contents, size_t size, size_t nmemb, void * userp) {

  size_t total_size = size * nmemb;
  char* data = (char*)(contents);

  fwrite(data, 1, total_size, stdout);

  return total_size;
}


int main(int argc, char * argv[]) {

  CURL * curl = curl_easy_init();

  if(!curl) error_and_exit("Failed to initialize libcurl");

  const char * url = "https://www.llama2.ai/api";
  const char * json_data = "\"systemPrompt\" : \"You are a helpful assistant.\", \"temperature\" : 0.75, \"topP\" : 0.9, \"maxTokens\" : 500, \"version\" : \"2796ee9483c3fd7aa2e171d38f4ca12251a30609463dcfd4cd76703f22e96cdf\"";

  char post_data[1024 * 7] = {0};
  sprintf(post_data, "{%s, \"prompt\" : \"%s\"}", json_data, argv[1]);

  //printf("%s\n", post_data);                                                                                                                      

  struct curl_slist * headers = NULL;

  headers = curl_slist_append(headers, "Content-Type: application/json");

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

  CURLcode result = curl_easy_perform(curl);

  if (result != CURLE_OK) {
    error_and_exit(curl_easy_strerror(result));
  }

  printf("\n");

  curl_easy_cleanup(curl);

  return 0;
}
