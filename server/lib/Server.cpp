
#include "Server.hpp"
#include "Client.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string.h>

#include <SDL_net.h>
#include "util.hpp"
#include "format.h"
#include "json11.hpp"
#include "common/util/container.hpp"
namespace cont = common::util::container;

#define PROTOCOL_VERSION 0x00
#define MAGIC_NUMBER 0xCAC35500 | PROTOCOL_VERSION

namespace server {
Server::Server(IPaddress address, unsigned int max_clients)
    : m_logger(stderr, [] { return "SERVER: "; }) {
    m_address = address;
    m_max_clients = max_clients;
    m_socket_set = SDLNet_AllocSocketSet(m_max_clients * 1);
    SDL_version compile_version;
    const SDL_version *link_version = SDLNet_Linked_Version();
    SDL_NET_VERSION(&compile_version);

    m_logger.log("[INFO] Compiled with SDL_net version: {:d}.{:d}.{:d}\n",
                 compile_version.major, compile_version.minor,
                 compile_version.patch);
    m_logger.log("[INFO] Running with SDL_net version: {:d}.{:d}.{:d}\n\n",
                 link_version->major, link_version->minor, link_version->patch);
    if (SDL_Init(0) == -1) {
        m_logger.log("[ERROR] SDL_Init: {}\n", SDL_GetError());
        m_logger.log(
            "[ERROR] Failed to initialize SDL. Quitting zordz-server...\n");
        exit(1);
    }
    if (SDLNet_Init() == -1) {
        m_logger.log("[ERROR] SDLNet_Init: {}\n", SDLNet_GetError());
        m_logger.log(
            "[ERROR] Failed to initialize SDL. Quitting zordz-server...\n");
        exit(1);
    }

    if (!(m_socket = SDLNet_TCP_Open(&address))) {
        m_logger.log("Failed to bind to interface {}", address);
    }
    m_logger.log("Bound to interface {}", m_address);
}

Server::~Server() { m_logger.log("[INFO] Server shut down.\n\n"); }

void Server::acceptConnections() {
    while (true) {
        // Returns immediately with NULL if no pending connections
        TCPsocket client_socket = SDLNet_TCP_Accept(m_socket);

        if (!client_socket) {
            break;
        }

        if (m_clients.size() >= m_max_clients) {
            // Perhaps issue some kind of "server full" warning. But how would
            // this be done as the client would be in the PENDING state
            // intially.
            SDLNet_TCP_Close(client_socket);
        } else {
            m_clients.emplace_back(client_socket);
            SDLNet_TCP_AddSocket(m_socket_set, client_socket);
        }
    }
}

int Server::exec() {
    while (true) {
        acceptConnections();
        SDLNet_CheckSockets(m_socket_set, 0);
        for (auto &client : m_clients) {
            if (client.getState() == PENDING ||
                client.getState() == CONNECTED) {
                client.recv();
            }
        }
        // Remove disconnected clients
        cont::remove_if(m_clients, [](Client const &client) {
            return client.getState() == DISCONNECTED;
        });
    }
    return 1;
}
}
