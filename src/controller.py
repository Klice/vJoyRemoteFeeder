import socket
import json
import xml.etree.ElementTree as ET
import urllib
import urllib.request

from ssdp import SSDP


class Controller:
    api_url = None
    ssdp_service_type = "ArduinoGameController"

    def __init__(self, ip=None, upd_port=6789, upd_host=None):
        self.api_url = "http://{ip}/".format(ip=ip) if ip else self._discover()
        self.upd_port = upd_port
        self.upd_host = upd_host if upd_host else self._get_self_ip()

    def _discover(self):
        ret = SSDP.discover(st=self.ssdp_service_type, retries=2)
        if len(ret) == 0:
            raise Exception('No controllers found')

        ssdp = ret[0]
        api_url = self._get_api_url(ssdp.location)
        print("Discovered {} on: {}".format(self.ssdp_service_type, api_url))
        return api_url
    
    def _get_api_url(self, url):
        desc = urllib.request.urlopen(urllib.request.Request(url)).read().decode('utf-8')
        root = ET.fromstring(desc)
        for e in root.iter():
            if "URLBase" in e.tag:
                return e.text

    def _get_self_ip(self):     
        o = urllib.parse.urlsplit(self.api_url)   
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect((o.hostname, o.port))
        my_ip = s.getsockname()[0]
        print("My IP: {}".format(my_ip))
        s.close()
        return my_ip

    def register(self, pin_map):
        data = {
            "udp_host": self.upd_host,
            "udp_port": self.upd_port,
            "pins": pin_map
        }
        req = urllib.request.Request("{url}register".format(url=self.api_url), data=json.dumps(data).encode('utf-8'))
        urllib.request.urlopen(req)

    def listen(self, callback, workers_pool):
        socket.setdefaulttimeout(None)
        server_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_sock.bind(("0.0.0.0", self.upd_port))
        try:
            while True:
                data, _ = server_sock.recvfrom(1024)
                callback(data.decode("utf-8"), workers_pool)
        except KeyboardInterrupt:
            pass
