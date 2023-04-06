#!/usr/bin/env python3
from utils import *
import unittest


class InlineVectorWrapperTest(unittest.TestCase):
    def setUp(self):
        self.p = start(["../../../bin/inline_vector_wrapper"])

    def tearDown(self):
        terminate(self.p)

    def test_exit(self):
        read_menu(self.p)
        send_value(self.p, 5)
        read_exit(self.p)

    def test_wrong_key(self):
        read_menu(self.p)
        send_value(self.p, 44)
        read_invalid_choice(self.p)

    def add_value(self, value):
        read_menu(self.p)
        send_value(self.p, 1)
        read_add(self.p)
        send_value(self.p, value)
        read_value_added(self.p)

    def read_stats(self, size, capacity):
        read_menu(self.p)
        send_value(self.p, 3)
        read_size_capacity(self.p, size, capacity)

    def read_vector(self, values, empty=False):
        read_menu(self.p)
        send_value(self.p, 4)
        if empty:
            read_empty(self.p)
        else:
            read_vector_contents(self.p, values)

    def read_remove(self, empty=False):
        read_menu(self.p)
        send_value(self.p, 2)
        if empty:
            read_empty(self.p)
        else:
            read_remove(self.p)

    def test_add_value(self):
        self.read_stats(0, 5)
        self.add_value(1)
        self.read_stats(1, 5)
        self.add_value(-10)
        self.add_value(22)
        self.add_value(333333)
        self.add_value(0)
        self.read_stats(5, 5)
        self.read_vector([1, -10, 22, 333333, 0])
        self.add_value(1)
        self.read_stats(6, 8)
        self.read_vector([1, -10, 22, 333333, 0, 1])
        self.add_value(-441)
        self.read_stats(7, 8)
        self.read_vector([1, -10, 22, 333333, 0, 1, -441])

    def test_remove_value(self):
        self.test_add_value()
        self.read_remove()
        self.read_remove()
        self.read_stats(5, 5)
        self.read_vector([1, -10, 22, 333333, 0])
        self.read_remove()
        self.read_stats(4, 5)
        self.read_remove()
        self.read_remove()
        self.read_remove()
        self.read_remove()
        self.read_stats(0, 5)
        self.read_vector([], True)
        self.read_remove(True)

if __name__ == '__main__':
    unittest.main()
