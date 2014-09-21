JNMailbox
=========

Don't go postal: check your mailbox using a JeeNode.

Stay informed about the status of your physical mailbox with a shameless amount of hardware and software.


BASIC CONCEPTS
==============

Here I'm assuming that you've a mailbox with an upper door (the lid) from which the letters are inserted, and a front door from which it's taken out.

Once the mailman open the lid of your mailbox, a JeeNode is powered on and it immediately sends an "upper\_door:open" message.
If the lid is left open (and so the JeeNode powered on), after 30 seconds an "upper\_door:still\_open" message is sent, and the JeeNode turns itself off.

At the other end, a receiver reads the messages and presents a web interface allowing the user to acknowledge the fact that the post was collected.


HARDWARE
========

You will have some hardware in the mailbox to transmit the information that the mailbox was opened, and a receiver inside the home.


For the mailbox:
- a JeeNode Micro
- some batteries to power it
- a switch of some kind to power on the JeeNode, once the lid is opened

Inside the house:
- a Raspberry Pi or another low-power computer
- a JeeLink or another kind of JeeNode


SOFTWARE
========

Install a Raspbian or another distribution of your choice on an SD Card.

Copy these files:
- bin/jn\_mailbox\_daemon in /usr/local/bin; edit it to change the locations of data and pid files (also check the ACK\_FILE\_UID and ACK\_FILE\_GID variables)
- etc/init.d/jn\_mailbox\_daemon in /etc/init.d (also, be sure that it's run at boot, linking it in /etc/rc2.d or calling it from rc.local)
- etc/monit/jn\_mailbox\_daemon\_monit can be (optionally) used to configure monit
- webserver/cgi-bin/jn\_mailbox\_serve\_data must be copyed (with executable permissions) in the CGI directory of your webserver (usually /usr/lib/cgi-bin, for Apache); modify it accordingly to the changes made in the jn\_mailbox\_daemon script.
- the webserver/jnmailbox/ must be copied at the root directory of your webserver (usually /var/www or /var/www/html)

The Arduino software for the sender (in the mailbox) and the receiver (connected to the Raspberry Pi) can be found in the jeenode directory: copy all the directories in your sketchbook directory and upload the software to your JeeNodes.


IMPROVEMENTS
============

- avoid manual ACKs using another sensor at the front door
- power the JeeNode using solar cells
- improve the web interface


LICENSE
=======

(C) 2014 Davide Alberani <da@erlug.linux.it>

This software is released under the terms of the MIT license.

