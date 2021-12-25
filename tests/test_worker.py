import unittest
from unittest.mock import MagicMock

from src.worker import ButtonWorker


class WorkersTest(unittest.TestCase):
    def setUp(self):
        delay = 1
        self.vjoy_mock = MagicMock()
        self.worker = ButtonWorker(MagicMock(), press_delay=delay, queue_size=5)

    def tearDown(self):
        self.worker.kill()
        if self.worker.is_alive():
            self.worker.join()

    def test_queue_size(self):
        for _ in range(100):
            self.worker.set_button((1, True))
        self.assertEqual(5, self.worker.get_queue_size())

    def test_press(self):
        for _ in range(5):
            self.worker.set_button((1, True))
        self.worker.start()
        self.worker.queue.join()
        self.assertEqual(5, self.worker.vjoy.set_btn.call_count)
