from ctypes import cdll
from enum import Enum


class VjdStat(Enum):
    VJD_STAT_OWN = 0
    VJD_STAT_FREE = 1
    VJD_STAT_BUSY = 2
    VJD_STAT_MISS = 3
    VJD_STAT_UNKN = 4


class VJoy:
    vJoyLib = cdll.LoadLibrary('vJoyInterface.dll')

    def __init__(self, vjoy_interface):
        self.VJOY_INTERFACE = vjoy_interface
        self._chk_vjoy_enabled()
        self._chk_vjoy_status()
        self._acquire_interface()
        self.vJoyLib.ResetVJD(self.VJOY_INTERFACE)

    def set_btn(self, state, btn):
        self.vJoyLib.SetBtn(state, self.VJOY_INTERFACE, btn)

    def _chk_vjoy_enabled(self):
        if not self.vJoyLib.vJoyEnabled():
            print("Failed Getting vJoy attributes.")
            exit(-2)
        else:
            print("Vendor: %s\nProduct :%s\nVersion Number:%s" % (
                self.vJoyLib.GetvJoyManufacturerString(),
                self.vJoyLib.GetvJoyProductString(),
                self.vJoyLib.GetvJoySerialNumberString()
            ))

    def _chk_vjoy_status(self):
        vjoy_status = VjdStat(self.vJoyLib.GetVJDStatus(self.VJOY_INTERFACE))
        if vjoy_status == VjdStat.VJD_STAT_OWN:
            print("vJoy Device %d is already owned by this feeder\n" % self.VJOY_INTERFACE)
        elif vjoy_status == VjdStat.VJD_STAT_FREE:
            print("vJoy Device %d is free\n" % self.VJOY_INTERFACE)
        elif vjoy_status == VjdStat.VJD_STAT_BUSY:
            print("vJoy Device %d is already owned by another feeder\nCannot continue\n" % self.VJOY_INTERFACE)
            exit(-3)
        elif vjoy_status == VjdStat.VJD_STAT_MISS:
            print("vJoy Device %d is not installed or disabled\nCannot continue\n" % self.VJOY_INTERFACE)
            exit(-4)
        else:
            print("vJoy Device %d general error\nCannot continue\n" % self.VJOY_INTERFACE)
            exit(-1)
        print("Number of buttons\t\t%d\n" % self.vJoyLib.GetVJDButtonNumber(self.VJOY_INTERFACE))

    def _acquire_interface(self):
        vjoy_status = VjdStat(self.vJoyLib.GetVJDStatus(self.VJOY_INTERFACE))
        if vjoy_status == VjdStat.VJD_STAT_OWN or (
                vjoy_status == VjdStat.VJD_STAT_FREE and not self.vJoyLib.AcquireVJD(self.VJOY_INTERFACE)):
            print("Failed to acquire vJoy device number %d.\n" % self.VJOY_INTERFACE)
            exit(-1)
        else:
            print("Acquired: vJoy device number %d.\n" % self.VJOY_INTERFACE)
