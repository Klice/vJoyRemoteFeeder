from time import sleep
from controller import Controller
from vjoy import VJoy

vjoy = VJoy(1)

def main():
    controller = Controller()
    controller.register()
    controller.listen(pressButtons)

def pressButtons(data):
    buttons = data.split(",")
    for button, state in enumerate(buttons):
        vjoy.set_btn(int(state) == 1, button + 1)
    sleep(20 / 1000)

if __name__ == '__main__':
    main()
