# Simple Fork-Based HTTP Server in C

## Project Description

This project is a simple concurrent HTTP/1.0 server written in C using the **fork-per-request model**. It listens on a specified TCP port and serves static files (HTML, images, etc.) from a specified directory

It demonstrates:
- Low-level socket programming
- Process management with `fork()`
- File I/O and request parsing
- Basic concurrency principles
  
---

## 🚀 Features

- Concurrent request handling using `fork()`
- Serves static content (HTML, CSS, JS, images)
- Minimal and clean C code
- Lightweight — no external dependencies
- Educational: designed to understand HTTP servers and Unix processes

---

## 🗂️ Project Structure

Bonus/
├── main.c # Entry point: sets up the server and accepts connections
├── server.c # Handles client requests and responses
├── server.h # Header declarations for server logic
├── www/ # Static files served by the server
├── server # Compiled binary
└── README.md # This documentation

---

## How to Build

Make sure you have GCC installed. Then run:

```bash
gcc -o server server.c main.c
