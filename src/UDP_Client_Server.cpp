#include "../libraries/UDP_Client_Server.hpp"
#include <string.h>
#include <unistd.h>

namespace udp_client_server
{

    // ======================= CLIENT ==========================

    // This function initializes the UDP client object
    // The port is expected to be a host side port number (i.e. 59200).
    // addr should be an IPv4 or IPv6 textual address, it can represents a host name or an address defined 
    // only with numbers, if the address cannot be resolved the constructor will throw an error. 
    udp_client::udp_client(const std::string &addr, int port)
        : f_port(port), f_addr(addr)
    {
        char decimal_port[16];
        snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
        decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
        int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));

        if (r != 0 || f_addrinfo == NULL)
        {
            throw udp_client_server_runtime_error(("Invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
        }

        f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);

        if (f_socket == -1)
        {
            freeaddrinfo(f_addrinfo);
            throw udp_client_server_runtime_error(("Could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
        }
    }

    // This function is made for clean up, here we free the address information structure
    // and close the socket before running
    udp_client::~udp_client()
    {
        freeaddrinfo(f_addrinfo);
        close(f_socket);
    }

    int udp_client::get_socket() const
    {
        return f_socket;
    }

    int udp_client::get_port() const
    {
        return f_port;
    }

    std::string udp_client::get_addr() const
    {
        return f_addr;
    }

    // return -1 in case of an error occurs, otherwise return the number of bytes sent.
    // param msg = message to be send.
    // param size = number of bytes representing this message
    int udp_client::send(const char *msg, size_t size)
    {
        return sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
    }

    // ======================= SERVER ==========================

    //This function initializes the server and make it ready to start receiving messages.
    udp_server::udp_server(const std::string &addr, int port)
        : f_port(port), f_addr(addr)
    {
        char decimal_port[16];
        snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
        decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
        int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));

        if (r != 0 || f_addrinfo == NULL)
        {
            throw udp_client_server_runtime_error(("Invalid address or port for UDP socket: \"" + addr + ":" + decimal_port + "\"").c_str());
        }

        f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);

        if (f_socket == -1)
        {
            freeaddrinfo(f_addrinfo);
            throw udp_client_server_runtime_error(("Could not create UDP socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
        }

        r = bind(f_socket, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);

        if (r != 0)
        {
            freeaddrinfo(f_addrinfo);
            close(f_socket);
            throw udp_client_server_runtime_error(("Could not bind UDP socket with: \"" + addr + ":" + decimal_port + "\"").c_str());
        }
    }

    // This function is made for clean up, here we free the address information structure
    // and close the socket before running
    udp_server::~udp_server()
    {
        freeaddrinfo(f_addrinfo);
        close(f_socket);
    }

    int udp_server::get_socket() const
    {
        return f_socket;
    }

    int udp_server::get_port() const
    {
        return f_port;
    }

    std::string udp_server::get_addr() const
    {
        return f_addr;
    }

    // This function waits until a message is received on this UDP server.
    // As we're using UDP protocol, there are no means to return from this function
    // except by receiving a message.

    // Return the number of bytes read or -1 if an error occurs.
    int udp_server::recv(char *msg, size_t max_size)
    {
        return ::recv(f_socket, msg, max_size, 0);
    }

    // This function is similar to the previous one, but we have here a timer to wait until data comes.
    // If we didn't receive anything during the time specified, it will return an error (return -1)
    // Otherwise the number of bytes will be received.

    // Param max_wait_ms defines the maximum time in milliseconds to wait for a message.
    int udp_server::timed_recv(char *msg, size_t max_size, int max_wait_ms)
    {
        fd_set s;
        FD_ZERO(&s);
        FD_SET(f_socket, &s);
        struct timeval timeout;
        timeout.tv_sec = max_wait_ms / 1000;
        timeout.tv_usec = (max_wait_ms % 1000) * 1000;
        int retval = select(f_socket + 1, &s, &s, &s, &timeout);

        if (retval == -1)
        {
            //select() set errno accordingly
            return -1;
        }
        if (retval > 0)
        {
            //our socket has data
            return ::recv(f_socket, msg, max_size, 0);
        }

        // our socket has no data
        errno = EAGAIN;
        return -1;
    }
} // namespace udp_client_server