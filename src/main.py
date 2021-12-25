from controller import Controller
from src.worker import ButtonWorker
from vjoy import VJoy

vjoy = VJoy(1)

controller_map = {
    "buttons": [2],
    "encoders": [
        [1, 3],
        [13, 14],
        [4, 5]
    ]
}


def get_thread_pool():
    button_id = 0
    pool = {}
    for _ in controller_map["buttons"]:
        pool[button_id] = ButtonWorker(vjoy, press_delay=20, queue_size=10)
        button_id += 1
    for _ in controller_map["encoders"]:
        pool[button_id] = ButtonWorker(vjoy, press_delay=20, queue_size=10)
        pool[button_id + 1] = pool[button_id]
        button_id += 2

    for _, t in pool.items():
        if not t.is_alive():
            t.start()
    return pool


def main():
    controller = Controller()
    controller.register(controller_map)
    controller.listen(press_buttons, get_thread_pool())


def press_buttons(data, workers_pool):
    buttons = data.split(",")
    for button, state in enumerate(buttons):
        workers_pool[button + 1].set_button((button + 1, int(state) == 1))


if __name__ == '__main__':
    main()
