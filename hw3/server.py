from socketserver import BaseRequestHandler, TCPServer
from socketserver import ThreadingTCPServer
import random
import string
import hashlib
import socket
from threading import Thread


class EchoHandler(BaseRequestHandler):
    def randomStringDigits(self, stringLength):
        """ Generate a random string of letters and digits """
        lettersAndDigits = string.ascii_letters + string.digits
        return ''.join(
            random.choice(lettersAndDigits) for i in range(stringLength))

    def hash_sha256(self, data):
        """ Generate sha256 hash """
        hash_value = hashlib.sha256(data.encode('utf-8')).hexdigest()
        return hash_value

    def checkPoW(self, hash_val):
        """ Check PoW correctness"""
        PoWDifficulty = 6
        for i in range(PoWDifficulty):
            if hash_val[i] != '0':
                return False
        return True

    def generate_echo_msg(self, msg):
        while True:
            """ Generate echo message: msg+seed+hash_value """
            seed = self.randomStringDigits(5)
            hash_value = self.hash_sha256(seed + msg)
            # print('hash value***: ', hash_value)
            # check PoW
            if (self.checkPoW(hash_value)):
                break
        echo_msg = msg + ',' + seed + ',' + hash_value + "\n"
        print(echo_msg)
        return echo_msg

    def handle(self):
        """ Main Tcp Server Handler """
        print('Got connection from', self.client_address)
        while True:
            msg = self.request.recv(8192)
            if not msg:
                break
            msg = msg.decode('utf-8').rstrip()
            print('Recv msg: ', msg)
            self.request.send(self.generate_echo_msg(msg).encode('utf-8'))


if __name__ == '__main__':
    serv = ThreadingTCPServer(('', 20000), EchoHandler)
    print('Listening on 127.0.0.1:8000...')
    serv.serve_forever()
