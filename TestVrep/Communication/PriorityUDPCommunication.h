
#ifndef Tribots_PriorityUDPCommunication_h
#define Tribots_PriorityUDPCommunication_h

#include "MultiPacketUDPCommunication.h"

namespace Tribots {

  /** UDP-Verbindung, die Nachrichten in internen Puffern nach Prioritaeten
      getrennt sammelt und verschickt */
  class PriorityUDPCommunication {
  public:
    /** Konstruktor; Arg1: zwei Markierungsbytes, Arg2: Maximalgroesse einer Nachricht in Byte */
    PriorityUDPCommunication (const char*, unsigned int =8190) ;
    virtual ~PriorityUDPCommunication () noexcept;
    /** als Server an Port arg1 initialisieren; Rueckgabewert: erfolgreich? */
    virtual bool init_as_server (int) noexcept;
    /** als Client initialisieren; Verbindung mit Rechner arg1 an Port arg2; Rueckgabewert: erfolgreich? */
    virtual bool init_as_client (const char*, int) noexcept;
    /** Verbindung schliessen */
    virtual void close () noexcept;

    /** Wie lange in Byte darf eine Nachricht maximal sein? */
    virtual unsigned int max_message_length () const noexcept;

    /** Alle Pakete senden;
        Return: Anzahl gesendeter Pakete */
    virtual unsigned int send () noexcept;
    /** Alle Pakete an bestimmten Empfaenger (Arg1) senden;
        Return: Anzahl gesendeter Pakete */
    virtual unsigned int sendto (const struct sockaddr_in&) noexcept;
    /** Pakete des naechsten Senders empfangen;
        Return: Anzahl fehlerfrei empfangener Pakete */
    virtual unsigned int receive () ;
    /** Pakete eines Senders (Arg1) empfangen;
        Return: Anzahl fehlerfrei empfangener Pakete */
    virtual unsigned int receivefrom (const struct sockaddr_in&) ;

    /** eine Nachricht hinzufuegen;
        Arg1: die Nachricht,
        Arg2: Laenge der Nachricht in Byte,
        Arg3: Prioritaet der Nachricht,
        Return: konnte Nachricht hinzugefuegt werden? */
    virtual bool put (const char*, unsigned int, unsigned int =0) ;
    /** den Ausgabepuffer loeschen; wird nach "send" auch automatisch aufgerufen */
    virtual void clear_send_buffer () noexcept;
    /** den Eingabepuffer loeschen; wird vor "receive" auch automatisch aufgerufen */
    virtual void clear_receive_buffer () noexcept;
    /** die naechste empfange Nachricht holen 
        Arg1: Rueckgabewert: ein Zeiger auf den Puffer,
        Arg2: Rueckgabewert: die Laenge der Nachricht in Byte,
        Return: lag noch ein Puffer vor?
        Beachte: mit jedem "get"-Aufruf wird die nachfolgende Nachricht abgerufen, 
        "receive" setzt auf die erste empfange Nachricht zurueck */
    virtual bool get (const char*&, unsigned int&) noexcept;

    /** Adresse des letzten Kommunikationspartners */
    virtual const struct sockaddr_in& partner_address () const noexcept;
    /** true, wenn init_as_client oder init_as_server erfolgreich aufgerufen wurde */
    virtual bool started() const noexcept;
    /** liefert true, wenn als Server gestartet */
    virtual bool as_server() const noexcept;

    /** berechnet die durchschnittliche Kommunikationsbelastung durch ausgehende Pakete in den letzten maximal (arg1) Sekunden */
    virtual UDPCommunicationStatistics send_load (int =10) const noexcept;
    /** berechnet die durchschnittliche Kommunikationsbelastung durch eingehende Pakete in den letzten maximal (arg1) Sekunden */
    virtual UDPCommunicationStatistics receive_load (int =10) const noexcept;

  protected:
    MultiPacketUDPCommunication socket;
    char magic_byte [2];

    const unsigned int max_buffer_size;

    std::vector<char*> send_buffers;
    std::vector<unsigned int> send_buffer_length;
    std::vector<unsigned int> send_buffer_priorities;

    std::vector<char*> receive_buffers;
    std::vector<unsigned int> receive_buffer_length;
    unsigned int get_buffer;
    unsigned int get_message;
  };

}

#endif
