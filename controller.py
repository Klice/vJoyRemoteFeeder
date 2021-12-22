from urllib import request


class Controller:
    def __init__(self, ip):
        self.ip = ip

    def register(self):
        request.Request("http://{ip}/register".format(ip=self.ip), data="hui".encode('utf-8'))
