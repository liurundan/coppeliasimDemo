
#include "NonspecificTaggedUDPCommunication.h"
#include <cstring>

using namespace Tribots;
using namespace std;


NonspecificTaggedUDPCommunication::NonspecificTaggedUDPCommunication (const char* magic_bytes, unsigned int maxbufsize)  : socket(magic_bytes, maxbufsize) {;}

NonspecificTaggedUDPCommunication::~NonspecificTaggedUDPCommunication () noexcept {
  close ();
}

bool NonspecificTaggedUDPCommunication::init_as_server (int port) noexcept {
  return socket.init_as_server (port);
}

bool NonspecificTaggedUDPCommunication::init_as_client (const char* host, int port) noexcept {
  return socket.init_as_client (host, port);
}

void NonspecificTaggedUDPCommunication::close () noexcept {
  socket.close();
}

unsigned int NonspecificTaggedUDPCommunication::max_message_length () const noexcept {
  return socket.max_message_length();
}

const struct sockaddr_in& NonspecificTaggedUDPCommunication::partner_address () const noexcept {
  return socket.partner_address();
}

bool NonspecificTaggedUDPCommunication::started() const noexcept {
  return socket.started();
}

bool NonspecificTaggedUDPCommunication::as_server() const noexcept {
  return socket.as_server();
}

unsigned int NonspecificTaggedUDPCommunication::send () noexcept {
  return socket.send();
}

unsigned int NonspecificTaggedUDPCommunication::sendto (const struct sockaddr_in& add) noexcept {
  return socket.sendto(add);
}

unsigned int NonspecificTaggedUDPCommunication::receive ()  {
  return socket.receive();
}

unsigned int NonspecificTaggedUDPCommunication::receivefrom (const struct sockaddr_in& add)  {
  return socket.receivefrom (add);
}

void NonspecificTaggedUDPCommunication::clear_send_buffer () noexcept {
  socket.clear_send_buffer();
}

void NonspecificTaggedUDPCommunication::clear_receive_buffer () noexcept {
  socket.clear_receive_buffer();
}

UDPCommunicationStatistics NonspecificTaggedUDPCommunication::send_load (int dt) const noexcept {
  return socket.send_load(dt);
}

UDPCommunicationStatistics NonspecificTaggedUDPCommunication::receive_load (int dt) const noexcept {
  return socket.receive_load(dt);
}



void NonspecificTaggedUDPCommunication::write_signed_short (char* buffer, signed short int val) noexcept {
  write_unsigned_short (buffer, static_cast<unsigned short int>(val));
}

signed short int NonspecificTaggedUDPCommunication::read_signed_short (const char* buffer) noexcept {
  return static_cast<signed int>(read_unsigned_short (buffer));
}

void NonspecificTaggedUDPCommunication::write_signed_short (char* buffer, double val) noexcept {
  write_signed_short (buffer, static_cast<signed short int>(val));
}

void NonspecificTaggedUDPCommunication::write_unsigned_short (char* buffer, unsigned short int val) noexcept {
  buffer[0]=static_cast<unsigned char>(val%256);
  buffer[1]=static_cast<unsigned char>(val/256);
}

unsigned short int NonspecificTaggedUDPCommunication::read_unsigned_short (const char* buffer) noexcept {
  return static_cast<unsigned short int>(static_cast<unsigned char>(buffer[0]))+(static_cast<unsigned short int>(static_cast<unsigned char>(buffer[1]))<<8);
}

void NonspecificTaggedUDPCommunication::write_unsigned_short (char* buffer, double val) noexcept {
  write_unsigned_short (buffer, static_cast<unsigned short int>(val));
}

char* NonspecificTaggedUDPCommunication::write_string_nl (char* buffer, const std::string& src, char es) noexcept {
  memcpy (buffer, src.c_str(), src.length());
  char* pt = buffer+src.length();
  *(pt++)=es;
  return pt;
}

const char* NonspecificTaggedUDPCommunication::read_string_nl (const char* buffer, std::string& dest, unsigned int maxn, char es) noexcept {
  unsigned int endpos=0;
  while (endpos<maxn || maxn==0) {
    if (buffer[endpos]==es)
      break;
    endpos++;
  }
  if (buffer[endpos]==es)
    dest = string (buffer, endpos);
  else
    dest = string (buffer, endpos+1);
  return buffer+endpos+1;
}
