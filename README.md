Program to check socket behaviour during fork

Open a socket and listen on it.
Before doing accept, do a fork().
Now both process listens on the same listen socket queue.
This is quite different from REUSE_PORT behaviour where 2 sockets are created and LBalanced.

Here whichever process is first in the sleep queue will be woken up and serve the client.

In multiple process with REUSEPORT method, the TCP layer will do Load balance the clients.
