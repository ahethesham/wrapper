#include <vector>
#include <string>
#include "include/client.h"

int main()
{
    // will take care of connecting with the remote process 
    https::client client("generativelanguage.googleapis.com" , 443);
    const char *apiKey = getenv("API_KEY");
    http::headers reqHeaders;
    reqHeaders["Content-Type"] = "application/json";
    reqHeaders["x-goog-api-key"] = apiKey;

    http::jsonBody body;
    body["contents"] = *(new http::jsonBody);
    auto inner = body["contents"];


}
