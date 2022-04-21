# webserv

[![build](https://github.com/AdrianWR/webserv/actions/workflows/cmake.yml/badge.svg)](https://github.com/AdrianWR/webserv/actions/workflows/cmake.yml)

In the context of this project, a web server is special software capable of litens to internet requests via the HyperText Transfer Protocol (HTTP), letting us communicate two or more devices through a network. With a web server, one may accept and lsiten to requests how may serve static websites, REST API responses or any other piece of data that may be transfered usgin the HTTP network protocol. For the more pratical people, `webserv` aims to implement similar functionalities as classic HTTP web servers, such as the [Apache Web Server](https://httpd.apache.org/) and [NGINX](https://nginx.org/).

## Build and Running

```shell
cmake -B build
cmake --build build
./webserv
```
