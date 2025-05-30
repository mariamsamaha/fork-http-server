# Simple Fork-Based HTTP Server

## Project Description

This project is a simple concurrent HTTP/1.0 server written in C using the fork-per-request model.  
It listens on a specified TCP port and serves static files (HTML, images, etc.) from a specified directory.  
It demonstrates the use of low-level socket programming, process management (`fork()`), file I/O, and basic concurrency principles.

---

## How to Build

First, compile the source files:

```bash
gcc -o server server.c main.c