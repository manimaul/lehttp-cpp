import json

import requests
import urllib3

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)


class Response(object):
    def __init__(self):
        self.headers = {}
        self.body = ''
        self.status = 0

    @property
    def str_body(self):
        return self.body

    @property
    def json_body(self):
        return json.loads(self.body)


_CHAR_ENCODING = 'iso-8859-1'
_TIMEOUT_SECS = 5


def http_get(url: str, headers: dict = None):
        r = requests.get(url, headers=headers, verify=False, timeout=_TIMEOUT_SECS)
        response = Response()
        response.headers = r.headers
        response.body = r.text
        response.status = r.status_code
        return response


def http_post(url: str, data: dict, headers: dict= None):
        r = requests.post(url, headers=headers, data=data, verify=False, timeout=_TIMEOUT_SECS)
        response = Response()
        response.headers = r.headers
        response.body = r.text
        response.status = r.status_code
        return response
