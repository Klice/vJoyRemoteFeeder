from time import sleep
from controller import Controller
from vjoy import VJoy


def main():
    controller = Controller("192.168.1.50")
    controller.register()

    vjoy = VJoy(1)
    while True:
        vjoy.set_btn(False, 3)
        sleep(1)
        vjoy.set_btn(True, 3)
        sleep(1)


if __name__ == '__main__':
    main()
