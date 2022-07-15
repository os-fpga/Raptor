import socket
import sys

# Create a Local Unix Domain socket
dp_sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Connect the socket to the diagnostics port
dp_sa = './.logger'
print >>sys.stderr, 'connecting to diagnostics port: %s' % dp_sa
try:
    dp_sock.connect(dp_sa)
    while True:
        data = dp_sock.recv(1024)
        if len(data) == 0:
            print >>sys.stderr, 'diagnostics port closed'
            sys.exit(0)
        print data,
except socket.error, msg:
    print >>sys.stderr, msg
    sys.exit(1)
except KeyboardInterrupt:
    sys.exit(0)
