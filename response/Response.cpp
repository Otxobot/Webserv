/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:31 by abasante          #+#    #+#             */
/*   Updated: 2024/06/11 18:09:39 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(): _headers(""), _statusCode(200)
{
    //Default constructor
}

Response::~Response()
{
    //Default destructor
}

std::string Response::getHeaders()
{
    return this->_headers;
}

std::string Response::getStatusCodeTranslate(int status_code)
{
    std::string status = "";
    switch (status_code)
    {
        case 200:
            status = "OK";
            break;
        case 301:
            status = "Moved Permanently\r\n";
            break;
        case 400:
            status = "Bad Request\r\n";
            break;
        case 403:
            status = "Forbidden\r\n";
            break;
        case 404:
            status = "Not Found\r\n";
            break;
        case 405:
            status = "Method Not Allowed\r\n";
            break;
        case 500:
            status = "Internal Server Error\r\n";
            break;
        case 501:
            status = "Not Implemented\n\r";
            break;
        case 502:
            status = "Bad Geteway\r\n";
            break;
        case 413:
            status = "Payload Too Large\r\n";
            break;
        case 505:
            status = "HTTP Version Not Supported\r\n";
            break;
        default:
            status = "Internal Server Error\r\n";
            break;
    }
    return status;
}

Config Response::calibrate_host_location(std::vector<Config> _servers, Request _request)
{
    int i = 0;
    int size = _servers.size();
    while (i < size)
    {
        if (_servers[i]._port == _request.getPort())
        {
            return (_servers[i]);
        }
        i++;
    }
    //return _servers[i];
    throw std::runtime_error("No server found with the requested port");
}

std::string readFileToString(const std::string& filename) {
    std::ifstream file(filename.c_str()); // c_str() needed for C++98
    if (file) {
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string content = ss.str();
        if (content.empty()) {
            std::cerr << "Warning: File content is empty" << std::endl;
        }
        return content;
    } else {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return "<!DOCTYPE html>\n"
               "<html>\n"
               "<head>\n"
               "<title>Error Page</title>\n"
               "<style>\n"
               "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }\n"
               "h1 { font-size: 50px; }\n"
               "p { font-size: 24px; }\n"
               "</style>\n"
               "</head>\n"
               "<body>\n"
               "<h1>Error page</h1>\n"
               "<p>Status code: 500 </p>\n"
               "<p> Internal Server Error\r\n </p>\n"
               "</body>\n"
               "</html>\n";
    }
}
void Response::handle_SC_error(int sc)
{
    this->_response.append("HTTP/1.1");
    this->_response.append(" ");
    this->_statusCode = sc;
    std::ostringstream oss;
    oss << sc;
    std::string statusCodeStr = oss.str();
    std::string html;
    if (this->_server._errorpage.count(sc)) {
    std::string filename = this->_server._root + "/" + this->_server._errorpage[sc];
    html = readFileToString(filename);
    }
    else {
    html =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>Error Page</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }"
        "h1 { font-size: 50px; }"
        "p { font-size: 24px; }"
        "</style>"
        "</head>"
        "<body>"
        "<h1>Error page</h1>"
        "<p>Status code: " + statusCodeStr + "</p>"
        "<p>" + this->getStatusCodeTranslate(sc) + "</p>"
        "</body>"
        "</html>";
    }
    std::ostringstream ossSize;
    ossSize << html.size();
    std::string html_size = ossSize.str();
    this->_body = html;
    this->_response.append(oss.str());
    this->_response.append(" " + this->getStatusCodeTranslate(sc) + "Content-Type: ");
    this->_response.append("text/html\r\n");
            this->_response.append("Content-Length: ");
            this->_response.append(html_size);
            this->_response.append("\r\nConnection: Closed");
            this->_response.append("\r\n\r\n");
            this->_response.append(_body);
}

void Response::createDirectoryListing(std::string directoryPath) {
    DIR* dir;
    struct dirent* ent;
    std::string htmlContent = "<html><body><ul>";

    if ((dir = opendir(directoryPath.c_str())) != NULL) {
        // Print all the files and directories within directory
        while ((ent = readdir(dir)) != NULL) {
            htmlContent += "<li><a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a></li>";
        }
        closedir(dir);
    } else {
        // Could not open directory
        perror("Could not open directory");
        return;
    }

    htmlContent += "</ul></body></html>";

    // Write the HTML content to a file
    std::ofstream htmlFile;
    std::string index = "/autoindex.html";
    std::string open = directoryPath + index;
    htmlFile.open(open.c_str());
    htmlFile << htmlContent;
    htmlFile.close();
    std::string filePath = this->_server._root + "/autoindex.html";
    std::ifstream file(filePath.c_str());

    if (file.is_open())
    {
        // Start of the HTTP response
        this->_response = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Connection: close\r\n"
                          "\r\n";

        // Append each line of the file to the response
        std::string line;
        while (std::getline(file, line)) {
            this->_response += line + "\n";
        }
        file.close();
    }
    else
        this->handle_SC_error(500);
}

void Response::responseCreation(std::vector<Config> &servers, Request &request)
{
    this->_request = request;
    this->_servers = servers;
    this->_server = this->calibrate_host_location(this->_servers, this->_request);
    
    std::string uri = this->_request.getTarget();
    std::string method = this->_request.getMethod();
    std::string protocol = this->_request.getProtocol();
    this->_statusCode = this->_request.getStatusCode();

    if ((method != "GET" && method != "POST" && method != "DELETE") ||
    ((!this->_server._locations[uri]._allowGET && method == "GET") || (!this->_server._locations[uri]._allowDELETE && method == "DELETE") ||
    ((!this->_server._locations[uri]._allowPOST && method == "POST"))))
    {
        this->_statusCode = 405;
        this->handle_SC_error(this->_statusCode);
        return ;
    }

    if (uri == "/" && this->_server._locations[uri]._file != "index.html" && (!this->_server._locations[uri]._autoindex))
    {
        createDirectoryListing(this->_server._root);
        return ;
    }
    if ((this->_request.getTarget()).find("/cgi-bin") != std::string::npos && method == "POST")
    {
        if (this->handle_POST_CGI())
        {
            this->handle_SC_error(500);
            return;
        }
        return ;
    }
    if (this->_request.getQueryString().find("=") != std::string::npos && method == "GET")
    {
        if (this->handle_GET_CGI())
        {
            this->handle_SC_error(500);
            return ;
        }
        return ;
    }

    if (request.getMethod() == "GET")
    {
        if (!this->_server._locations[uri]._redirect.empty())
        {
            std::string redirect = this->_server._locations[uri]._redirect;
            redirect.erase(0, redirect.find_first_not_of(' '));
            redirect.erase(redirect.find_last_not_of(' ') + 1);
            this->_response.append("HTTP/1.1");
            this->_response.append(" ");
            this->_statusCode = 301;
            this->_response.append("301");
            this->_response.append(" ");
            this->_response.append(getStatusCodeTranslate(301));
            this->_response.append("Location: ");
            this->_response.append(redirect);
            this->_response.append("\r\nContent-Type: text/html; charset=UTF-8\r\n");
            this->_response.append("Content-Length: 0\r\n");
            this->_response.append("Connection: close");
            this->_response.append("\r\n\r\n");
            return ;
        }
        else
        {
            this->handle_GET();
        }
    }
    if (request.getMethod() == "POST")
    {
        this->handle_POST(protocol);
    }
    if (request.getMethod() == "DELETE")
        this->handle_DELETE();
}

void Response::handle_GET()
{
    time_t _time;
	std::string tm;
	time(&_time);
	tm = ctime(&_time);
    tm.erase(tm.length() - 1);

    std::string contentType;
    if (this->_server._locations[this->_request.getTarget()]._file.find(".html") != std::string::npos)
        contentType = "text/html\r\n";
    else if (this->_server._locations[this->_request.getTarget()]._file.find(".txt") != std::string::npos)
        contentType = "text/plain\r\n";
    this->createBody();
    int number = this->_statusCode;
    if (number != 200)
        return ;
    this->_response.append("HTTP/1.1");
    this->_response.append(" ");
    std::ostringstream oss;
    oss << number;
    std::string status_code = oss.str();
    this->_response.append(status_code);
    this->_response.append(" OK\r\n");
    this->_response.append("Date: ");
    this->_response.append(tm);
    this->_response.append(" GMT\r\n");
    this->_response.append("Content-Type: ");
    this->_response.append(contentType);
    this->_response.append("Server: ");
    this->_response.append(this->_server._servername + "\r\n");
    this->_response.append(this->_body);
}

std::streampos getFileSize(const std::string& filePath) {
    std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streampos fileSize = file.tellg();
        file.close();
        return fileSize;
    } else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return -1;
    }
}
std::string urlDecode(const std::string& encoded) {
    std::string decoded;
    std::istringstream iss(encoded);
    char ch;
    while (iss.get(ch)) {
        if (ch == '+') {
            decoded += ' ';
        } else if (ch == '%' && iss.get(ch)) {
            int hex;
            if (isdigit(ch)) {
                hex = ch - '0';
            } else {
                hex = toupper(ch) - 'A' + 10;
            }
            hex *= 16;
            if (iss.get(ch)) {
                if (isdigit(ch)) {
                    hex += ch - '0';
                } else {
                    hex += toupper(ch) - 'A' + 10;
                }
                decoded += static_cast<char>(hex);
            }
        } else {
            decoded += ch;
        }
    }
    return decoded;
}

void Response::writeUrlEncodedToFile(const std::string& content)
{
     std::string root = this->_server._root + "/upload/output.txt";
    std::ofstream outFile(root.c_str());
    std::string decodedContent = urlDecode(content);
    if (outFile.is_open()) {
        outFile.write(decodedContent.c_str(), decodedContent.size());
        outFile.close();
        this->_response.append("Content-Length: 0");
        this->_response.append("\r\n");
        this->_response.append("Connection: Closed\r\n");
        this->_response.append("\r\n");
    } else {
        this->_statusCode = 500;
        this->handle_SC_error(this->_statusCode);
    }
}

void Response::handle_POST(const std::string& protocol) 
{
    time_t _time;
	std::string tm;
	time(&_time);
	tm = ctime(&_time);
    tm.erase(tm.length() - 1);

    this->_response.append(protocol);
    this->_response.append(" ");
    int number = this->_statusCode;
    std::ostringstream oss;
    oss << number;
    std::string status_code = oss.str();
    std::cout << status_code << std::endl;
    if (status_code == "200" || status_code == "201") {
        this->_response.append(status_code);
        this->_response.append(" OK\r\n");
        this->_response.append("Date: ");
        this->_response.append(tm);
        this->_response.append(" GMT\r\n");
    } else {
        this->_response.append(status_code);
        std::string message = this->getStatusCodeTranslate(number);
        this->_response.append(" ");
        this->_response.append(message);
        this->_response.append("\r\nDate: ");
        this->_response.append(tm);
        this->_response.append(" GMT\r\n");
        this->handle_SC_error(number);
        return;
    }
    // Handle saving the request data
    std::string uri = this->_request.getTarget();
    Location our_location;
    if (uri.empty())
        uri.append("/");
    our_location = this->_server._locations[uri];
    // Detect the content type and file extension
    std::string contentType = this->_request.headers["Content-Type"];
    std::string fileExtension;
    if (contentType == "text/html") {
        fileExtension = ".html";
    } else if (contentType == "text/plain") {
        fileExtension = ".txt";
    } else if (contentType == "image/jpeg") {
        fileExtension = ".jpg";
    } else if (contentType == "image/png") {
        fileExtension = ".png";
    } else if (contentType == "application/pdf") {
        fileExtension = ".pdf";
    }
    else if (contentType == "application/x-www-form-urlencoded" || contentType == "application/octet-stream"){
        writeUrlEncodedToFile(_request.getBody());
        return ; 
    } else {
        std::cout << "Unsupported Media Type" << std::endl;
        this->_statusCode = 415; // Unsupported Media Type
        this->handle_SC_error(this->_statusCode);
        return;
    }
    // Construct the file path
    std::string filePath = this->_server._root + "/" +this->_request.headers["filename"];
    std::ofstream outFile(filePath.c_str(), std::ios::binary);
    if (outFile.is_open()) {
        // Write body to file
        outFile.write(this->_request.headers["value"].c_str(), this->_request.headers["value"].size());
        outFile.close();
        std::ostringstream len_stream;
        len_stream << getFileSize(filePath);
        std::string content_length = len_stream.str();
        this->_response.append("Content-Length: ");
        this->_response.append(content_length);
        this->_response.append("\r\n");
        this->_response.append("Connection: Closed\r\n");
        this->_response.append("\r\n");
    } else {
        this->_statusCode = 500;
        this->handle_SC_error(this->_statusCode);
    }
}

bool isDirectory(std::string path)
{
    DIR *dir;

    if ((dir = opendir(path.c_str())))
    {
        closedir(dir);
        return true;
    }
    return false;
}

void Response::handle_DELETE()
{
    time_t _time;
	std::string tm;
	time(&_time);
	tm = ctime(&_time);
    tm.erase(tm.length() - 1);

    this->_response.append("HTTP/1.1");
    this->_response.append(" ");
    std::string _path = this->_request.getTarget();
    if (isDirectory(_path))
            this->handle_SC_error(409);
        else
        {
            if (access(_path.c_str(), F_OK))
               this->handle_SC_error(500);
            else
            {
                if (!access(_path.c_str(), W_OK))
                {
                    if (std::remove(_path.c_str()))
                        this->handle_SC_error(500);
                    else
                    {
                        this->_response.append("204");
                        this->_response.append(" ");
                        this->_response.append("No Content\r\n");
                        this->_response.append("Date: ");
                        this->_response.append(tm);
                        this->_response.append(" GMT\r\n");
                        this->_response.append("Content-Length: 0\r\n");
                        this->_response.append("Connection: close\r\n\r\n");
                    }
                }
                else
                    this->handle_SC_error(403);
            }
        }
}

void Response::createBody()
{
    std::string uri = this->_request.getTarget();
    uri.erase(0, uri.find_first_not_of(' '));
    uri.erase(uri.find_last_not_of(' ') + 1);
    Location our_location;

    if (uri.empty())
        uri.append("/");

    our_location = this->_server._locations[uri];
    std::string path;
    if (!our_location._file.empty())
        path = this->_server._root + "/" + our_location._file;
    else
        path = this->_server._root + uri;
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        this->handle_SC_error(404);
        return ;
    }
    else
    {
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string Content = ss.str();
        this->_body.append("Content-Length: ");
        std::ostringstream oss;
        oss.str("");
        oss << Content.size();
        this->_body.append(oss.str());
        this->_body.append("\r\nConnection: close\r\n");
        this->_body.append("\r\n\r\n");
        this->_body.append(ss.str());
    }
}
