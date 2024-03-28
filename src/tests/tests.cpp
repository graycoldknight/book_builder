#define CATCH_CONFIG_MAIN

#include <vector>
#include "string_hash.h"

#include "catch.hpp"
#include "cpplib.h"

#include "OrderBook.hpp"
#include "CommonDefs.h"
#include "OuchMesgs.h"


/*void* operator new(std::size_t sz){
    std::cout << "Allocating: " << sz << '\n';
    return std::malloc(sz);
}*/


TEST_CASE("Ouch Messages") {

  char message[64];
  /*SECTION("heterogeneous lookup") {
    std::cout << "test start" << std::endl;
    std::unordered_map<std::string, int, 
       string_hash, std::equal_to<>> string_int_map;
   string_int_map.emplace("one", 1);
   string_int_map.emplace("two", 2);
   string_int_map.emplace("three", 3);

    char my_one[] = "one";
    REQUIRE(string_int_map[my_one] == 1);
    REQUIRE(string_int_map["two"] == 2);
  } */

  /*SECTION("SymbolOrderBook") {

    SymbolOrderBook<double, int> symbolOrderBook;
    symbolOrderBook.modifySize("AAPL", 198.0, 100, true);
    symbolOrderBook.modifySize("AAPL", 200.0, 100, false);  
    REQUIRE(symbolOrderBook.checkBookConsistency("AAPL") == true);
  }*/

  SECTION("SystemEventMessage")
  {
    unsigned char ucDataBlock[19] = {
	  //<--------packet header------------>
    //|        0|                     13|
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x0D,
    //<--message header----->
    //|       11|    S|    S|
      0x00, 0x0B, 0x53, 0x53, 
      0x00, 0x00, 0x0D, 0x18, 0xC2, 0xE4, 0xD0, 0x74, //timestamp
      0x53//Event enumeration
    };
    unsigned char* ptr = ucDataBlock;
    const PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(ptr);
    REQUIRE(packetHeader->getStreamIdentifier() == 0);
    REQUIRE(packetHeader->getPacketLength() == 13);
    ptr += sizeof(PacketHeader);
    const MessageHeader* messageHeader = reinterpret_cast<MessageHeader*>(ptr);
    REQUIRE(messageHeader->getMessageLength() == 11);
    REQUIRE(messageHeader->ouchPacketType == 'S');
    REQUIRE(messageHeader->ouchMessageType == 'S');
    // note : we dont move the pointer by the size of Message header as system even message extends from message header.
    const SystemEventMessage* systemEventMessage = reinterpret_cast<SystemEventMessage*>(ptr);
    REQUIRE(systemEventMessage->getTimeStamp() == 14400000151668);
    REQUIRE(systemEventMessage->eventEnumeration == 'S');
  }

  SECTION("AcceptedMessage")
  {
    unsigned char ucDataBlock[74] = {
	// Offset 0x00000013 to 0x0000005C
	0x00, 0x03, 0x00, 0x00, 0x00, 0x44, 0x00, 0x42, 0x53, 0x41, 0x00, 0x00,
	0x1D, 0x60, 0xED, 0x5F, 0x5F, 0x14, 0x30, 0x30, 0x30, 0x30, 0x32, 0x30,
	0x33, 0x35, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x42, 0x00, 0x00, 0x03,
	0xE8, 0x42, 0x53, 0x43, 0x51, 0x20, 0x20, 0x20, 0x20, 0x00, 0x03, 0x47,
	0x10, 0x00, 0x01, 0x86, 0x9F, 0x52, 0x42, 0x43, 0x44, 0x59, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xC3, 0x7E, 0xBD, 0x50, 0x4E, 0x00, 0x00, 0x00, 0x00,
	0x4E, 0x4C
  };
    unsigned char* ptr = ucDataBlock;
    const PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(ptr);
    REQUIRE(packetHeader->getStreamIdentifier() == 3);
    REQUIRE(packetHeader->getPacketLength() == 68);
    ptr += sizeof(PacketHeader);
    const MessageHeader* messageHeader = reinterpret_cast<MessageHeader*>(ptr);
    REQUIRE(messageHeader->getMessageLength() == 66);
    REQUIRE(messageHeader->ouchPacketType == 'S');
    REQUIRE(messageHeader->ouchMessageType == 'A');
    const AcceptedMessage* acceptedMessage = reinterpret_cast<AcceptedMessage*>(ptr);
    REQUIRE(acceptedMessage->getTimeStamp() == 32302136516372);
    REQUIRE(strncmp(acceptedMessage->orderToken, "00002035000000", 14) ==  0);
    REQUIRE(acceptedMessage->side=='B');
    REQUIRE(acceptedMessage->getShares() == 1000);
 }
}


