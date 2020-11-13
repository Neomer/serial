//
// Created by kir on 13.11.2020.
//

#ifndef SERIAL_SERIAL_H
#define SERIAL_SERIAL_H


class Serial
{
public:
    void open(const char *device);

    void close();

    void ping() const;

    void send(const char *buffer, size_t size) const;

    size_t read(char *buffer, size_t size) const;

private:
    int _fd;
};


#endif //SERIAL_SERIAL_H
