USE seccon_sqli;

INSERT INTO articles
(title, description, reference, status)
VALUES
('RFC 748', 'TELNET RANDOMLY-LOSE Option', '', 0),
('RFC 1097', 'TELNET SUBLIMINAL-MESSAGE option', '', 0),
('RFC 1149', 'Standard for the transmission of IP datagrams on Avian Carriers', '', 0),
('RFC 1216', 'Gigabit Network Economics and Paradigm Shifts', '', 0),
('RFC 1217', 'Memo from the Consortium for Slow Commotion Research (CSCR)', '', 0),
('RFC 1313', 'Today\'s Programming for KRFC AM 1313 Internet Talk Radio', '', 0),
('RFC 1437', 'The Extension of MIME Content-Types to a New Medium', '', 0),
('RFC 1438', 'Internet Engineering Task Force Statements Of Boredom (SOBs)', '', 0),
('RFC 1605', 'SONET to Sonnet Translation', '', 0),
('RFC 1606', 'A Historical Perspective On The Usage Of IP Version 9', '', 0),
('RFC 1607', 'A view from the 21st Century', '', 0),
('RFC 1776', 'The Address is the Message', '', 0),
('RFC 1924', 'A Compact Representation of IPv6 Addresses', '', 0),
('RFC 1925', 'The Twelve Networking Truths', '', 0),
('RFC 1926', 'An Experimental Encapsulation of IP Datagrams on Top of ATM', '', 0),
('RFC 1927', 'Suggested Additional MIME Types for Associating Documents', '', 0),
('RFC 2100', 'The Naming of Hosts', '', 0),
('RFC 2321', 'RITA -- The Reliable Internetwork Troubleshooting Agent', '', 0),
('RFC 2322', 'Management of IP numbers by peg-dhcp', '', 0),
('RFC 2323', 'IETF Identification and Security Guidelines', '', 0),
('RFC 2324', 'Hyper Text Coffee Pot Control Protocol (HTCPCP/1.0)', '', 0),
('RFC 2325', 'Definitions of Managed Objects for Drip-Type Heated Beverage Hardware Devices using SMIv2', '', 0),
('RFC 2549', 'IP over Avian Carriers with Quality of Service', 'RFC 1149', 0),
('RFC 2550', 'Y10K and Beyond', '', 0),
('RFC 2551', 'The Roman Standards Process -- Revision III', '', 0),
('RFC 2795', 'The Infinite Monkey Protocol Suite (IMPS)', '', 0),
('RFC 3091', 'Pi Digit Generation Protocol', '', 0),
('RFC 3092', 'Etymology of "Foo"', '', 0),
('RFC 3093', 'Firewall Enhancement Protocol (FEP)', '', 0),
('RFC 3251', 'Electricity over IP', '', 0),
('RFC 3252', 'Binary Lexical Octet Ad-hoc Transport', '', 0),
('RFC 3514', 'The Security Flag in the IPv4 Header (Evil Bit)', '', 0),
('RFC 3751', 'Omniscience Protocol Requirement', '', 0),
('RFC 4041', 'Requirements for Morality Sections in Routing Area Drafts', '', 0),
('RFC 4042', 'UTF-9 and UTF-18 Efficient Transformation Formats of Unicode', '', 0),
('RFC 4824', 'The Transmission of IP Datagrams over the Semaphore Flag Signaling System (SFSS)', '', 0),
('RFC 5241', 'Naming Rights in IETF Protocols', '', 0),
('RFC 5242', 'A Generalized Unified Character Code: Western European and CJK Sections', '', 0),
('RFC 5513', 'IANA Considerations for Three Letter Acronyms', '', 0),
('RFC 5514', 'IPv6 over Social Networks', '', 0),
('RFC 5841', 'TCP Option to Denote Packet Mood', '', 0),
('RFC 5984', 'Increasing Throughput in IP Networks with ESP-Based Forwarding: ESPBasedForwarding', '', 0),
('RFC 6214', 'Adaptation of RFC 1149 for IPv6', '', 0),
('RFC 6217', 'Regional Broadcast Using an Atmospheric Link Layer', '', 0),
('RFC 6592', 'The Null Packet', '', 0),
('RFC 6593', 'Service Undiscovery Using Hide-and-Go-Seek for the Domain Pseudonym System (DPS)', '', 0),
('RFC 6919', 'Further Key Words for Use in RFCs to Indicate Requirement Levels', '', 0),
('RFC 6921', 'Design Considerations for Faster-Than-Light (FTL) Communication', '', 0),
('RFC 7168', 'The Hyper Text Coffee Pot Control Protocol for Tea Efflux Appliances (HTCPCP-TEA)', '', 0),
('RFC 7169', 'The NSA (No Secrecy Afforded) Certificate Extension', '', 0),
('RFC 7511', 'Scenic Routing for IPv6', '', 0),
('RFC 7514', 'Really Explicit Congestion Notification (RECN)', '', 0),
('RFC 8135', 'Complex Addressing in IPv6', '', 0),
('RFC 8136', 'Additional Transition Functionality for IPv6', '', 0),
('RFC 8140', 'The Arte of ASCII: Or, An True and Accurate Representation of an Menagerie of Thynges Fabulous and Wonderful in Ye Forme of Character', '', 0),
('RFC 8367', 'Wrongful Termination of Internet Protocol (IP) Packets', '', 0),
('RFC 8369', 'Internationalizing IPv6 Using 128-Bit Unicode', '', 0),
('RFC 8565', 'Hypertext Jeopardy Protocol (HTJP/1.0)', '', 0),
('RFC 8567', 'Customer Management DNS Resource Records', '', 0),
('FLAG', 'The flag is "SECCON{Yeah_Sqli_Success_" ... well, the rest of flag is in "flag" table. Try more!', '', 1);


INSERT INTO flag
(piece)
VALUES
('You_Win_Yeah}');
