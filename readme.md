# mchat
mothchat is a multi-threaded, end to end encrypted chat service for unix operating systems. designed to be accessible over SSH/tty.

## client
the client does not depend on anything beyond the standard C library and pthread.

## server
the server uses HTTP for connectivity and thus hosts an HTTP server, by default on the 4818 port.

the server uses libmicrohttpd, which is available commonly on Linux, BSD, and macOS systems.

### macos install
brew install libmicrohttpd


