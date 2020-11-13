//
// Created by kir on 13.11.2020.
//

#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

#include "Serial.h"

void Serial::open(const char *device)
{
    std::cout << "try to open device " << device << std::endl;

    _fd = ::open(device, O_RDWR);
    if (_fd < 0) {
        char message[1024];
        sprintf(message, "Device opening failed. [%i] %s\n", errno, strerror(errno));
        throw std::runtime_error(message);
    }

    termios tty;
    if(tcgetattr(_fd, &tty) != 0) {
        char message[1024];
        sprintf(message, "tcgetattr() failed. [%i] %s\n", errno, strerror(errno));
        throw std::runtime_error(message);
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B57600);
    cfsetospeed(&tty, B57600);

    // Save tty settings, also checking for error
    if (tcsetattr(_fd, TCSANOW, &tty) != 0) {
        char message[1024];
        sprintf(message, "tcsetattr() failed. [%i] %s\n", errno, strerror(errno));
        throw std::runtime_error(message);
    }
}

void Serial::close()
{
    ::close(_fd);
    _fd = -1;
}

size_t Serial::read(char *buffer, size_t size) const
{
    return ::read(_fd, buffer, size);
}

void Serial::ping() const
{
    std::cout << "ping\n";
    char b = 'r';
    ::write(_fd, &b, 1);
}

void Serial::send(const char *buffer, size_t size) const
{
    ::write(_fd, buffer, size);
}

