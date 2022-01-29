import queue
import threading
from gs_time import delayMicroseconds
from time import sleep


class ButtonWorker(threading.Thread):
    def __init__(self, vjoy, press_delay=20, queue_size=10):
        threading.Thread.__init__(self)
        self.queue = queue.Queue(queue_size)
        self.press_delay = press_delay
        self.vjoy = vjoy
        self.kill_flag = False

    def set_button(self, state):
        try:
            self.queue.put(state, block=False)
        except queue.Full:
            pass

    def get_queue_size(self):
        return self.queue.qsize()

    def kill(self):
        self.kill_flag = True

    def run(self):
        while True:
            if self.kill_flag:
                break
            try:
                button_id, state = self.queue.get(timeout=1)
                self.vjoy.set_btn(int(state) == 1, button_id)
                delayMicroseconds(self.press_delay * 1000)
                self.queue.task_done()
            except queue.Empty:
                pass
