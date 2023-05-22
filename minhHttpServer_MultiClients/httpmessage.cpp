#include "httpmessage.h"

#include "sstream"
#include "algorithm"
#include "iostream"

HttpRequest::HttpRequest(){};
HttpRequest::~HttpRequest(){};

HttpMessage::HttpMessage(){};
HttpMessage::~HttpMessage(){};
// HttpResponse::HttpResponse(){};
HttpResponse::~HttpResponse(){};
Utility::Utility(){};
Utility::~Utility(){};

string Utility::to_string(HttpMethod method){
  switch (method) {
    case HttpMethod::GET:
      return "GET";
    case HttpMethod::HEAD:
      return "HEAD";
    case HttpMethod::POST:
      return "POST";
    case HttpMethod::PUT:
      return "PUT";
    case HttpMethod::DELETE:
      return "DELETE";
    case HttpMethod::CONNECT:
      return "CONNECT";
    case HttpMethod::OPTIONS:
      return "OPTIONS";
    case HttpMethod::TRACE:
      return "TRACE";
    case HttpMethod::PATCH:
      return "PATCH";
    default:
      return std::string();
  }
}

string Utility::to_string(HttpVersion version){
  switch (version) {
    case HttpVersion::HTTP_0_9:
      return "HTTP/0.9";
    case HttpVersion::HTTP_1_0:
      return "HTTP/1.0";
    case HttpVersion::HTTP_1_1:
      return "HTTP/1.1";
    case HttpVersion::HTTP_2_0:
      return "HTTP/2.0";
    default:
      return std::string();
  }
}

string Utility::to_string(HttpStatusCode status_code){
  switch (status_code) {
    case HttpStatusCode::Continue:
      return "Continue";
    case HttpStatusCode::Ok:
      return "OK";
    case HttpStatusCode::Accepted:
      return "Accepted";
    case HttpStatusCode::MovedPermanently:
      return "Moved Permanently";
    case HttpStatusCode::Found:
      return "Found";
    case HttpStatusCode::BadRequest:
      return "Bad Request";
    case HttpStatusCode::Forbidden:
      return "Forbidden";
    case HttpStatusCode::NotFound:
      return "Not Found";
    case HttpStatusCode::MethodNotAllowed:
      return "Method Not Allowed";
    case HttpStatusCode::ImATeapot:
      return "I'm a Teapot";
    case HttpStatusCode::InternalServerError:
      return "Internal Server Error";
    case HttpStatusCode::NotImplemented:
      return "Not Implemented";
    case HttpStatusCode::BadGateway:
      return "Bad Gateway";
    default:
      return std::string();
  }
}


void HttpMessage::setMethod(const std::string& method_string) {
  std::string method_string_uppercase;
  std::transform(method_string.begin(), method_string.end(),
                 std::back_inserter(method_string_uppercase),
                 [](char c) { return toupper(c); });
  if (method_string_uppercase == "GET") {
    std::cout<<"http method: "<< (int) method_ << std::endl;
    method_= HttpMethod::GET;
  } else if (method_string_uppercase == "HEAD") {
    method_= HttpMethod::HEAD;
  } else if (method_string_uppercase == "POST") {
    method_= HttpMethod::POST;
  } else if (method_string_uppercase == "PUT") {
    method_= HttpMethod::PUT;
  } else if (method_string_uppercase == "DELETE") {
    method_= HttpMethod::DELETE;
  } else if (method_string_uppercase == "CONNECT") {
    method_= HttpMethod::CONNECT;
  } else if (method_string_uppercase == "OPTIONS") {
    method_= HttpMethod::OPTIONS;
  } else if (method_string_uppercase == "TRACE") {
    method_= HttpMethod::TRACE;
  } else if (method_string_uppercase == "PATCH") {
    method_= HttpMethod::PATCH;
  } else {
    throw std::invalid_argument("Unexpected HTTP method");
  }
}

void HttpMessage::setPath(const string path ) 
{ 
    path_ = path; 
    std::cout<<"requested path: "<<path_<<endl;
} 

void HttpMessage::setVersion(const std::string& version_string) {
    std::string version_string_uppercase;
    std::transform(version_string.begin(), version_string.end(),
                  std::back_inserter(version_string_uppercase),
                  [](char c) { return toupper(c); });
    if (version_string_uppercase == "HTTP/0.9") {
        version_ = HttpVersion::HTTP_0_9;
    } else if (version_string_uppercase == "HTTP/1.0") {
        version_ = HttpVersion::HTTP_1_0;
    } else if (version_string_uppercase == "HTTP/1.1") {
        version_ = HttpVersion::HTTP_1_1;
    } else if (version_string_uppercase == "HTTP/2" ||
              version_string_uppercase == "HTTP/2.0") {
        version_ = HttpVersion::HTTP_2_0;
    } else {
        throw std::invalid_argument("Unexpected HTTP version");
    }
}

HttpRequest* HttpRequest::parseRequest(const std::string& request_string) {
    string start_line, header_lines, message_body;
    stringstream iss;
    HttpRequest * request = new HttpRequest();
    string line, method, path, version;  // used for first line
    string key, value;                   // used for header fields
    string uri;
    size_t lpos = 0, rpos = 0;

    rpos = request_string.find("\r\n", lpos);
    if (rpos == std::string::npos) {
        throw std::invalid_argument("Could not find request start line");
    }

    start_line = request_string.substr(lpos, rpos - lpos);
    std::cout<<"start_line: "<<start_line<<std::endl;
    lpos = rpos + 2;
    rpos = request_string.find("\r\n\r\n", lpos);
    if (rpos != std::string::npos) {  // has header
        header_lines = request_string.substr(lpos, rpos - lpos);
        std::cout<<"header_lines: "<<header_lines<<std::endl;
        lpos = rpos + 4;
        rpos = request_string.length();
        if (lpos < rpos) {
        message_body = request_string.substr(lpos, rpos - lpos);
        std::cout<<"message_body: "<<message_body<<std::endl;
        }
    }

    iss.clear();  // parse the start line
    iss.str(start_line);
    iss >> method >> path >> version;
    if (!iss.good() && !iss.eof()) {
        throw std::invalid_argument("Invalid start line format");
    }
    request->setMethod(method);
    request->setPath(path);
    request->setVersion(version);
    if ( request->getVersion() != HttpVersion::HTTP_1_1) {
        throw std::logic_error("HTTP version not supported");
    }

    iss.clear();  // parse header fields
    iss.str(header_lines);
    while (std::getline(iss, line)) {
        std::istringstream header_stream(line);
        std::getline(header_stream, key, ':');
        std::getline(header_stream, value);

        // remove whitespaces from the two strings
        key.erase(std::remove_if(key.begin(), key.end(),
                                [](char c) { return std::isspace(c); }),
                key.end());
        value.erase(std::remove_if(value.begin(), value.end(),
                                [](char c) { return std::isspace(c); }),
                    value.end());
        request->setHeader(key, value);
    }

    // request->setContent(message_body);

    return request;
}

string HttpResponse::constructResponseString (const HttpResponse& response, bool send_content){
    std::ostringstream oss;
    Utility utility;
    oss << utility.to_string(response.getVersion()) << ' ';
    oss << static_cast<int>(response.getStatusCode()) << ' ';
    oss << utility.to_string(response.getStatusCode()) << "\r\n";
    for (const auto& p : response.getHeaders())
      oss << p.first << ": " << p.second << "\r\n";
    oss << "\r\n";
    if (send_content) oss << response.getContent();

    return oss.str();
}


