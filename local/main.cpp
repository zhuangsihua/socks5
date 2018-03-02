/*******************************************************************************
 *
 * socks5
 * A C++11 socks5 proxy server based on Libevent 
 *
 * Copyright 2018 Senlin Zhan. All rights reserved.
 *
 ******************************************************************************/

#include "address.hpp"
#include "cipher.hpp"
#include "server.hpp"

#include <gflags/gflags.h>
#include <glog/logging.h>

// Check whether the port is in range [1, 65535]
static bool isValidPort(const char *flagname, gflags::int32 value)
{
    return (value > 0 && value < 65536);
}

// Check whether the screct key is valid
static bool isValidSecretKey(const char *flagname, const std::string &value)
{
    return value.size() == Cryptor::KEY_SIZE;
}

// Listening address of the local server
DEFINE_string(host, "0.0.0.0", "Listening host");
DEFINE_int32(port, 5050, "Listening port");
DEFINE_validator(port, &isValidPort);

// Listening address of the proxy server
DEFINE_string(remoteHost, "", "Remote host");
DEFINE_int32(remotePort, 6060, "Remote port");
DEFINE_validator(remotePort, &isValidPort);

// Secret key
DEFINE_string(key, "", "Secret key");
DEFINE_validator(key, &isValidSecretKey);

int main(int argc, char *argv[])
{
    auto port = static_cast<unsigned short>(FLAGS_port);
    auto remotePort = static_cast<unsigned short>(FLAGS_remotePort);
    auto address = Address::FromHostOrder(
        Address::Type::ipv4, FLAGS_remoteHost, remotePort
    );

    Server server(FLAGS_host, port, address, FLAGS_key);
    server.run();
    
    return 0;
}
