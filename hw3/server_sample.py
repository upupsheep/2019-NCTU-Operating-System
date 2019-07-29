import socketserver
import threading
import time


class MiTcpHandler(socketserver.BaseRequestHandler):
    def handle(self):
        while 1:
            try:
                data = self.request.recv(1024)
                print(data)
                time.sleep(0.1)
                if not data:
                    break
            except:
                print("Exception error!")
                break


class ThreadServer(socketserver.ThreadingMixIn, socketserver.ForkingTCPServer):
    pass


def main():
    host = "localhost"
    port = 20000
    server = ThreadServer((host, port), MiTcpHandler)
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.start()
    print(" server starting...")


main()