
#include "TaggedUDPCommunication.h"
#include "cstring"

using namespace Tribots;
using namespace std;

TaggedUDPCommunication::TaggedUDPCommunication (const char* mb, unsigned int ml)  : socket (mb, ml+1), received_messages (256), received_message_length (256) {;}


TaggedUDPCommunication::~TaggedUDPCommunication () noexcept {;}


unsigned int TaggedUDPCommunication::max_message_length () const noexcept {
  return socket.max_message_length()-1;
}

bool TaggedUDPCommunication::put (unsigned char tag, const char* msg, unsigned int msg_len, unsigned int prio)  {
  char* interbuf = new char [msg_len+1];
  interbuf[0]=static_cast<char>(tag);
  memcpy (interbuf+1, msg, msg_len);
  bool success=socket.put (interbuf, msg_len+1, prio);
  delete [] interbuf;
  return success;
}

unsigned int TaggedUDPCommunication::num_messages (unsigned char tag) noexcept {
  return received_messages[tag].size();
}

bool TaggedUDPCommunication::get (unsigned char tag, const char*& msg, unsigned int& msg_len) noexcept {
  return get (tag, msg, msg_len, num_messages(tag)-1);
}

bool TaggedUDPCommunication::get (unsigned char tag, const char*& msg, unsigned int& msg_len, unsigned int index) noexcept {
  if (index<received_messages[tag].size()) {
    msg = received_messages[tag][index];
    msg_len = received_message_length[tag][index];
    return true;
  }
  msg_len=0;
  msg=NULL;
  return false;
}


unsigned int TaggedUDPCommunication::send () noexcept {
  return socket.send();
}

unsigned int TaggedUDPCommunication::sendto (const struct sockaddr_in& add) noexcept {
  return socket.sendto(add);
}

unsigned int TaggedUDPCommunication::receive ()  {
  return receive_intern (NULL);
}

unsigned int TaggedUDPCommunication::receivefrom (const struct sockaddr_in& add)  {
  return receive_intern (&add);
}

unsigned int TaggedUDPCommunication::receive_intern (const struct sockaddr_in* addp)  {
  // alte Werte wegwerfen
  for (unsigned int i=0; i<256; i++) {
    received_messages[i].clear();
    received_message_length[i].clear();
  }

  // Socket auslesen
  unsigned int num_packets;
  if (addp)
    num_packets=socket.receivefrom(*addp);
  else
    num_packets=socket.receive ();

  // Nachrichten nach Tag sortieren
  const char* msg;
  unsigned int msg_len;
  bool next = socket.get (msg, msg_len);
  while (next) {
    if (msg_len>0) {  // ganz leere Nachrichten ohne Tag ignorieren
      unsigned char tag = static_cast<unsigned char>(msg[0]);
      received_messages[tag].push_back (msg+1);
      received_message_length[tag].push_back (msg_len-1);
    }
    next = socket.get (msg, msg_len);
  }

  return num_packets;
}


bool TaggedUDPCommunication::init_as_server (int port) noexcept {
  return socket.init_as_server (port);
}

bool TaggedUDPCommunication::init_as_client (const char* host, int port) noexcept {
  return socket.init_as_client (host, port);
}

void TaggedUDPCommunication::close () noexcept {
  socket.close();
}

const struct sockaddr_in& TaggedUDPCommunication::partner_address () const noexcept {
  return socket.partner_address();
}

bool TaggedUDPCommunication::started() const noexcept {
  return socket.started();
}

bool TaggedUDPCommunication::as_server() const noexcept {
  return socket.as_server();
}

void TaggedUDPCommunication::clear_send_buffer () noexcept {
  socket.clear_send_buffer();
}

void TaggedUDPCommunication::clear_receive_buffer () noexcept {
  socket.clear_receive_buffer();
  for (unsigned int i=0; i<256; i++) {
    received_messages[i].clear();
    received_message_length[i].clear();
  }
}

UDPCommunicationStatistics TaggedUDPCommunication::send_load (int dt) const noexcept {
  return socket.send_load(dt);
}

UDPCommunicationStatistics TaggedUDPCommunication::receive_load (int dt) const noexcept {
  return socket.receive_load(dt);
}

