# webserv

42 seoul 3인 팀 프로젝트, C++로 자체 웹 서버를 제작합니다. HTTP 서버를 실행하고, 요청을 처리하고, 구성 파일을 구문 분석할 수 있는 프로그램입니다.

Team : [Feedbaek](https://github.com/Feedbaek) / [OverRaddit](https://github.com/OverRaddit) / [Rob-Yoo](https://github.com/Rob-Yoo)

## About
이 프로젝트는 42 seoul 핵심 교육 과정의 일부 입니다. 이 프로젝트의 목표는 C++98을 사용하여 기본적인 HTTP 웹 서버를 처음부터 구축하는 것입니다. 이 웹 서버는 HTTP GET, POST, PUT, DELETE 요청을 처리할 수 있으며, 지정된 루트 디렉토리에서 정적 파일을 제공하거나 CGI를 사용하여 동적 콘텐츠를 제공할 수 있습니다. 또한 select()의 도움을 받아 여러 클라이언트 연결을 동시에 처리할 수 있습니다

프로젝트의 자세한 내용은 root 경로에 webserv_en.subject.pdf 파일을 참고하세요

# Parts of a webserv

기본적인 HTTP 웹 서버로서 클라이언트의 HTTP 요청을 수신 및 처리하기 위해 여러 구성 요소로 구성됩니다. 아래는 우리 webserv의 주요 부분입니다.

## Server Core
TCP 연결을 처리하고 들어오는 요청을 수신하고 응답을 다시 보내는 등의 작업을 수행하는 웹 서버의 네트워킹 부분입니다. 소켓 생성 및 관리, 입력 및 출력 스트림 처리, 서버와 클라이언트 간의 데이터 흐름 관리와 같은 웹 서버의 낮은 수준 네트워킹 작업을 담당합니다.

### I/O Multiplexing
webserv 프로젝트의 핵심인 I/O Multiplexing을 여기에 설명하기엔 너무 양이 많기 때문에, 이미 [잘 정리된 글](https://blog.naver.com/n_cloudplatform/222189669084)을 참고하시면 좋을 것 같습니다.


## Request Parsing

webserv의 Parsing 부분은 HTTP Request에서 정보를 해석하고 추출하는 프로세스를 나타냅니다. webserv에서 Request Parsing은 Request 클래스에 의해 수행됩니다. 들어오는 요청을 받아 구문 분석하고 메서드, 경로, 헤더 및 메시지 본문(있는 경우)과 같은 관련 정보를 추출합니다.

## Response Building

Response 클래스는 Request 대한 응답으로 클라이언트에 다시 전송되는 HTTP 응답을 구성하고 형식화 합니다. 또한 status, header 및 message body를 포함하여 HTTP 응답을 작성하고 저장하는 일을 담당합니다.

## Configuration File

config 파일은 webserv 작동 방식을 지정하는 다양한 설정과 지시문이 포함된 텍스트 파일입니다. 이러한 설정에는 웹 서버가 수신해야 하는 포트 번호, 웹 서버의 루트 디렉터리 위치 및 기타 여러 설정이 포함될 수 있습니다.

다음은 config 파일 형식을 보여주는 예제 파일입니다.
<br>

  ```nginx
server {
	listen 4242 4243;

	server_name localhost;

	root sudo;

	index index.html;
	cgi cgi_tester .bla;

	location / {
		limit_except GET;
	}

	location /post_body {
		limit_except POST;
		client_max_body_size 100;
	}

	location /put_test {
		limit_except PUT;
		upload file_storage;
	}

	location /directory {
		root YoupiBanane;
		index youpi.bad_extension;
	}
}
  ```

## CGI

CGI는 웹 서버에서 외부 프로그램을 실행하기 위한 표준 프로토콜 입니다. 사용자가 CGI 프로그램에 의해 처리되어야 하는 웹 페이지를 요청하면 웹 서버는 CGI 프로그램을 실행하고 동적으로 생성된 결과물을 사용자의 웹 브라우저에 응답 해줍니다.

CGI 프로그램은 Perl, Python, Php 또는 bash와 같은 프로그래밍 언어로 작성할 수 있는 단순한 스크립트이며 일반적으로 사용자가 웹 브라우저를 통해 제출한 데이터를 처리하거나 웹 페이지에서 동적 콘텐츠를 생성하는 데 사용됩니다.


<p align="center">
  <img width="60%" height="50%" src="https://i1.ae/img/webserv/CGI.jpg">
</p>

# References
__Networking__
- [Create a simple HTTP server in c](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
- [(Video) Create a simple web server in c](https://www.youtube.com/watch?v=esXw4bdaZkc&ab_channel=JacobSorber)
- [(Video) explaining select()](https://www.youtube.com/watch?v=Y6pFtgRdUts&ab_channel=JacobSorber)
- [IBM - Nonblocking I/O and select()](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm)
- [All about sockets blocking](http://dwise1.net/pgm/sockets/blocking.html)
- [TCP Socket Programming: HTTP](https://w3.cs.jmu.edu/kirkpams/OpenCSF/Books/csf/html/TCPSockets.html)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)

__HTTP__
- [MDN - HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP)
- [An Overview of the HTTP as Coverd in RFCs](https://www.inspirisys.com/HTTP_Protocol_as_covered_in_RFCs-An_Overview.pdf)
- [How the web works: HTTP and CGI explained](https://www.garshol.priv.no/download/text/http-tut.html)
- [MIME](https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types)
- [HTTP Status Codes](https://umbraco.com/knowledge-base/http-status-codes/)

__RFC__
- [How to Read an RFC](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)
- [RFC 9110 - HTTP Semantics ](https://www.rfc-editor.org/info/rfc9110)
- [RFC 9112 - HTTP/1.1 ](https://www.rfc-editor.org/info/rfc9112)
- [RFC 2068 - ABNF](https://www.cs.columbia.edu/sip/syntax/rfc2068.html)
- [RFC 3986 -  (URI) Generic Syntax](https://www.ietf.org/rfc/rfc3986)
- [RFC 6265 - HTTP State Management Mechanism (Cookies)](https://www.rfc-editor.org/rfc/rfc6265)
- [RFC 3875 - CGI](https://datatracker.ietf.org/doc/html/rfc3875)

__CGI__
- [Python web Programming](https://www.tutorialspoint.com/python/python_cgi_programming.htm)
- [CPP web Programming](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)
- [(Video) Creating a file upload page](https://www.youtube.com/watch?v=_j5spdsJdV8&t=562s)

__StackOverFlow__
- [What HTTP response headers are required](https://stackoverflow.com/questions/4726515/what-http-response-headers-are-required)
- [Why do we cast sockaddr_in to sockaddr when calling bind()](https://stackoverflow.com/questions/21099041/why-do-we-cast-sockaddr-in-to-sockaddr-when-calling-bind)
- [Is an entity body allowed for an HTTP DELETE request?](https://stackoverflow.com/questions/299628/is-an-entity-body-allowed-for-an-http-delete-request)
- [Sending images over http to browser in C](https://stackoverflow.com/questions/28631767/sending-images-over-http-to-browser-in-c)
- [Handling whitespaces in http headers](https://stackoverflow.com/questions/31773667/handling-whitespaces-in-http-headers)

__Tools__
- [Postman](https://www.postman.com/downloads/) : Send custom requests to the server
- [PuTTY](https://www.putty.org/) : Send raw data to the server (Windows Only)
    - [Video: How to use](https://www.youtube.com/watch?v=ptJYNY7UbQU&ab_channel=GeekThis)
- [Wireshark]() : Capture request/response traffic
- [Sige](https://www.linode.com/docs/guides/load-testing-with-siege/) : Load testing

__Other__
- [URL Encoding](https://www.urlencoder.io/learn/#:~:text=A%20URL%20is%20composed%20from,%22%20%2C%20%22~%22%20)
- [Nginx](https://nginx.org/en/)
- [Nginx Source Code](https://github.com/nginx/nginx)