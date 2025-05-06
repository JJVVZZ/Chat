# Chat
Anon chat
🧾 How to Connect to the Chat from Another Network

✅ 1. Prepare the Server

On the server side (the computer where the server program will run):

Open TCP port 5678 in the firewall (Windows Defender or any other).

Make sure the server has a public IP or VPN is set up, and you know the internal VPN IP.

Check that the server is launched and listening on INADDR_ANY, i.e., all interfaces:


serverAddr.sin_addr.s_addr = INADDR_ANY;

✅ 2. Set Up VPN (one-time setup)

📌 Server:

Set up a VPN server (for example, using Windows VPN Server or OpenVPN).

Add a user account (in your example: user max with password Enterprise).

Enable routing in the VPN settings and in Windows (through the Routing and Remote Access service — RRAS).

📌 Client:
Make sure a VPN connection named home is created (used in "rasdial home max Enterprise").

In rasphone.pbk, the profile must be correctly configured (you can create it through “Create a new connection”).

✅ 3. Change the IP Address in the Client
In your client code, you currently have:


serverAddr.sin_addr.s_addr = inet_addr("192.168.1.121");
You should:

Specify the VPN interface IP address of the server. For example:

serverAddr.sin_addr.s_addr = inet_addr("10.0.0.1"); // Example VPN IP
Check the server's IP in the VPN using ipconfig after the client connects.

✅ 4. Run Everything Step by Step
First, launch the server on your main PC.

On the client machine, start the client program:

It will automatically run rasdial to connect to VPN.

Wait for 5 seconds.

Then attempts to connect to the server.

If the connection fails, the user can start writing messages, which are saved to a temporary file and sent to the server later.

✅ 5. How to Check That It Works?
After starting the server, you should see:
Waiting for client

After starting the client and successful connection:
Connected to the GOD OF WINDOWS

Any messages you type should appear on the server side, and vice versa.

🧼 Additional Notes
Make sure that:

Both PCs are in the same VPN network.

Firewall does not block port 5678.

VPN connects successfully (rasdial returns no error).

If it doesn't work — try temporarily disabling antivirus or firewall on both sides for testing.

