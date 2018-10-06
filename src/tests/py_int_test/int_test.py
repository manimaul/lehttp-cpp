import os.path as path
import unittest
from unsafe_client import http_get, http_post, Response
from unittest import TestCase


class Test(TestCase):

    # @skip
    def test_get_foo(self):
        response = http_get(url='http://localhost:8080/foo')
        self.assertEqual("hello path GET foo", response.body)
        self.assertEqual("there", response.headers['hi'])
        self.assertEqual(200, response.status)

    # @skip
    def test_post_foo(self):
        response = http_post(url='http://localhost:8080/foo', data={})
        self.assertEqual("hello path POST foo", response.body)
        self.assertEqual("there", response.headers['hi'])
        self.assertEqual(200, response.status)


if __name__ == '__main__':
    unittest.main()
