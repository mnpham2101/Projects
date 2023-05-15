#include "string"
#include "map"
using namespace std;

// This field identify HttpRequest message.
// only GET messages are serviced.
typedef enum class HttpMethod{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
} HttpMethod;

// HTTP response status codes as listed in:
// https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
// Note that not all of them are present in this enum class
// This field is needed to construct HttpResponse message
enum class HttpStatusCode {
  Continue = 100,
  SwitchingProtocols = 101,
  EarlyHints = 103,
  Ok = 200,
  Created = 201,
  Accepted = 202,
  NonAuthoritativeInformation = 203,
  NoContent = 204,
  ResetContent = 205,
  PartialContent = 206,
  MultipleChoices = 300,
  MovedPermanently = 301,
  Found = 302,
  NotModified = 304,
  BadRequest = 400,
  Unauthorized = 401,
  Forbidden = 403,
  NotFound = 404,
  MethodNotAllowed = 405,
  RequestTimeout = 408,
  ImATeapot = 418,
  InternalServerError = 500,
  NotImplemented = 501,
  BadGateway = 502,
  ServiceUnvailable = 503,
  GatewayTimeout = 504,
  HttpVersionNotSupported = 505
};

// the HttpVersion is not cared in Resquest msg but must be included in Response message
// only HTTP_1_1 is supported
enum class HttpVersion {
  HTTP_0_9 = 9,
  HTTP_1_0 = 10,
  HTTP_1_1 = 11,
  HTTP_2_0 = 20
};

// Defines the functions that converts HttpMessage fields to string
class Utility{
    public:
    Utility();
    ~Utility();
    
    string to_string(const HttpMethod method);
    string to_string(const HttpVersion version);
    string to_string(const HttpStatusCode status_code);
};

/* HttpMessage defines common fields of Http messages and get/set functions.
 * Sample http request: 
GET / HTTP/1.1
Host: localhost:8081
User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/113.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive   
 * method: GET  is used to identify the request
 * path:   /    identify the resource that HttpServer needs to get
 * headers: User-Agent, Accept, Accept-Language etc...
 * only the method and path are parsed, stored and used in logic for this project. 
 */
class HttpMessage{
    public:
    HttpMessage();
    virtual ~HttpMessage();
    
    // set, only accept const parameters
    void setMethod(const string & method );
    void setPath(const string path );
    void setHeader(const string& key, const string& value) {
        headers_[key] = move(value);
    }
    void setContent(const string& content) {
        content_ = move(content);
    }
    void setVersion(const string& version_string);
    
    // get, only const functions 
    HttpMethod getMethod() const { return method_; }
    string getPath() const { return path_; }
    string getContent() const { return content_; }
    HttpVersion getVersion() const {return version_; }
    map<string, string> getHeaders() const { return headers_; }

    private:
    HttpMethod method_;
    HttpVersion version_;
    string path_;
    map<string, string> headers_;
    string content_; 
};

// An HttpRequest object represents a single HTTP request
// HttpServer parse the requests and reads Http method, path, and other contents
// only method and paths are used by HttpServer in this project to make response to client.
class HttpRequest : public HttpMessage {
    public:
    HttpRequest();
    virtual ~HttpRequest();
    // string to_string(const HttpRequest& request);
    // parse the request message
    virtual HttpRequest* parseRequest(const string& request_string);
};

// An HttpResponse object represents a single HTTP response
// The HTTP server sends an HTTP response to a client that include
// an HTTP status code, headers, and (optional) content
class HttpResponse : public HttpMessage {
    public:
    HttpResponse() : status_code_(HttpStatusCode::Ok) {}
    HttpResponse(HttpStatusCode status_code) : status_code_(status_code) {}
    virtual ~HttpResponse();
    string constructResponseString (const HttpResponse& response, bool send_content);
    private:
    void setStatusCode(const HttpStatusCode status_code) { status_code_ = status_code; }
    HttpStatusCode getStatusCode() const { return status_code_; }
    
    private:
    HttpStatusCode status_code_;
};